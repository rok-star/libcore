#ifdef __linux__
    #define _POSIX_C_SOURCE 200809L
#endif

#include <stdint.h>
#include <assert.h>
#include <libcore/core/signal.h>
#include <libcore/core/MACRO.h>

#define sigaction_E(a, b, c) { \
	int __err = sigaction(a, b, c); \
    if (__err != 0) \
        _ABORT("sigaction() failed: %s\n", strerror(__err)); \
}

#define __CLEAR_EVENTS(array) { \
	for (int64_t i = 0; i < array.size; i++) { \
		_FREE(array.data[i].message); \
	} \
	array.size = 0; \
}

typedef struct _signal_t {
    struct {
    	int* data;
    	int size;
    	int capacity;
    } signums;
	struct {
		_signal_event_t* data;
		int64_t size;
		int64_t capacity;
	} events;
} _signal_t;

static void* __map[256] = {};

static void __handler(int signum) {
	assert(signum > 0);
	assert(signum < 256);

	_signal_t* signal = __map[signum];

	if (signal != NULL) {
		_PUSH(signal->signums, signum);
	}
}

_signal_t* _signal_create(int* signums, int num) {
	assert(signums != NULL);
	assert(num > 0);

	_signal_t* signal = _NEW(_signal_t, {});

	for (int i = 0; i < num; i++) {
		int signum = signums[i];
		assert(signum > 0);
		assert(signum < 256);
		__map[signum] = signal;
		sigaction_E(
			signum,
			&(struct sigaction){
				.sa_handler = __handler
			},
			NULL
		);
	}

	return signal;
}

void _signal_destroy(_signal_t* signal) {
	assert(signal != NULL);
	__CLEAR_EVENTS(signal->events);
	_FREE(signal->events.data);
	_FREE(signal->signums.data);
	_FREE(signal);
}

void _signal_process(_signal_t* signal, _signal_event_t const** events, int64_t* num) {
	assert(signal != NULL);

	(*events) = NULL;
	(*num) = 0;

	__CLEAR_EVENTS(signal->events);

	while (signal->signums.size > 0) {
		int signum = _SHIFT(signal->signums);
		_PUSH(signal->events, (_signal_event_t){
			.type = _SIGNAL_SIGNAL_EVENT,
			.signum = signum
		});
	}

	(*events) = signal->events.data;
	(*num) = signal->events.size;
}