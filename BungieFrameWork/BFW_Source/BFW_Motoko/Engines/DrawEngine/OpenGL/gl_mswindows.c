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
void make_pixel_format_descriptor(PIXELFORMATDESCRIPTOR *pfd, word bit_depth);
static void gl_unload_opengl_dll(void);

/*---------- globals */

static DEVMODE original_display_mode= {0};
static WORD original_gamma_ramp[256 * 3]= {0};
static BOOL gamma_ramp_supported= FALSE;

/*---------- code */

void M3rSetGamma(
	float inGamma)
{
	// TODO

	// float power= 0.4f + (1.2f * (1.f - inGamma));

	// UUmAssert(sizeof(WORD) == sizeof(UUtUns16));
	
	// if (gamma_ramp_supported)
	// {
	// 	UUtInt32 itr;
	// 	WORD new_gamma_ramp[256 * 3];
		
	// 	for (itr= 0; itr<(256*3); itr++)
	// 	{
	// 		// WORD src_gamma = (itr % 256) << 8 | (itr % 256);
	// 		WORD src_gamma= original_gamma_ramp[itr];
	// 		float value= (src_gamma) / ((float) 0xFFFF);

	// 		value= (float)pow(value, power);
	// 		new_gamma_ramp[itr]= (WORD)UUmPin(MUrFloat_Round_To_Int(value * ((float) 0xFFFF)), 0, 0xFFFF);
	// 	}
	// 	// try the 3Dfx gamma control extensions first
	// 	if (GL_EXT(wglSetDeviceGammaRamp3DFX) != NULL)
	// 	{
	// 		GL_EXT(wglSetDeviceGammaRamp3DFX)(gl->device_context, new_gamma_ramp);
	// 	}
	// 	else
	// 	{
	// 		SetDeviceGammaRamp(gl->device_context, new_gamma_ramp);
	// 	}
	// }

	return;
}

static void RestoreGammaTable(
	void)
{
	// TODO

	// if (gamma_ramp_supported)
	// {
	// 	// try the 3Dfx gamma control extensions first
	// 	if (GL_EXT(wglSetDeviceGammaRamp3DFX) != NULL)
	// 	{
	// 		GL_EXT(wglSetDeviceGammaRamp3DFX)(gl->device_context, original_gamma_ramp);
	// 	}
	// 	else
	// 	{
	// 		SetDeviceGammaRamp(gl->device_context, original_gamma_ramp);
	// 	}
	// }

	return;
}

static void SetupGammaTable(
	void)
{
	// TODO

	// if (gl->device_context)
	// {
	// 	// try the 3Dfx gamma control extensions first
	// 	if (GL_EXT(wglGetDeviceGammaRamp3DFX) != NULL)
	// 	{
	// 		UUrStartupMessage("Using 3DFX gamma adjustment");
	// 		gamma_ramp_supported= GL_EXT(wglGetDeviceGammaRamp3DFX)(gl->device_context, original_gamma_ramp);
	// 	}
	// 	else
	// 	{
	// 		UUrStartupMessage("Using standard Windows gamma adjustment");
	// 		gamma_ramp_supported= GetDeviceGammaRamp(gl->device_context, original_gamma_ramp);
	// 	}

	// 	if (gamma_ramp_supported)
	// 	{
	// 		M3rSetGamma(ONrPersist_GetGamma());
	// 	}
	// 	else
	// 	{
	// 		UUrStartupMessage("gamma adjustment not supported");
	// 	}
	// }

	return;
}

static boolean fullscreen_candidate(
	HWND hwnd)
{
	RECT win_rect;
	RECT dt_rect;
	boolean success= FALSE;
    DWORD style= GetWindowLong (hwnd, GWL_STYLE);

	if ((style & WS_POPUP) && !(style & (WS_BORDER | WS_THICKFRAME)))
	{
		GetClientRect( hwnd, &win_rect );
		GetWindowRect(GetDesktopWindow(), &dt_rect);
		if (!memcmp(&win_rect, &dt_rect, sizeof(dt_rect)))
		{
			success= TRUE;
		}
	}

	return success;
}

static boolean set_display_settings(
	word width,
	word height,
	word depth)
{
	boolean success= FALSE;
	GLFWvidmode desired_display_mode= {0};

	UUmAssert(depth >= 16);
	int count = 0;
	const GLFWvidmode *display_modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
	success = display_modes != NULL;
	UUmAssert(success);

	if (success) {
		success = FALSE;

		const GLFWvidmode *display_mode = NULL;
		for (int i = 0; i < count; i++) {
			display_mode = &display_modes[i];

			int red_bits = depth == 32 ? 8 : 5;
			int green_bits = depth == 32 ? 8 : 6;
			int blue_bits = depth == 32 ? 8 : 5;
				
			if (display_modes[i].width == width
				&& display_modes[i].height == height
				&& display_modes[i].redBits == red_bits
				&& display_modes[i].greenBits == green_bits
				&& display_modes[i].blueBits == blue_bits) {
				success = TRUE;
				break;
			}		
		}
		
		if (success) {
			glfwWindowHint(GLFW_RED_BITS, display_mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, display_mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS,  display_mode->blueBits);
			glfwWindowHint(GLFW_DEPTH_BITS,  depth == 32 ? 32 : 16);
			 
			glfwWindowHint(GLFW_REFRESH_RATE, display_mode->refreshRate);
			
			gl->display_mode.bitDepth = depth;
			
			glfwSetWindowSize(ONgPlatformData.gameWindow, width, height);
			
			glfwMakeContextCurrent(ONgPlatformData.gameWindow);
		}
	}
	
	return success;
}

static void restore_display_settings(
	void)
{
	return;
}

void make_pixel_format_descriptor(
	PIXELFORMATDESCRIPTOR *pfd,
	word bit_depth)
{
	UUmAssert(pfd);
	memset(pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd->nSize= sizeof(PIXELFORMATDESCRIPTOR);
	pfd->nVersion= 1;
	pfd->dwFlags= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

	pfd->iPixelType= PFD_TYPE_RGBA;

	pfd->dwLayerMask= PFD_MAIN_PLANE;

	switch (bit_depth)
	{
		case 16:
			pfd->cColorBits= 16; 
			pfd->cDepthBits= 16;
			break;
		case 32:
			pfd->cColorBits= 24;
			pfd->cDepthBits= 32;
			break;
		default:
			pfd->cColorBits= 16;
			pfd->cDepthBits= 16;
			break;
	}

	return;
}

boolean gl_pixel_format_is_accelerated(
	PIXELFORMATDESCRIPTOR *pfd)
{
	boolean accelerated= TRUE;
	int generic_format, generic_accelerated;

	UUmAssert(pfd);

	generic_format= pfd->dwFlags & PFD_GENERIC_FORMAT;
	generic_accelerated= pfd->dwFlags & PFD_GENERIC_ACCELERATED;

	if (generic_format && ! generic_accelerated)
	{
		// software
		accelerated= FALSE;
	}
	else if (generic_format && generic_accelerated)
	{
		// hardware - MCD
	}
	else if (!generic_format && !generic_accelerated)
	{
		// hardware - ICD
	}
	else
	{
		accelerated= FALSE; // ?
	}

	return accelerated;
}

boolean gl_create_render_context(
	void)
{
	glfwMakeContextCurrent(ONgPlatformData.gameWindow);

	return TRUE;
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
		
		if (!success)
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

					current_width= gl->display_mode.width;
					current_height= gl->display_mode.height;
					current_depth= gl->display_mode.bitDepth;
					
					SetupGammaTable();
				}
				else
				{
					AUrMessageBox(AUcMBType_OK, "Failed to initialize OpenGL contexts; Oni will now exit.");
					exit(0);
				}
			}
		}
	}

	return success;
}

void gl_platform_dispose(
	void)
{
	RestoreGammaTable();

	restore_display_settings();

#ifdef GL_LOAD_API_DYNAMICALLY
	gl_unload_opengl_dll();
#endif

	return;	
}


void gl_display_back_buffer(
	void)
{
	glfwSwapBuffers(ONgPlatformData.gameWindow);

	return;
}

void gl_matrox_display_back_buffer(
	void)
{
	gl_display_back_buffer();

	return;
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

/*
	this function searches for some common opengl dlls in the windows registry
	it returns the first one it finds, which is not necessarily the correct one!!
*/
static char *gl_find_opengl_dll_from_registry(
	void)
{
	char *registry_subkey[]= {
		// NT/2000 subkeys
		"Software\\Microsoft\\Windows NT\\CurrentVersion\\OpenGLDrivers\\3dfx",
		"Software\\Microsoft\\Windows NT\\CurrentVersion\\OpenGLDrivers\\RIVATNT",
		"Software\\Microsoft\\Windows NT\\CurrentVersion\\OpenGLDrivers",
		// Win9x subkeys
		"Software\\Microsoft\\Windows\\CurrentVersion\\OpenGLDrivers\\3dfx",
		"Software\\Microsoft\\Windows\\CurrentVersion\\OpenGLDrivers\\RIVATNT",
		"Software\\Microsoft\\Windows\\CurrentVersion\\OpenGLDrivers",
		NULL};
	static char dll_filename[256]= "";
	boolean success= FALSE;
	int i=0;

	while ((registry_subkey[i] != NULL) && !success)
	{
		HKEY registry_key= NULL;
		long ret;
		
		ret= RegOpenKeyEx(HKEY_LOCAL_MACHINE, registry_subkey[i], 0, KEY_READ, &registry_key);
		
		if (ret == ERROR_SUCCESS)
		{
			long length= 256, type= REG_SZ;
			char *names[]= {"DLL", "Dll", "dll", NULL};
			int j=0;

			while ((names[j] != NULL) && !success)
			{
				int ret2= RegQueryValueEx(registry_key, names[j], NULL, (unsigned long*)&type, (unsigned char*)dll_filename, (unsigned long*)&length);
				
				if (ret2 == ERROR_SUCCESS)
				{
					success= TRUE; // found it
				}
				++j;
			}
		}
		if (registry_key)
		{
			RegCloseKey(registry_key);
		}
		++i;
	}

	if (!success) dll_filename[0]= '\0';

	return dll_filename;
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

#define LOAD_GL_FUNCTION(name)		GL_FXN(name)= (void*)GetProcAddress(opengl_dll, #name)
#define LOAD_GDI_FUNCTION(name)		GDI_FXN(name)= (void*)GetProcAddress(gdi_dll, #name)

static HANDLE opengl_dll= NULL;
//static HANDLE gdi_dll= NULL;

boolean gl_load_opengl_dll(
	void)
{
	boolean success= TRUE;
	HANDLE window= ONgPlatformData.gameWindow;

	opengl_dll= gl_load_dll(DEFAULT_WIN32_GL_DLL);
	if (!opengl_dll)
	{
		char *dll;

		/*
			this is potentially dangerous; namely, if there are drivers for more
			than 1 video card listed in the registry, we have no way of knowing if
			the one we pick is the right one - picking the wrong one will crash
			the app... ahh well, Windows users should be used to that! :O)
		*/
		dll= gl_find_opengl_dll_from_registry();
		if (dll && (dll[0] != '\0'))
		{
			opengl_dll= gl_load_dll(dll);
		}
	}

	if (opengl_dll) // verify hardware support
	{
		HDC device_context;
		boolean hardware= FALSE;

		device_context= GetDC(window);
		if (device_context)
		{
			PIXELFORMATDESCRIPTOR pfd;
			short pixel_format= 0;
			extern void make_pixel_format_descriptor(PIXELFORMATDESCRIPTOR *pfd, word bit_depth); // gl_mswindows.c
			extern boolean gl_pixel_format_is_accelerated(PIXELFORMATDESCRIPTOR *pfd); // ditto
			DEVMODE current_display_mode;

			current_display_mode.dmSize= sizeof(DEVMODE);
			current_display_mode.dmDriverExtra= 0;
			if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &current_display_mode))
			{
				// save the original display mode
				if (original_display_mode.dmSize == 0)
				{
					original_display_mode= current_display_mode;
				}

				if (current_display_mode.dmBitsPerPel < 16)
				{
					// display has to be 16 bpp or better to get hardware acceleration
					long result;
					int width, height;

					width= current_display_mode.dmPelsWidth;
					height= current_display_mode.dmPelsHeight;
					UUrMemory_Clear(&current_display_mode, sizeof(current_display_mode));
					current_display_mode.dmSize= sizeof(DEVMODE);
					current_display_mode.dmBitsPerPel= 16;
					current_display_mode.dmPelsWidth= width;
					current_display_mode.dmPelsHeight= height;
					current_display_mode.dmFields= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
					result= ChangeDisplaySettings(&current_display_mode, 0);
					// it's not terribly important that this succeed;
					// if this failed, make_pixel_format_descriptor() will return a software GL pixel format
					if (result == DISP_CHANGE_SUCCESSFUL)
					{
						// the device context will no longer be valid
						if (ReleaseDC(window, device_context))
						{
							// reload DLL... man what a bitch this is turning out to be
							FreeLibrary(opengl_dll);
							// only bother to look for default DLL;
							// they deserve to die a horrible death
							// if their POS card can't handle 16-bit
							opengl_dll= gl_load_dll(DEFAULT_WIN32_GL_DLL);
							// need to ensure we get a 16-bit window... grab the desktop
							// I HATE WINDOWS
							// I HATE WINDOWS
							// I HATE WINDOWS
							// I WORK AT MICROSOFT
							// oops
							window= GetDesktopWindow();
							device_context= GetDC(window);
						}
					}
				}
			}

			if (opengl_dll)
			{
				make_pixel_format_descriptor(&pfd, (UUtUns16)current_display_mode.dmBitsPerPel); // try for hardware GL
				pixel_format= ChoosePixelFormat(device_context, &pfd);
			}
			if (pixel_format)
			{
				hardware= gl_pixel_format_is_accelerated(&pfd);

				if (!hardware)
				{
					FreeLibrary(opengl_dll);
					opengl_dll= NULL;
				}
			}
			else
			{
				if (opengl_dll)
				{
					FreeLibrary(opengl_dll);
				}
				opengl_dll= NULL;
			}

			if (device_context)
			{
				ReleaseDC(window, device_context);
			}
		}
		else
		{
			if (opengl_dll)
			{
				FreeLibrary(opengl_dll);
			}
			opengl_dll= NULL;
		}
	}

	if (opengl_dll)
	{
		int i, n;

/*	#ifdef _WINGDI_
		LOAD_GDI_FUNCTION(ChoosePixelFormat);
		LOAD_GDI_FUNCTION(DescribePixelFormat);
		LOAD_GDI_FUNCTION(SetPixelFormat);
		LOAD_GDI_FUNCTION(GetPixelFormat);
		LOAD_GDI_FUNCTION(SwapBuffers);

		LOAD_GDI_FUNCTION(CopyContext);
		LOAD_GDI_FUNCTION(CreateContext);
		LOAD_GDI_FUNCTION(CreateLayerContext);
		LOAD_GDI_FUNCTION(DeleteContext);
		LOAD_GDI_FUNCTION(GetCurrentContext);
		LOAD_GDI_FUNCTION(GetCurrentDC);
		LOAD_GDI_FUNCTION(GetProcAddress);
		LOAD_GDI_FUNCTION(MakeCurrent);
		LOAD_GDI_FUNCTION(ShareLists);
		LOAD_GDI_FUNCTION(UseFontBitmaps);

		LOAD_GDI_FUNCTION(UseFontOutlines);

		LOAD_GDI_FUNCTION(DescribeLayerPlane);
		LOAD_GDI_FUNCTION(SetLayerPaletteEntries);
		LOAD_GDI_FUNCTION(GetLayerPaletteEntries);
		LOAD_GDI_FUNCTION(RealizeLayerPalette);
		LOAD_GDI_FUNCTION(SwapLayerBuffers);
	#endif // _WINGDI_*/

		gl_api->end_standard_api= (void *)NONE;

		n= sizeof(struct gl_api)/sizeof(void *);
		for (i=0; i<n; i++)
		{
			void **api_ptr= (void **)gl_api;

			if (api_ptr[i] == (void *)NONE) // gl_api->end_standard_api
			{
				break;
			}
			if (api_ptr[i] == NULL)
			{
				success= FALSE;
				break;
			}
		}
	}
	else
	{
		success= FALSE;
	}

	return success;
}

#define UNLOAD_GL_FUNCTION(name)	GL_FXN(name)= NULL

static void gl_unload_opengl_dll(
	void)
{
	if (opengl_dll)
	{
		FreeLibrary(opengl_dll);
		opengl_dll= NULL;
	}

#ifdef GL_LOAD_API_DYNAMICALLY
	if (gl_api)
#endif
	{
	}

	return;
}

#endif // __ONADA__

