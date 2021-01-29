#ifndef _LIBEXT_SIGNAL_H
#define _LIBEXT_SIGNAL_H

#include <signal.h>

typedef struct _signal_t _signal_t;

typedef enum {
    _SIGNAL_SIGNAL_EVENT = 0,
    _ERROR_SIGNAL_EVENT  = 1
} _SIGNAL_EVENT;

static char const* _SIGNAL_EVENT_NAME[2] = { "SIGNAL", "ERROR" };

typedef struct _signal_event_t {
	_SIGNAL_EVENT type;
    char* message;
	int signum;
} _signal_event_t;

#ifdef __cplusplus
extern "C" {
#endif

_signal_t* _signal_create(int*,int);
void _signal_destroy(_signal_t*);
void _signal_process(_signal_t*,_signal_event_t const**,int64_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBEXT_SIGNAL_H */