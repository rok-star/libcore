#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <execinfo.h>
#include <libcore/core/MACRO.h>
#include <libcore/core/thread.h>
#include <libcore/core/sleep.h>
#include <libcore/ui/timer.h>

typedef struct _timer_t {
    _thread_t* thread;
    void(*proc)(void*);
    void* param;
    double ms;
    bool exit;
} _timer_t;

static void __thread_proc(void* param) {
    assert(param != NULL);
    _timer_t* timer = (_timer_t*)param;
    for (;;) {
        _sleep(timer->ms);
        timer->proc(timer->param);
        if (timer->exit)
            break;
    }
}

_timer_t* _timer_create(double ms, void(*proc)(void*), void* param) {
    assert(ms > 0);
    assert(proc != NULL);
    _timer_t* timer = _NEW(_timer_t, {});
    timer->proc = proc;
    timer->param = param;
    timer->ms = ms;
    timer->exit = false;
    timer->thread = _thread_create(__thread_proc, timer);
    return timer;
}

void _timer_destroy(_timer_t* timer) {
    assert(timer != NULL);
    timer->exit = true;
    _thread_join(timer->thread);
    _thread_destroy(timer->thread);
    _FREE(timer);
}
