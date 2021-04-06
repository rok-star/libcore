#ifndef _LIBCORE_NET_H
#define _LIBCORE_NET_H

#include <stdint.h>
#include <libcore/status.h>

typedef struct _listen_info_t {
	uint16_t port;
} _listen_info_t;

typedef struct _listen_t _listen_t;
typedef struct _socket_t _socket_t;
typedef struct _poll_t _poll_t;

typedef enum {
    _ACCEPT_LISTEN_EVENT   = 0,
    _INTERUPT_LISTEN_EVENT = 1,
    _ERROR_LISTEN_EVENT    = 2
} _LISTEN_EVENT;

typedef enum {
    _READ_SOCKET_EVENT     = 0,
    _WRITE_SOCKET_EVENT    = 1,
    _CLOSE_SOCKET_EVENT    = 2,
    _INTERUPT_SOCKET_EVENT = 3,
    _ERROR_SOCKET_EVENT    = 4
} _SOCKET_EVENT;

static char const* _LISTEN_EVENT_NAME[3] = { "ACCEPT", "INTERUPT", "ERROR" };
static char const* _SOCKET_EVENT_NAME[5] = { "READ", "WRITE", "CLOSE", "INTERUPT", "ERROR" };

typedef struct _listen_event_t {
	_LISTEN_EVENT type;
    char* message;
} _listen_event_t;

typedef struct _socket_event_t {
	_SOCKET_EVENT type;
	_socket_t* socket;
    char* message;
} _socket_event_t;

#ifdef __cplusplus
extern "C" {
#endif

_listen_t* _listen_create(_listen_info_t*,_status_t*);
void _listen_destroy(_listen_t*);
void _listen_process(_listen_t*);
void _listen_process_timeout(_listen_t*,double);
void _listen_on_event(_listen_t*,void(*)(_listen_event_t const*,void*),void*);

_socket_t* _socket_accept(_listen_t*,_status_t*);
void _socket_destroy(_socket_t*);
void _socket_write(_socket_t*,uint8_t*,int64_t);
void _socket_read(_socket_t*,uint8_t**,int64_t*);
void _socket_process(_socket_t*);
void _socket_process_timeout(_listen_t*,double);
void _socket_on_event(_socket_t*,void(*)(_socket_event_t const*,void*),void*);

_poll_t* _poll_create(_status_t*);
void _poll_destroy(_poll_t*);
void _poll_append(_poll_t*,_socket_t*);
void _poll_remove(_poll_t*,_socket_t*);
void _poll_process(_poll_t*);
void _poll_process_timeout(_poll_t*,double);
void _poll_on_event(_poll_t*,void(*)(_socket_event_t const*,void*),void*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_NET_H */
