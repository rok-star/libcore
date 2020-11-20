#include <stdio.h>
#include <libcore/WINDOWS.h>
#include <libcore/MACRO.h>
#include <libcore/app.h>

unsigned long long __thread_id = 0;
void (*__on_event)(_AppEvent const*,void*) = NULL;
void* __param = NULL;
bool __running = false;

void _App_run() {
	_ASSERT(__running == false);
	__running = true;
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	__running = false;
}

void _App_exit(void) {
	_ASSERT(__running == true);
	PostQuitMessage(0);
}

void _App_wakeup(void) {
	_ASSERT(__running == true);
	PostThreadMessageA(__thread_id, 0, 0, 0);
}

bool _App_running(void) {
	return __running;
}

void _App_on_event(void (*on_event)(_AppEvent const*,void*), void* param) {
	_ASSERT(on_event != NULL);
	__on_event = on_event;
	__param = param;
}

__attribute__((constructor))
void _App_module_init(void) {
	__thread_id = GetCurrentThreadId();
}
