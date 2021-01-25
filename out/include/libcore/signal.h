#ifndef _LIBCORE_SIGNAL_H
#define _LIBCORE_SIGNAL_H

typedef struct _signal_t _signal_t;

typedef enum : int {
    _SIGNAL_SIGNAL_EVENT = 0
} _SIGNAL_EVENT;

static char const* _SIGNAL_EVENT_NAME[1] = {
    "SIGNAL"
};

typedef struct _signal_event_t {
	_SIGNAL_EVENT type;
	int signum;
} _signal_event_t;

#ifdef __cplusplus
extern "C" {
#endif

_signal_t* _signal_create(int*,int);
void _signal_destroy(_signal_t*);
void _signal_process(_signal_t*);
void _signal_on_event(_signal_t*,void(*)(_signal_event_t const*,void*),void*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_SIGNAL_H */