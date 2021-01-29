#include <stdlib.h>
#include <libext/http.h>
#include <libext/MACRO.h>

_http_message_t* _http_message_create(char const* data, int64_t size, _status_t* status) {
    return _NEW(struct _http_message_t, {});
}

void _http_message_destroy(_http_message_t* http_message) {
    free((struct http_message_t*)http_message);
}
