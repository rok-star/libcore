#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <libcore/thread.h>
#include <libcore/MACRO.h>

#define pthread_threadid_np_E(a, b) { \
	int __err = pthread_threadid_np(a, b); \
	if (__err != 0)  \
		_ABORT("pthread_threadid_np() failed: %s\n", strerror(__err)); \
}

#define pthread_create_E(a, b, c, d) { \
	int __err = pthread_create(a, b, c, d); \
	if (__err != 0) \
		_ABORT("pthread_create() failed: %s\n", strerror(__err)); \
}

#define pthread_cancel_E(a) { \
	int __err = pthread_cancel(a); \
	if (__err != 0) \
		_ABORT("pthread_cancel() failed: %s\n", strerror(__err)); \
}

#define pthread_join_E(a, b) { \
	int __err = pthread_join(a, b); \
	if (__err != 0) \
		_ABORT("pthread_join() failed: %s\n", strerror(__err)); \
}

typedef struct _thread_t {
	void(*proc)(void*);
	void* param;
	_Atomic uint64_t id;
	pthread_t thread;
} _thread_t;

void* __thread_proc(void* param) {
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
	uint64_t id = 0;
	pthread_create_E(&thread->thread, NULL, __thread_proc, thread);
	pthread_threadid_np_E(thread->thread, &id);
	thread->id = id;
	return thread;
}

void _thread_destroy(_thread_t* thread) {
	_ASSERT(thread != NULL);
	if (thread->id != 0)
		pthread_cancel_E(thread->thread);
	_FREE(thread);
}

void _thread_join(_thread_t* thread) {
	_ASSERT(thread != NULL);
	pthread_join_E(thread->thread, NULL);
	thread->id = 0;
}

uint64_t _thread_id(_thread_t const* thread) {
	_ASSERT(thread != NULL);
	return thread->id;
}