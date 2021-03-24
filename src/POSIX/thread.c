#ifdef __linux__
    #define _POSIX_C_SOURCE 200809L
#endif

#include <pthread.h>
#include <libcore/thread.h>
#include <libcore/MACRO.h>

typedef struct _thread_t {
    void(*proc)(void*);
    void* param;
    pthread_t thread;
} _thread_t;

static void* __thread_proc(void* param) {
    _ASSERT(param != NULL);
    _thread_t* thread = (_thread_t*)param;
    thread->proc(thread->param);
    return NULL;
}

_thread_t* _thread_create(void(*proc)(void*), void* param) {
    _ASSERT(proc != NULL);
    _thread_t* thread = _NEW(_thread_t, {
        .proc = proc,
        .param = param
    });
    _CALL(pthread_create, &thread->thread, NULL, __thread_proc, thread);
    return thread;
}

void _thread_destroy(_thread_t* thread) {
    _ASSERT(thread != NULL);
    _FREE(thread);
}

void _thread_join(_thread_t* thread) {
    _ASSERT(thread != NULL);
    _CALL(pthread_join, thread->thread, NULL);
}