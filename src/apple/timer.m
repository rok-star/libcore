#include <Cocoa/Cocoa.h>
#include <libcore/MACRO.h>
#include <libcore/timer.h>

typedef struct _timer_t {
    void(*proc)(void*);
    void* param;
    NSTimer* timer;
} _timer_t;

_timer_t* _timer_create(double ms, void(*proc)(void*), void* param) {
    _ASSERT(ms > 0);
    _ASSERT(proc != NULL);
    _timer_t* timer = _NEW(_timer_t, {
        .proc = proc,
        .param = param
    });
    timer->timer = [NSTimer scheduledTimerWithTimeInterval: (ms / 1000.0L) repeats: YES block: ^(NSTimer* timer_){
        timer->proc(timer->param);
    }];
    return timer;
}

void _timer_destroy(_timer_t* timer) {
    _ASSERT(timer != NULL);
    [timer->timer invalidate];
    timer->timer = nil;
    _FREE(timer);
}