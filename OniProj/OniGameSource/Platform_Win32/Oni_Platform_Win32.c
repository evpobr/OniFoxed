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

#include <GLFW/glfw3native.h>

#include "BFW_Motoko.h"
#include "BFW_LocalInput.h"
#include "BFW_AppUtilities.h"
#include "BFW_LI_Platform_Win32.h"

#include "Oni.h"

#include "Oni_Platform.h"

#include "BFW_Console.h"

struct OniGLFWKeyboardMap {
	LItKeyCode	oniKeyCode;
	int			glfwKey;
};

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
static int	ONgKeyModifiers;
IMtPoint2D	ONgMousePos;

// ======================================================================
// prototypes
// ======================================================================
void UUcExternal_Call main(int argc, char *argv[]);

// ======================================================================
// functions
// ======================================================================
// ----------------------------------------------------------------------

static UUtUns32 ONiTranslateGLFWKey(int key)
{
	switch (key) {
	case GLFW_KEY_SPACE:
		return LIcKeyCode_Space;
	case GLFW_KEY_APOSTROPHE:
		return LIcKeyCode_Apostrophe;
	case GLFW_KEY_COMMA:
		return LIcKeyCode_Comma;
	case GLFW_KEY_MINUS:
		return LIcKeyCode_Minus;
	case GLFW_KEY_PERIOD:
		return LIcKeyCode_Period;
	case GLFW_KEY_SLASH:
		return LIcKeyCode_Slash;
	case GLFW_KEY_0:
		return LIcKeyCode_0;
	case GLFW_KEY_1:
		return LIcKeyCode_1;
	case GLFW_KEY_2:
		return LIcKeyCode_2;
	case GLFW_KEY_3:
		return LIcKeyCode_3;
	case GLFW_KEY_4:
		return LIcKeyCode_4;
	case GLFW_KEY_5:
		return LIcKeyCode_5;
	case GLFW_KEY_6:
		return LIcKeyCode_6;
	case GLFW_KEY_7:
		return LIcKeyCode_7;
	case GLFW_KEY_8:
		return LIcKeyCode_8;
	case GLFW_KEY_9:
		return LIcKeyCode_9;
	case GLFW_KEY_SEMICOLON:
		return LIcKeyCode_Semicolon;
	case GLFW_KEY_EQUAL:
		return LIcKeyCode_Equals;
	case GLFW_KEY_A:
		return LIcKeyCode_A;
	case GLFW_KEY_B:
		return LIcKeyCode_B;
	case GLFW_KEY_C:
		return LIcKeyCode_C;
	case GLFW_KEY_D:
		return LIcKeyCode_D;
	case GLFW_KEY_E:
		return LIcKeyCode_E;
	case GLFW_KEY_F:
		return LIcKeyCode_F;
	case GLFW_KEY_G:
		return LIcKeyCode_G;
	case GLFW_KEY_H:
		return LIcKeyCode_H;
	case GLFW_KEY_I:
		return LIcKeyCode_I;
	case GLFW_KEY_J:
		return LIcKeyCode_J;
	case GLFW_KEY_K:
		return LIcKeyCode_K;
	case GLFW_KEY_L:
		return LIcKeyCode_L;
	case GLFW_KEY_M:
		return LIcKeyCode_M;
	case GLFW_KEY_N:
		return LIcKeyCode_N;
	case GLFW_KEY_O:
		return LIcKeyCode_O;
	case GLFW_KEY_P:
		return LIcKeyCode_P;
	case GLFW_KEY_Q:
		return LIcKeyCode_Q;
	case GLFW_KEY_R:
		return LIcKeyCode_R;
	case GLFW_KEY_S:
		return LIcKeyCode_S;
	case GLFW_KEY_T:
		return LIcKeyCode_T;
	case GLFW_KEY_U:
		return LIcKeyCode_U;
	case GLFW_KEY_V:
		return LIcKeyCode_V;
	case GLFW_KEY_W:
		return LIcKeyCode_W;
	case GLFW_KEY_X:
		return LIcKeyCode_X;
	case GLFW_KEY_Y:
		return LIcKeyCode_Y;
	case GLFW_KEY_Z:
		return LIcKeyCode_Z;
	case GLFW_KEY_LEFT_BRACKET:
		return LIcKeyCode_LeftBracket;
	case GLFW_KEY_BACKSLASH:
		return LIcKeyCode_BackSlash;
	case GLFW_KEY_RIGHT_BRACKET:
		return LIcKeyCode_RightBracket;
	case GLFW_KEY_GRAVE_ACCENT:
		return LIcKeyCode_Grave;
	// case GLFW_KEY_WORLD_1:
	// case GLFW_KEY_WORLD_2:
	case GLFW_KEY_ESCAPE:
		return  LIcKeyCode_Escape;
	case GLFW_KEY_ENTER:
		return LIcKeyCode_Return;
	case GLFW_KEY_TAB:
		return LIcKeyCode_Tab;
	case GLFW_KEY_BACKSPACE:
		return LIcKeyCode_BackSpace;
	case GLFW_KEY_INSERT:
		return LIcKeyCode_Insert;
	case GLFW_KEY_DELETE:
		return LIcKeyCode_Delete;
	case GLFW_KEY_RIGHT:
		return LIcKeyCode_RightArrow;
	case GLFW_KEY_LEFT:
		return LIcKeyCode_LeftArrow;
	case GLFW_KEY_DOWN:
		return LIcKeyCode_DownArrow;
	case GLFW_KEY_UP:
		return LIcKeyCode_UpArrow;
	case GLFW_KEY_PAGE_UP:
		return LIcKeyCode_PageUp;
	case GLFW_KEY_PAGE_DOWN:
		return LIcKeyCode_PageDown;
	case GLFW_KEY_HOME:
		return LIcKeyCode_Home;
	case GLFW_KEY_END:
		return LIcKeyCode_End;
	case GLFW_KEY_CAPS_LOCK:
		return LIcKeyCode_CapsLock;
	case GLFW_KEY_SCROLL_LOCK:
		return LIcKeyCode_ScrollLock;
	case GLFW_KEY_NUM_LOCK:
		return LIcKeyCode_NumLock;
	case GLFW_KEY_PRINT_SCREEN:
		return LIcKeyCode_PrintScreen;
	case GLFW_KEY_PAUSE:
		return LIcKeyCode_Pause;
	case GLFW_KEY_F1:
		return LIcKeyCode_F1;
	case GLFW_KEY_F2:
		return LIcKeyCode_F2;
	case GLFW_KEY_F3:
		return LIcKeyCode_F3;
	case GLFW_KEY_F4:
		return LIcKeyCode_F4;
	case GLFW_KEY_F5:
		return LIcKeyCode_F5;
	case GLFW_KEY_F6:
		return LIcKeyCode_F6;
	case GLFW_KEY_F7:
		return LIcKeyCode_F7;
	case GLFW_KEY_F8:
		return LIcKeyCode_F8;
	case GLFW_KEY_F9:
		return LIcKeyCode_F9;
	case GLFW_KEY_F10:
		return LIcKeyCode_F10;
	case GLFW_KEY_F11:
		return LIcKeyCode_F11;
	case GLFW_KEY_F12:
		return LIcKeyCode_F12;
	case GLFW_KEY_F13:
		return LIcKeyCode_F13;
	case GLFW_KEY_F14:
		return LIcKeyCode_F14;
	case GLFW_KEY_F15:
		return LIcKeyCode_F15;
	// case GLFW_KEY_F16:
	// case GLFW_KEY_F17:
	// case GLFW_KEY_F18:
	// case GLFW_KEY_F19:
	// case GLFW_KEY_F20:
	// case GLFW_KEY_F21:
	// case GLFW_KEY_F22:
	// case GLFW_KEY_F23:
	// case GLFW_KEY_F24:
	// case GLFW_KEY_F25:
	case GLFW_KEY_KP_0:
		return  LIcKeyCode_NumPad0;
	case GLFW_KEY_KP_1:
		return  LIcKeyCode_NumPad1;
	case GLFW_KEY_KP_2:
		return  LIcKeyCode_NumPad2;
	case GLFW_KEY_KP_3:
		return  LIcKeyCode_NumPad3;
	case GLFW_KEY_KP_4:
		return  LIcKeyCode_NumPad4;
	case GLFW_KEY_KP_5:
		return  LIcKeyCode_NumPad5;
	case GLFW_KEY_KP_6:
		return  LIcKeyCode_NumPad6;
	case GLFW_KEY_KP_7:
		return  LIcKeyCode_NumPad7;
	case GLFW_KEY_KP_8:
		return  LIcKeyCode_NumPad8;
	case GLFW_KEY_KP_9:
		return  LIcKeyCode_NumPad9;
	case GLFW_KEY_KP_DECIMAL:
		return  LIcKeyCode_Decimal;
	case GLFW_KEY_KP_DIVIDE:
		return LIcKeyCode_Divide;
	case GLFW_KEY_KP_MULTIPLY:
		return LIcKeyCode_Multiply;
	case GLFW_KEY_KP_SUBTRACT:
		return LIcKeyCode_Subtract;
	case GLFW_KEY_KP_ADD:
		return LIcKeyCode_Add;
	case GLFW_KEY_KP_ENTER:
		return LIcKeyCode_NumPadEnter;
	case GLFW_KEY_KP_EQUAL:
		return LIcKeyCode_NumPadEquals;
	case GLFW_KEY_LEFT_SHIFT:
		return LIcKeyCode_LeftShift;
	case GLFW_KEY_LEFT_CONTROL:
		return LIcKeyCode_LeftControl;
	case GLFW_KEY_LEFT_ALT:
		return LIcKeyCode_LeftAlt;
	case GLFW_KEY_LEFT_SUPER:
		return LIcKeyCode_LeftWindowsKey;
	case GLFW_KEY_RIGHT_SHIFT:
		return LIcKeyCode_RightShift;
	case GLFW_KEY_RIGHT_CONTROL:
		return LIcKeyCode_RightControl;
	case GLFW_KEY_RIGHT_ALT:
		return LIcKeyCode_RightAlt;
	case GLFW_KEY_RIGHT_SUPER:
		return LIcKeyCode_RightWindowsKey;
	case GLFW_KEY_MENU:
		return LIcKeyCode_AppMenuKey;
	default:
		return LIcKeyCode_None;
	};
}

// ----------------------------------------------------------------------
void ONiHandleKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	LItInputEventType inEventType;
	
	ONgKeyModifiers = mods;

	switch (action) {
	case GLFW_PRESS:
		inEventType = LIcInputEvent_KeyDown;
		break;
	case GLFW_RELEASE:
		inEventType = LIcInputEvent_KeyUp;
		break;
	case GLFW_REPEAT:
		inEventType = LIcInputEvent_KeyRepeat;
		break;
	
	default:
		inEventType = LIcInputEvent_None;
		break;
	}

	UUtUns32 inKey = ONiTranslateGLFWKey(key);
	
	LIrInputEvent_Add(inEventType, NULL, inKey, ONgKeyModifiers);
}

// ----------------------------------------------------------------------
static void ONiHandleMousePosEvent(GLFWwindow* window, double xpos, double ypos)
{
	ONgMousePos.x = xpos;
	ONgMousePos.y = ypos;

	// TODO: modifiers
	LIrInputEvent_Add(LIcInputEvent_MouseMove, &ONgMousePos, 0, ONgKeyModifiers);
}

// ----------------------------------------------------------------------
static void ONiHandleMouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
	ONgKeyModifiers = mods;

	LItInputEventType inEventType;
	
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		inEventType = action == GLFW_PRESS ? LIcInputEvent_LMouseDown : LIcInputEvent_LMouseUp;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		inEventType = action == GLFW_PRESS ? LIcInputEvent_RMouseDown : LIcInputEvent_RMouseUp;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		inEventType = action == GLFW_PRESS ? LIcInputEvent_MMouseDown : LIcInputEvent_MMouseUp;
		break;
	default:
		inEventType = LIcInputEvent_None;
		break;
	}
	
	LIrInputEvent_Add(inEventType, &ONgMousePos, 0, ONgKeyModifiers);
}

// ----------------------------------------------------------------------
void ONiHandleWindowFocusEvent(GLFWwindow* window, int focused)
{
	LIrGameIsActive(focused == GLFW_TRUE ? UUcTrue : UUcFalse);
}

// ----------------------------------------------------------------------
static void
ONiPlatform_CreateWindow(
	ONtPlatformData		*ioPlatformData)
{
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);

	ioPlatformData->gameWindow = glfwCreateWindow(
		vidmode->width,
		vidmode->height,
		ONcMainWindowTitle,
		monitor,
		NULL);
		
	glfwSetKeyCallback(ioPlatformData->gameWindow, ONiHandleKeyEvent);
	glfwSetCursorPosCallback(ioPlatformData->gameWindow, ONiHandleMousePosEvent);
	glfwSetMouseButtonCallback(ioPlatformData->gameWindow, ONiHandleMouseButtonEvent);
	glfwSetWindowFocusCallback(ioPlatformData->gameWindow, ONiHandleWindowFocusEvent);
	
	ioPlatformData->hWnd = glfwGetWin32Window(ioPlatformData->gameWindow);
}


// ----------------------------------------------------------------------
UUtError ONrPlatform_Initialize(
	ONtPlatformData			*outPlatformData)
{
	if (glfwInit() == GLFW_FALSE) {
		return UUcError_Generic;
	}
	
	outPlatformData->appInstance = ONgAppHInstance;
	
	ONiPlatform_CreateWindow(outPlatformData);

	ShowCursor(FALSE);	// FALSE = hide the cursor
	
	return UUcError_None;
}

// ----------------------------------------------------------------------
UUtBool
ONrPlatform_IsForegroundApp(
	void)
{

	return (ONgPlatformData.hWnd == GetForegroundWindow());
}

// ----------------------------------------------------------------------
void ONrPlatform_Terminate(
	void)
{
	if(ONgErrorFile != NULL)
	{
		fclose(ONgErrorFile);
	}

	glfwDestroyWindow(ONgPlatformData.gameWindow);
	
	glfwTerminate();
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
