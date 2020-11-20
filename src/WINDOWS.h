#ifndef _LIBCORE_WINDOWS_H
#define _LIBCORE_WINDOWS_H

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <windows.h>

#pragma comment (lib, "user32.lib")
#pragma comment (lib, "gdi32.lib")
#pragma comment (lib, "gdiplus.lib")
#pragma comment (lib, "Shlwapi.lib")

__attribute__((unused))
static char* GetLastErrorMessage() {
	DWORD error = GetLastError();
	if (error != 0) {
		LPSTR message = NULL;
	    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL, error, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), (LPSTR)&message, 0, NULL);
	    return message;
	} else {
		static char const* err = "Error message not retrieved";
		size_t len = strlen(err);
		char* ret = (char*)malloc(len + 1);
		memset(ret, 0x0, len + 1);
		memcpy(ret, err, len + 1);
		return ret;
	}
}

#endif /* _LIBCORE_WINDOWS_H */
