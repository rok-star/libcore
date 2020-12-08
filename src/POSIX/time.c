#include <sys/time.h>
#include <libcore/time.h>

double _time(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (double)(tv.tv_sec * 1000) + (double)(tv.tv_usec / 1000.0);
}