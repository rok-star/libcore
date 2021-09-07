#include <assert.h>
#include <Cocoa/Cocoa.h>
#include <libcore/core/MACRO.h>
#include <libcore/ui/timer.h>

typedef struct _timer_t {
    void(*proc)(void*);
    void* param;
    NSTimer* timer;
} _timer_t;

_timer_t* _timer_create(double ms, void(*proc)(void*), void* param) {
    assert(ms > 0);
    assert(proc != NULL);
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
    assert(timer != NULL);
    [timer->timer invalidate];
    timer->timer = nil;
    _FREE(timer);
}