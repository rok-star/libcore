#include <stdbool.h>
#include <stdint.h>
#include <libcore/MACRO.h>
#include <libcore/cond.h>
#include <libcore/lock.h>
#include <libcore/dispatchqueue.h>

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

_dispatch_queue_t* _dispatch_queue_create(void) {
    return _NEW(_dispatch_queue_t, {
        .lock = _lock_create(),
        .cond = _cond_create()
    });
}

void _dispatch_queue_destroy(_dispatch_queue_t* queue) {
    _ASSERT(queue != NULL);
    _lock_destroy(queue->lock);
    _cond_destroy(queue->cond);
    _FREE(queue->items.data);
    _FREE(queue);
}

void _dispatch_queue_post(_dispatch_queue_t* queue, void(*proc)(void*), void* param) {
    _ASSERT(queue != NULL);
    _ASSERT(proc != NULL);
    _lock_acquire(queue->lock);
    _PUSH(queue->items, (_dispatch_queue_item_t){ proc, param });
    _lock_release(queue->lock);
    if (queue->wakeup != NULL)
        queue->wakeup();
}

bool _dispatch_queue_remove(_dispatch_queue_t* queue, void(*proc)(void*), void* param) {
    _ASSERT(queue != NULL);
    _ASSERT(proc != NULL);
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
    _ASSERT(queue != NULL);
    bool ret = false;
    _lock_acquire(queue->lock);
    for (int i = (queue->items.size - 1); i >= 0; i--) {
        if (queue->items.data[i].proc == proc) {
            _REMOVE_INDEX(queue->items, i);
            ret = true;
        }
    }
    _lock_release(queue->lock);
    return ret;
}

bool _dispatch_queue_remove_param(_dispatch_queue_t* queue, void* param) {
    _ASSERT(queue != NULL);
    bool ret = false;
    _lock_acquire(queue->lock);
    for (int i = (queue->items.size - 1); i >= 0; i--) {
        if (queue->items.data[i].param == param) {
            _REMOVE_INDEX(queue->items, i);
            ret = true;
        }
    }
    _lock_release(queue->lock);
    return ret;
}

void _dispatch_queue_process(_dispatch_queue_t* queue) {
    _ASSERT(queue != NULL);
    _lock_acquire(queue->lock);
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

void _dispatch_queue_runloop(_dispatch_queue_t* queue, bool* exit_) {
    _ASSERT(queue != NULL);
    _ASSERT(exit_ != NULL);
    _lock_acquire(queue->lock);
    for (;;) {
        if (exit_)
            break;
        if (queue->items.size > 0) {
            _dispatch_queue_item_t item = _SHIFT(queue->items);
            _lock_release(queue->lock);
            item.proc(item.param);
            _lock_acquire(queue->lock);
        } else {
            _cond_wait_timeout(queue->cond, queue->lock, 1);
        }
    }
    _lock_release(queue->lock);
}
