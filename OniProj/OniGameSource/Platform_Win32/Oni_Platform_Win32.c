/*
	FILE:	Oni_Platform_Win32.c
	
	AUTHOR:	Brent H. Pease, Michael Evans, Kevin Armstrong
	
	CREATED: May 26, 1997
	
	PURPOSE: Win32 specific code
	
	Copyright 1997, 2000

*/
// ======================================================================
// includes
// ======================================================================
#include "BFW.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

#include <SDL_syswm.h>

#include "BFW_Motoko.h"
#include "BFW_LocalInput.h"
#include "BFW_AppUtilities.h"
#include "BFW_LI_Platform_Win32.h"

#include "Oni.h"

#include "Oni_Platform.h"

#include "BFW_Console.h"

// ======================================================================
// defines
// ======================================================================
#define ONcMainWindowClass	TEXT("ONI ")
#define ONcMainWindowTitle	TEXT("ONI ")

#define	ONcSurface_Width	MScScreenWidth
#define ONcSurface_Height	MScScreenHeight

#define ONcSurface_Left	0
#define ONcSurface_Top	0

#if defined(DEBUGGING) && DEBUGGING

	#define DEBUG_AKIRA 1

#endif

// ======================================================================
// globals
// ======================================================================

HINSTANCE	ONgAppHInstance = NULL;
int			ONgICmdShow;
FILE*		ONgErrorFile = NULL;
UUtBool		ONgShiftDown = UUcFalse;

// ======================================================================
// functions
// ======================================================================
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
static void
ONiPlatform_CreateWindow(
	ONtPlatformData		*ioPlatformData)
{
	UUtUns16 screen_width = GetSystemMetrics(SM_CXSCREEN);
	UUtUns16 screen_height = GetSystemMetrics(SM_CYSCREEN);

	ioPlatformData->sdlWindow = SDL_CreateWindow(
		ONcMainWindowTitle,
		ONcSurface_Left,
		ONcSurface_Top,
		screen_width,
		screen_height,
		SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);

	UUmAssert(ioPlatformData->sdlWindow != NULL);

	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(ONgPlatformData.sdlWindow, &wmInfo);
	ONgPlatformData.gameWindow = wmInfo.info.win.window;

	return;
}


// ----------------------------------------------------------------------
UUtError ONrPlatform_Initialize(
	ONtPlatformData			*outPlatformData)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	outPlatformData->appInstance = ONgAppHInstance;
	
	ONiPlatform_CreateWindow(outPlatformData);

	SDL_ShowCursor(SDL_DISABLE);	// FALSE = hide the cursor
	
	return UUcError_None;
}

// ----------------------------------------------------------------------
UUtBool
ONrPlatform_IsForegroundApp(
	void)
{
	return (ONgPlatformData.gameWindow == GetForegroundWindow());
}

// ----------------------------------------------------------------------
void ONrPlatform_Terminate(
	void)
{
	if(ONgErrorFile != NULL)
	{
		fclose(ONgErrorFile);
	}
	
	SDL_DestroyWindow(ONgPlatformData.sdlWindow);
	ONgPlatformData.sdlWindow = NULL;
	
	SDL_Quit();
}

// ----------------------------------------------------------------------
void ONrPlatform_Update(
	void)
{
}

// ----------------------------------------------------------------------
void ONrPlatform_ErrorHandler(
	UUtError			theError,
	char				*debugDescription,
	UUtInt32			userDescriptionRef,
	char				*message)
{

	if(ONgErrorFile == NULL)
	{
		ONgErrorFile = UUrFOpen("oniErr.txt", "wb");

		if(ONgErrorFile == NULL)
		{
			/* XXX - Someday bitch really loudly */
		}
	}
	
	fprintf(ONgErrorFile, "InternalError: %s, %s\n\r", debugDescription, message);
}

void
ONrPlatform_CopyAkiraToScreen(
	UUtUns16	inBufferWidth,
	UUtUns16	inBufferHeight,
	UUtUns16	inRowBytes,
	UUtUns16*	inBaseAdddr);
	
void
ONrPlatform_CopyAkiraToScreen(
	UUtUns16	inBufferWidth,
	UUtUns16	inBufferHeight,
	UUtUns16	inRowBytes,
	UUtUns16*	inBaseAdddr)
{

}

#ifdef ONI_MAP_FILE
extern int handle_exception(LPEXCEPTION_POINTERS);
extern void stack_walk_initialize(void);
#endif

// ----------------------------------------------------------------------
int WINAPI WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	PSTR		ssCmdLine,
	int			iCmdShow)
{
//	MSG	msg;
#define iMaxArguments 20
	int argc;
	char *argv[iMaxArguments + 1];

#ifdef ONI_MAP_FILE
	stack_walk_initialize();
#endif

	ONgAppHInstance = hInstance;
	ONgICmdShow = iCmdShow;

	argv[0] = "";
	AUrBuildArgumentList(ssCmdLine, iMaxArguments, (UUtUns32*)&argc, argv + 1);

#if (UUmPlatform != UUmPlatform_Win32) || (defined(DEBUGGING) && DEBUGGING)

	#ifdef ONI_MAP_FILE
	__try {
	#endif

	main(argc + 1, argv);

	#ifdef ONI_MAP_FILE
	} __except (handle_exception(GetExceptionInformation())) {}
	#endif

#else
	{
		DEVMODE original_display_mode;
		BOOL success;

		original_display_mode.dmSize = sizeof(DEVMODE);
		original_display_mode.dmDriverExtra = 0;

		success = EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &original_display_mode);
		UUmAssert(success);

	// FIXME: add SEH for mingw
	#ifdef _MSC_VER
		__try
	#endif
		{
			main(argc + 1, argv);
		}
	#ifdef _MSC_VER
	#ifdef ONI_MAP_FILE
		__except (handle_exception(GetExceptionInformation())) {
	#else
		__except (1) {
	#endif
			ChangeDisplaySettings(&original_display_mode, 0);
			MessageBox(NULL, "Blam, Oni crashed", "damn!", MB_OK);
		}
	#endif
	}
#endif

	return 0;
}
