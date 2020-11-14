#include <stdio.h>
#include <libcore/WINDOWS.h>
#include <libcore/MACRO.h>
#include <libcore/app.h>

typedef struct _App {
	int a;
} _App;

_App* _app_current = NULL;

_App* _App_create(void) {
	puts("_App_create");
	return _NEW(_App, { .a = 1 });
}

_App* _App_current(void) {
	return _app_current;
}

void _App_destroy(_App* app) {
	_ASSERT(app != NULL);
	_FREE(app);
}

void _App_run(_App* app) {
	_ASSERT(app != NULL);
	_app_current = app;
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	_app_current = NULL;
}

void _App_exit(_App* app) {
	_ASSERT(app != NULL);
	PostQuitMessage(0);
}
