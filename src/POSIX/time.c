#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <libcore/MACRO.h>
#include <libcore/time.h>

#define gettimeofday_E(a, b) { \
	int __err = gettimeofday(a, b); \
	if (__err != 0)  \
		_ABORT("gettimeofday() failed: %s\n", strerror(__err)); \
}

double _time(void) {
	struct timeval tv;
	gettimeofday_E(&tv, NULL);
	return (tv.tv_sec * 1000.0)
		+ (tv.tv_usec / 1000.0);
}

double _time_local(void) {
	struct timeval tv;
	struct tm utc;
	struct tm loc;
	gettimeofday_E(&tv, NULL);
	localtime_r(&tv.tv_sec, &loc);
	gmtime_r(&tv.tv_sec, &utc);
	return (tv.tv_sec * 1000.0)
		+ (tv.tv_usec / 1000.0)
		+ (((loc.tm_sec * 1000.0) + (loc.tm_min * 60000.0) + (loc.tm_hour * 3600000.0))
			- ((utc.tm_sec * 1000.0) + (utc.tm_min * 60000.0) + (utc.tm_hour * 3600000.0)));
}