#ifndef _LIBCORE_NET_H
#define _LIBCORE_NET_H

#include <libcore/status.h>

typedef struct _connection_t _connection_t;
typedef struct _listener_t _listener_t;

typedef struct _listener_info_t {
	uint16_t port;
} _listener_info_t;

typedef enum : int {
    _DATA_CONNECTION_EVENT = 0
} _CONNECTION_EVENT;

typedef enum : int {
    _CONNECTION_LISTENER_EVENT = 0
} _LISTENER_EVENT;

static char const* _CONNECTION_EVENT_NAME[1] = { "DATA" };
static char const* _LISTENER_EVENT_NAME[1] = { "CONNECTION" };

typedef struct _connection_event_t {
	_CONNECTION_EVENT type;
} _connection_event_t;

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

void _connection_process(_connection_t*,int64_t);
void _connection_close(_connection_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_NET_H */
