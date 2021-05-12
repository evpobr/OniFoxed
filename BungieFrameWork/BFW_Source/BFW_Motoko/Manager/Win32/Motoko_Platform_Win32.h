// ======================================================================
// Motoko_Platform_Win32.h
// ======================================================================
#ifndef MOTOKO_PLATFORM_WIN32_H
#define MOTOKO_PLATFORM_WIN32_H

#include <windows.h>

// ======================================================================
// defines
// ======================================================================
#define RELEASE(p)	if (p) (p)->lpVtbl->Release(p);

// ======================================================================
// prototypes
// ======================================================================
char*
M3rPlatform_GetErrorMsg(
	HRESULT error);

// ======================================================================
#endif /* MOTOKO_PLATFORM_WIN32_H */