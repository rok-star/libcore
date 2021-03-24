#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <execinfo.h>
#include <libcore/MACRO.h>
#include <libcore/thread.h>
#include <libcore/sleep.h>
#include <libcore/timer.h>

typedef struct _timer_t {
    _thread_t* thread;
    void(*proc)(void*);
    void* param;
    double ms;
    bool exit;
} _timer_t;

static void __thread_proc(void* param) {
    _ASSERT(param != NULL);
    _timer_t* timer = (_timer_t*)param;
    for (;;) {
        _sleep(timer->ms);
        timer->proc(timer->param);
        if (timer->exit)
            break;
    }
}

_timer_t* _timer_create(double ms, void(*proc)(void*), void* param) {
    _ASSERT(ms > 0);
    _ASSERT(proc != NULL);
    _timer_t* timer = _NEW(_timer_t, {});
    timer->proc = proc;
    timer->param = param;
    timer->ms = ms;
    timer->exit = false;
    timer->thread = _thread_create(__thread_proc, timer);
    return timer;
}

void _timer_destroy(_timer_t* timer) {
    _ASSERT(timer != NULL);
    timer->exit = true;
    _thread_join(timer->thread);
    _thread_destroy(timer->thread);
    _FREE(timer);
}
