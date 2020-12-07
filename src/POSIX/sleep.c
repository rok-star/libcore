#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <libcore/sleep.h>
#include <libcore/MACRO.h>

#define nanospeep_E(a, b) { \
	int __err = nanosleep(a, b); \
	if (__err != 0) \
		_ABORT("nanosleep() failed: %s\n", strerror(__err)); \
}

void _sleep(double ms) {
	struct timespec ts = {
		.tv_sec = (ms / 1000),
		.tv_nsec = fmod(ms, 1000.0) * 1000000L
	};
	nanospeep_E(&ts, NULL);
}