#include <stdio.h>
#include <libcore/WINDOWS.h>
#include <libcore/MACRO.h>
#include <libcore/app.h>

unsigned long long __thread_id = 0;
void (*__on_event)(_app_event_t const*,void*) = NULL;
void* __param = NULL;
bool __running = false;

void _app_run() {
    assert(__running == false);
    __running = true;
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    __running = false;
}

void _app_exit(void) {
    assert(__running == true);
    PostQuitMessage(0);
}

void _app_wakeup(void) {
    assert(__running == true);
    PostThreadMessageA(__thread_id, 0, 0, 0);
}

bool _app_running(void) {
    return __running;
}

void _app_on_event(void (*on_event)(_app_event_t const*,void*), void* param) {
    assert(on_event != NULL);
    __on_event = on_event;
    __param = param;
}

__attribute__((constructor))
void _app_module_init(void) {
    __thread_id = GetCurrentThreadId();
}
