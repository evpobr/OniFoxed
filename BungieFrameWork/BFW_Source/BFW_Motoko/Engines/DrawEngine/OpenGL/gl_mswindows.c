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

#include "SDL2/SDL_pixels.h"
#include <SDL_syswm.h>

#define ONcMainWindowTitle	TEXT("ONI ")

void M3rSetGamma(
	float inGamma)
{
	Uint16 ramp[256];
	SDL_CalculateGammaRamp(inGamma + 0.5f, ramp);
	SDL_SetWindowGammaRamp(ONgPlatformData.sdlWindow, ramp, ramp, ramp);

	return;
}

static boolean gl_create_render_context(
	void)
{
	UUtBool success;
	if (!gl->context)
	{
		gl->context= SDL_GL_CreateContext(ONgPlatformData.sdlWindow);
		if (!gl->context)
		{
			UUrDebuggerMessage("wglCreateContext() failed : %s", SDL_GetError());
		}
	}
	success= ((gl->context != NULL) &&
		SDL_GL_MakeCurrent(ONgPlatformData.sdlWindow, gl->context) == 0);
	return success;
}

//#define DESTROY_CONTEXTS_ON_DISPLAY_CHANGE we will just crash on cards that don't work with this
// WARNING! This call is also used for resolution switching!!!
UUtBool gl_platform_initialize(
	void)
{
	UUtBool success= UUcTrue;
	static int current_width= 0;
	static int current_height= 0;
	static int current_depth= 0;
	int width= gl->display_mode.width;
	int height= gl->display_mode.height;
	int depth= gl->display_mode.bitDepth;
	
	if (ONgPlatformData.sdlWindow)
	{
		
		SDL_DisplayMode mode =
			{
    			.format = depth == 32 ? SDL_PIXELFORMAT_RGB888 : SDL_PIXELFORMAT_RGB565,
    			.w = width,
    			.h = height
			};
		success = SDL_SetWindowDisplayMode(ONgPlatformData.sdlWindow, &mode) == 0;
	}
	else
	{
		ONgPlatformData.sdlWindow = SDL_CreateWindow(
			ONcMainWindowTitle,
			0,
			0,
			width,
			height,
			SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
	
			SDL_SysWMinfo wmInfo;
			SDL_VERSION(&wmInfo.version);
			SDL_GetWindowWMInfo(ONgPlatformData.sdlWindow, &wmInfo);
			ONgPlatformData.gameWindow = wmInfo.info.win.window;
		
		success = ONgPlatformData.sdlWindow != NULL;
	}

	if (success)
	{
		M3rSetGamma(ONrPersist_GetGamma());

		success= gl_create_render_context();
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
	UUtBool success;

	success= SDL_GL_MakeCurrent(NULL, NULL) == 0;
	UUmAssert(success);

	UUmAssert(gl->context);
	SDL_GL_DeleteContext(gl->context);
	
	UUmAssert(ONgPlatformData.gameWindow);

	return;	
}


void gl_display_back_buffer(
	void)
{
	SDL_GL_SwapWindow(ONgPlatformData.sdlWindow);

	return;
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
		//TODO: filter available modes
		display_mode_list[num_valid_display_modes]= desired_display_mode_list[i];
		++num_valid_display_modes;
	}

	return num_valid_display_modes;
}

#endif
