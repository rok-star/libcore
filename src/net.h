#ifndef _LIBEXT_NET_H
#define _LIBEXT_NET_H

#include <stdint.h>
#include <libext/status.h>

typedef struct _listener_info_t {
	uint16_t port;
} _listener_info_t;

typedef struct _listener_t _listener_t;
typedef struct _connection_t _connection_t;
typedef struct _connection_poll_t _connection_poll_t;

typedef enum {
    _ACCEPT_LISTENER_EVENT   = 0,
    _INTERUPT_LISTENER_EVENT = 1,
    _ERROR_LISTENER_EVENT    = 2
} _LISTENER_EVENT;

typedef enum {
    _READ_CONNECTION_EVENT     = 0,
    _WRITE_CONNECTION_EVENT    = 1,
    _CLOSE_CONNECTION_EVENT    = 2,
    _INTERUPT_CONNECTION_EVENT = 3,
    _ERROR_CONNECTION_EVENT    = 4
} _CONNECTION_EVENT;

static char const* _LISTENER_EVENT_NAME[3] = { "ACCEPT", "INTERUPT", "ERROR" };
static char const* _CONNECTION_EVENT_NAME[5] = { "READ", "WRITE", "CLOSE", "INTERUPT", "ERROR" };

typedef struct _listener_event_t {
	_LISTENER_EVENT type;
    char* message;
} _listener_event_t;

typedef struct _connection_event_t {
	_CONNECTION_EVENT type;
	_connection_t* connection;
    char* message;
} _connection_event_t;

#ifdef __cplusplus
extern "C" {
#endif

_listener_t* _listener_create(_listener_info_t*,_status_t*);
void _listener_destroy(_listener_t*);
void _listener_process(_listener_t*,_listener_event_t const**,int64_t*);

_connection_t* _connection_accept(_listener_t*,_status_t*);
void _connection_destroy(_connection_t*);
void _connection_process(_connection_t*,_connection_event_t const**,int64_t*);
void _connection_write(_connection_t*,uint8_t*,int64_t);
void _connection_read(_connection_t*,uint8_t**,int64_t*);

_connection_poll_t* _connection_poll_create(_status_t*);
void _connection_poll_destroy(_connection_poll_t*);
void _connection_poll_process(_connection_poll_t*,_connection_event_t const**,int64_t*);
void _connection_poll_append(_connection_poll_t*,_connection_t*);
void _connection_poll_remove(_connection_poll_t*,_connection_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBEXT_NET_H */
