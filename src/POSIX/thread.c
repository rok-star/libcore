#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
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

typedef struct _Thread {
	void(*proc)(void*);
	void* param;
	_Atomic uint64_t id;
	pthread_t thread;
} _Thread;

void* __thread_proc(void* param) {
	_ASSERT(param != NULL);
	_Thread* thread = (_Thread*)param;
	thread->proc(thread->param);
	return NULL;
}

_Thread* _Thread_create(void(*proc)(void*), void* param) {
	_ASSERT(proc != NULL);
	_Thread* thread = _NEW(_Thread, {
		.proc = proc,
		.param = param
	});
	uint64_t id = 0;
	pthread_create_E(&thread->thread, NULL, __thread_proc, thread);
	pthread_threadid_np_E(thread->thread, &id);
	thread->id = id;
}

void _Thread_destroy(_Thread* thread) {
	_ASSERT(thread != NULL);
	pthread_cancel_E(thread->thread);
	_FREE(thread);
}

void _Thread_join(_Thread* thread) {
	_ASSERT(thread != NULL);
	pthread_join_E(thread->thread, NULL);
}

uint64_t _Thread_id(_Thread const* thread) {
	_ASSERT(thread != NULL);
	return thread->id;
}