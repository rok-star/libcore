#ifndef _LIBCORE_HTTP_H
#define _LIBCORE_HTTP_H

#include <stdint.h>
#include <libcore/status.h>

typedef enum {
    _REQUEST_HTTP_MESSAGE_TYPE  = 0,
    _RESPONSE_HTTP_MESSAGE_TYPE = 1
} _HTTP_MESSAGE_TYPE;

typedef struct _http_value_t {
    char const* data;
    int64_t size;
} _http_value_t;

typedef struct _http_header_t {
    _http_value_t name;
    _http_value_t value;
} _http_header_t;

typedef struct _http_message_t {
    _HTTP_MESSAGE_TYPE type;
    _http_value_t version;
    _http_value_t method;
    _http_value_t url;
    _http_value_t message;
    int status;
    struct  {
        _http_header_t* data;
        int64_t size;
        int64_t capacity;
    } headers;
    _http_value_t body;
} _http_message_t;

#ifdef __cplusplus
extern "C" {
#endif

_http_message_t* _http_message_create(char const*,int64_t,_HTTP_MESSAGE_TYPE,_status_t*);
void _http_message_destroy(_http_message_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_HTTP_H */
