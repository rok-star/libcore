#ifdef __linux__
    #define _POSIX_C_SOURCE 200809L
#endif

#include <stdint.h>
#include <libext/signal.h>
#include <libext/MACRO.h>

#define sigaction_E(a, b, c) { \
	int __err = sigaction(a, b, c); \
    if (__err != 0) \
        _ABORT("sigaction() failed: %s\n", strerror(__err)); \
}

typedef struct _signal_t {
	void (*proc)(_signal_event_t const*, void*);
    void* param;
    struct {
    	int* data;
    	int size;
    	int capacity;
    } signums;
} _signal_t;

static void* __map[256] = {};

static void __handler(int signum) {
	_ASSERT(signum > 0);
	_ASSERT(signum < 256);

	_signal_t* signal = __map[signum];

	if (signal != NULL) {
		_PUSH(signal->signums, signum);
	}
}

_signal_t* _signal_create(int* signums, int num) {
	_ASSERT(signums != NULL);
	_ASSERT(num > 0);

	_signal_t* signal = _NEW(_signal_t, {});

	for (int i = 0; i < num; i++) {
		int signum = signums[i];
		_ASSERT(signum > 0);
		_ASSERT(signum < 256);
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
	_ASSERT(signal != NULL);
}

void _signal_process(_signal_t* signal) {
	_ASSERT(signal != NULL);

	while (signal->signums.size > 0) {
		int signum = _SHIFT(signal->signums);
		if (signal->proc != NULL) {
			signal->proc(
				&(_signal_event_t){
					.type = _SIGNAL_SIGNAL_EVENT,
					.signum = signum
				},
				signal->param
			);
		}
	}
}

void _signal_on_event(_signal_t* signal, void(*proc)(_signal_event_t const*,void*), void* param) {
	_ASSERT(signal != NULL);

	signal->proc = proc;
	signal->param = param;
}