/*
	gl_mswindows.c
	Friday Aug. 11, 2000 5:32 pm
	Stefan
*/

#ifdef __ONADA__

/*---------- includes */

#include "gl_engine.h"
#include "Oni_Platform.h"
#include "Oni_Persistance.h"

/*---------- prototypes */

static boolean fullscreen_candidate(HWND hwnd);
static void restore_display_settings(void);
void make_pixel_format_descriptor(word bit_depth);
static void gl_unload_opengl_dll(void);

/*---------- globals */

static DEVMODE original_display_mode= {0};
static WORD original_gamma_ramp[256 * 3]= {0};
static BOOL gamma_ramp_supported= FALSE;

/*---------- code */

void M3rSetGamma(
	float inGamma)
{
	Uint16 ramp[256];
	SDL_CalculateGammaRamp(inGamma + 0.5f, ramp);
	SDL_SetWindowGammaRamp(ONgPlatformData.sdlWindow, ramp, ramp, ramp);
}

static boolean set_display_settings(
	word width,
	word height,
	word depth)
{
	SDL_DisplayMode mode =
		{
			.format = depth == 32 ? SDL_PIXELFORMAT_RGB888 : SDL_PIXELFORMAT_RGB565,
    		.w= width,
    		.h= height
		};

	int ret = SDL_SetWindowDisplayMode(ONgPlatformData.sdlWindow, &mode);
	if (ret != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO, " SDL_SetWindowDisplayMode() failed with \"%s\"", SDL_GetError());
		return FALSE;
	}
	
	SDL_Log("SDL_SetWindowDisplayMode() succeded, display mode is set to %dx%d %d bit", width, height, depth);

	return ret == 0;	
}

static void restore_display_settings(
	void)
{
	DEVMODE current_display_settings= {0};

	current_display_settings.dmSize= sizeof(DEVMODE);
	if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &current_display_settings) &&
		((current_display_settings.dmBitsPerPel != original_display_mode.dmBitsPerPel) ||
		(current_display_settings.dmPelsWidth != original_display_mode.dmPelsWidth) ||
		(current_display_settings.dmPelsHeight != original_display_mode.dmPelsHeight)))
	{
		long result= ChangeDisplaySettings(&original_display_mode, 0);
		UUmAssert(result == DISP_CHANGE_SUCCESSFUL);
	}

	return;
}

void make_pixel_format_descriptor(
	word bit_depth)
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	switch (bit_depth)
	{
		case 16:
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 6);
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
			break;
		case 32:
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
			break;
		default:
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
			break;
	}

	return;
}

boolean gl_pixel_format_is_accelerated(void)
{
	int value = 0;

	if (SDL_GL_GetAttribute(SDL_GL_ACCELERATED_VISUAL, &value) == 0)
	{
		return FALSE;
	}
	return value != 0 ? TRUE : FALSE;
}

boolean gl_create_render_context(
	void)
{
	boolean success= FALSE;
	
	if (gl)
	{
		if (gl->render_context)
		{
			SDL_GL_DeleteContext(gl->render_context);
			SDL_GL_MakeCurrent(NULL, NULL);
		}

		make_pixel_format_descriptor(gl->display_mode.bitDepth);
		
		gl->render_context= SDL_GL_CreateContext(ONgPlatformData.sdlWindow);
		success = gl->render_context != NULL;
		if (success)
		{
			success = SDL_GL_MakeCurrent(ONgPlatformData.sdlWindow, gl->render_context) == 0 ? 1 : 0;
		}
		else
		{
			UUrDebuggerMessage("SDL_GL_CreateContext() failed : %s", SDL_GetError());
		}
	}
	return success;
}

//#define DESTROY_CONTEXTS_ON_DISPLAY_CHANGE we will just crash on cards that don't work with this
// WARNING! This call is also used for resolution switching!!!
UUtBool gl_platform_initialize(
	void)
{
	UUtBool success= UUcTrue;
	static word current_width= 0;
	static word current_height= 0;
	static word current_depth= 0;
	word width= gl->display_mode.width;
	word height= gl->display_mode.height;
	word depth= gl->display_mode.bitDepth;

	if ((width != current_width) || (height != current_height) || (depth != current_depth))
	{
		success= set_display_settings(width, height, depth);
	}

	if (success == FALSE)
	{
		if (gl->display_mode.bitDepth != 16 || (width != 640) || (height != 480))
		{
			width= 640;
			height= 480;
			depth= 16;
			success= set_display_settings(width, height, depth);
			if (success)
			{
				// sure the higher level code thinks the bit depth is different, but who cares? not me!
				gl->display_mode.bitDepth= depth;
			}
		}
	}

	if (success)
	{
		UUmAssert(ONgPlatformData.gameWindow && (ONgPlatformData.gameWindow != INVALID_HANDLE_VALUE));

		M3rSetGamma(ONrPersist_GetGamma());

		success= gl_create_render_context();
		if (!success)
		{
			if (depth != 16)
			{
				// try again at 16-bit
				success= set_display_settings(width, height, 16);
				if (success)
				{
					gl->display_mode.bitDepth= depth= 16;
					success= gl_create_render_context();
				}
			}
		}
	}

	if (!success) // the app doesn't handle failure so we do it here
	{
		AUrMessageBox(AUcMBType_OK, "Failed to initialize OpenGL contexts; Oni will now exit.");
		exit(0);
	}
	else
	{
		current_width= gl->display_mode.width;
		current_height= gl->display_mode.height;
		current_depth= gl->display_mode.bitDepth;
	}

	return success;
}

void gl_platform_dispose(
	void)
{
	word screen_width, screen_height;

	UUmAssert(gl->render_context);
	SDL_GL_DeleteContext(gl->render_context);
	
	UUmAssert(ONgPlatformData.gameWindow && (ONgPlatformData.gameWindow != INVALID_HANDLE_VALUE));

	screen_width= GetSystemMetrics(SM_CXSCREEN);
	screen_height= GetSystemMetrics(SM_CYSCREEN);

#ifdef GL_LOAD_API_DYNAMICALLY
	gl_unload_opengl_dll();
#endif

	return;	
}


void gl_display_back_buffer(
	void)
{
	SDL_GL_SwapWindow(ONgPlatformData.sdlWindow);
}

void gl_matrox_display_back_buffer(
	void)
{
	gl_display_back_buffer();
}

// fills in an array of display modes; returns number of elements in array
// array MUST BE AT LEAST M3cMaxDisplayModes elements in size!!!
int gl_enumerate_valid_display_modes(
	M3tDisplayMode display_mode_list[M3cMaxDisplayModes])
{
	M3tDisplayMode desired_display_mode_list[]= {
			{640, 480, 16, 0},
			{800, 600, 16, 0},
			{1024, 768, 16, 0},
			{1152, 864,	16, 0},
			//{1280, 1024, 16, 0},
			{1600, 1200, 16, 0},
			{1920, 1080, 16, 0},
			//{1920, 1200, 16, 0},
			{640, 480, 32, 0},
			{800, 600, 32, 0},
			{1024, 768, 32, 0},
			{1152, 864, 32, 0},
			//{1280, 1024, 32, 0},
			{1600, 1200, 32, 0},
			{1920, 1080, 32, 0}
			/*{1920, 1200, 32, 0}*/};
	int n= sizeof(desired_display_mode_list)/sizeof(desired_display_mode_list[0]);
	int i, num_valid_display_modes= 0;

	for (i=0; i<n && i<M3cMaxDisplayModes; i++)
	{
		DEVMODE desired_display_mode;

		memset(&desired_display_mode, 0, sizeof(desired_display_mode));
		desired_display_mode.dmSize= sizeof(DEVMODE);
		desired_display_mode.dmBitsPerPel= desired_display_mode_list[i].bitDepth;
		desired_display_mode.dmPelsWidth= desired_display_mode_list[i].width;
		desired_display_mode.dmPelsHeight= desired_display_mode_list[i].height;
		desired_display_mode.dmFields= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&desired_display_mode, CDS_TEST) == DISP_CHANGE_SUCCESSFUL)
		{
			display_mode_list[num_valid_display_modes]= desired_display_mode_list[i];
			++num_valid_display_modes;
		}
	}

	return num_valid_display_modes;
}

static HANDLE gl_load_dll(
	char *name)
{
	HANDLE dll= NULL;

	dll= GetModuleHandle(name);
	if (dll == NULL)
	{
		dll= LoadLibrary(name);
	}

	return dll;
}

//static HANDLE gdi_dll= NULL;

boolean gl_load_opengl_dll(
	void)
{
	return TRUE;
}

#endif // __ONADA__

