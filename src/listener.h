#ifndef _LIBCORE_LISTENER_H
#define _LIBCORE_LISTENER_H

#include <libcore/status.h>

typedef struct _listener_t _listener_t;

typedef struct _listener_info_t {
	uint16_t port;
} _listener_info_t;

typedef enum : int {
    _CONNECTION_LISTENER_EVENT = 0
} _LISTENER_EVENT;

static char const* _LISTENER_EVENT_NAME[1] = {
    "CONNECTION"
};

typedef struct _listener_event_t {
	_LISTENER_EVENT type;
} _listener_event_t;

#ifdef __cplusplus
extern "C" {
#endif

_listener_t* _listener_create(_listener_info_t const*,_status_t*);
void _listener_destroy(_listener_t*);
void _listener_process(_listener_t*);
void _listener_on_event(_listener_t*,void(*)(_listener_event_t const*,void*),void*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_LISTENER_H */
