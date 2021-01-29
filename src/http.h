#ifndef _LIBEXT_HTTP_H
#define _LIBEXT_HTTP_H

#include <stdint.h>
#include <libext/status.h>

typedef const struct _http_message_t {
    struct {
        struct {
            struct {
                char const* data;
                int64_t size;
            } name;
            struct {
                char const* data;
                int64_t size;
            } value;
        } *data;
        int64_t size;
        int64_t capacity;
    } headers;
    struct {
        char const* data;
        int64_t size;
    } body;
} _http_message_t;

#ifdef __cplusplus
extern "C" {
#endif

_http_message_t* _http_message_create(char const*,int64_t,_status_t*);
void _http_message_destroy(_http_message_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBEXT_HTTP_H */
