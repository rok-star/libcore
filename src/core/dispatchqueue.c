#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <libcore/core/MACRO.h>
#include <libcore/core/cond.h>
#include <libcore/core/lock.h>
#include <libcore/core/dispatchqueue.h>

typedef struct _dispatch_queue_item_t {
    void (*proc)(void*);
    void* param;
} _dispatch_queue_item_t;

typedef struct _dispatch_queue_t {
    struct {
        _dispatch_queue_item_t* data;
        int size;
        int capacity;
    } items;
    _lock_t* lock;
    _cond_t* cond;
    void(*wakeup)();
} _dispatch_queue_t;

static void __dispatch_queue_process(_dispatch_queue_t* queue, double timeout) {
    assert(queue != NULL);
    _lock_acquire(queue->lock);
    if (queue->items.size == 0) {
        if (timeout < 0) {
            _cond_wait(queue->cond, queue->lock);
        } else if (timeout > 0) {
            _cond_wait_timeout(queue->cond, queue->lock, timeout);
        }
    }
    for (;;) {
        if (queue->items.size == 0) break;
        else {
            _dispatch_queue_item_t item = _SHIFT(queue->items);
            _lock_release(queue->lock);
            item.proc(item.param);
            _lock_acquire(queue->lock);
        }
    }
    _lock_release(queue->lock);
}

_dispatch_queue_t* _dispatch_queue_create(void) {
    return _NEW(_dispatch_queue_t, {
        .lock = _lock_create(),
        .cond = _cond_create()
    });
}

void _dispatch_queue_destroy(_dispatch_queue_t* queue) {
    assert(queue != NULL);
    _lock_destroy(queue->lock);
    _cond_destroy(queue->cond);
    _FREE(queue->items.data);
    _FREE(queue);
}

void _dispatch_queue_post(_dispatch_queue_t* queue, void(*proc)(void*), void* param) {
    assert(queue != NULL);
    assert(proc != NULL);
    _lock_acquire(queue->lock);
    _PUSH(queue->items, (_dispatch_queue_item_t){ proc, param });
    _lock_release(queue->lock);
    if (queue->wakeup != NULL)
        queue->wakeup();
}

bool _dispatch_queue_remove(_dispatch_queue_t* queue, void(*proc)(void*), void* param) {
    assert(queue != NULL);
    assert(proc != NULL);
    bool ret = false;
    _lock_acquire(queue->lock);
    for (int i = (queue->items.size - 1); i >= 0; i--) {
        if ((queue->items.data[i].proc == proc)
        && (queue->items.data[i].param == param)) {
            _REMOVE_INDEX(queue->items, i);
            ret = true;
        }
    }
    _lock_release(queue->lock);
    return ret;
}

bool _dispatch_queue_remove_proc(_dispatch_queue_t* queue, void(*proc)(void*)) {
    assert(queue != NULL);
    _lock_acquire(queue->lock);
    for (int i = (queue->items.size - 1); i >= 0; i--) {
        if (queue->items.data[i].proc == proc) {
            _REMOVE_INDEX(queue->items, i);
            _lock_release(queue->lock);
            return true;
        }
    }
    _lock_release(queue->lock);
    return false;
}

bool _dispatch_queue_remove_param(_dispatch_queue_t* queue, void* param) {
    assert(queue != NULL);
    _lock_acquire(queue->lock);
    for (int i = (queue->items.size - 1); i >= 0; i--) {
        if (queue->items.data[i].param == param) {
            _REMOVE_INDEX(queue->items, i);
            _lock_release(queue->lock);
            return true;
        }
    }
    _lock_release(queue->lock);
    return false;
}

void _dispatch_queue_process(_dispatch_queue_t* queue) {
    assert(queue != NULL);
    __dispatch_queue_process(queue, -1);
}

void _dispatch_queue_process_timeout(_dispatch_queue_t* queue, double timeout) {
    assert(queue != NULL);
    assert(timeout >= 0);
    __dispatch_queue_process(queue, timeout);
}