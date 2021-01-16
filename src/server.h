#ifndef _LIBCORE_SERVER_H
#define _LIBCORE_SERVER_H

#include <libcore/status.h>

typedef struct _server_t _server_t;

typedef struct _server_info_t {
	uint16_t port;
} _server_info_t;

typedef enum : int {
    _OPEN_SERVER_EVENT  = 0,
    _CLOSE_SERVER_EVENT = 1,
    _DATA_SERVER_EVENT  = 2,
} _SERVER_EVENT;

static char const* _SERVER_EVENT_NAME[15] = {
    "OPEN", "CLOSE", "DATA"
};

typedef struct _server_event_t {
	_SERVER_EVENT type;
	int connection;
	struct {
		uint8_t* data;
		int64_t	size;
	} data;
} _server_event_t;

#ifdef __cplusplus
extern "C" {
#endif

_server_t* _server_create(void);
void _server_destroy(_server_t*);
void _server_on_event(_server_t*,void(*)(_server_event_t const*,void*),void*);
bool _server_running(_server_t const*);
void _server_start(_server_t*,_server_info_t const*,_status_t*);
void _server_stop(_server_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_SERVER_H */
