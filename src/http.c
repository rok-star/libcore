#include <stdlib.h>
#include <libext/http.h>
#include <libext/string.h>
#include <libext/parser.h>
#include <libext/MACRO.h>

static char const* __METHODS[] = { "GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH", NULL };

static bool __read_space(_parser_t* parser, _status_t* status) {
    _ASSERT(parser != NULL);
    _ASSERT(status != NULL);

    if (_parser_read_exact(parser, " ", 1)) {
        return true;
    } else {
        _status_set(status, _FAILURE_STATUS_TYPE, "Unexpected data, should be whitespace");
        return false;
    }
}

static bool __read_return(_parser_t* parser, _status_t* status) {
    _ASSERT(parser != NULL);
    _ASSERT(status != NULL);

    if (_parser_read_exact(parser, "\r\n", 2)) {
        return true;
    } else {
        _status_set(status, _FAILURE_STATUS_TYPE, "Unexpected data, should be line ending");
        return false;
    }
}

static bool __read_version(_parser_t* parser, _http_value_t* value, _status_t* status) {
    _ASSERT(parser != NULL);
    _ASSERT(value != NULL);
    _ASSERT(status != NULL);

    if (_parser_read_string(parser, &value->data, &value->size)) {
        if ((value->size > 6)
        && (strncmp("HTTP/", value->data, 5) == 0)) {
            return true;
        } else {
            _STATUS_SET_FORMAT(status, _FAILURE_STATUS_TYPE, "Unsupported HTTP version \"%.*s\"", (int)value->size, value->data);
            return false;
        }
    } else {
        _status_set(status, _FAILURE_STATUS_TYPE, "Unable to parse HTTP version");
        return false;
    }
}

static bool __read_method(_parser_t* parser, _http_value_t* value, _status_t* status) {
    _ASSERT(parser != NULL);
    _ASSERT(value != NULL);
    _ASSERT(status != NULL);

    if (_parser_read_string(parser, &value->data, &value->size)) {
        int64_t i = 0;
        for (;;) {
            if (__METHODS[i] != NULL) {
                if (strncmp(__METHODS[i], value->data, value->size) == 0) {
                    return true;
                }
            } else {
                break;
            }
            i += 1;
        }
        _STATUS_SET_FORMAT(status, _FAILURE_STATUS_TYPE, "Unsupported HTTP method \"%.*s\"", (int)value->size, value->data);
        return false;
    } else {
        _status_set(status, _FAILURE_STATUS_TYPE, "Unable to parse HTTP method");
        return false;
    }
}

static bool __read_status(_parser_t* parser, int* value, _status_t* status) {
    _ASSERT(parser != NULL);
    _ASSERT(value != NULL);
    _ASSERT(status != NULL);

    if (_parser_read_int32(parser, false, value)) {
        return true;
    } else {
        _status_set(status, _FAILURE_STATUS_TYPE, "Unable to parse HTTP status");
        return false;
    }
}

static bool __read_message(_parser_t* parser, _http_value_t* value, _status_t* status) {
    _ASSERT(parser != NULL);
    _ASSERT(value != NULL);
    _ASSERT(status != NULL);

    if (_parser_read_string(parser, &value->data, &value->size)) {
        return true;
    } else {
        _status_set(status, _FAILURE_STATUS_TYPE, "Unable to parse HTTP url");
        return false;
    }
}

static bool __read_url(_parser_t* parser, _http_value_t* value, _status_t* status) {
    _ASSERT(parser != NULL);
    _ASSERT(value != NULL);
    _ASSERT(status != NULL);

    if (_parser_read_string(parser, &value->data, &value->size)) {
        if (value->data[0] == '/') {
            return true;
        } else {
            _STATUS_SET_FORMAT(status, _FAILURE_STATUS_TYPE, "Wrong HTTP method \"%.*s\"", (int)value->size, value->data);
            return false;
        }
    } else {
        _status_set(status, _FAILURE_STATUS_TYPE, "Unable to parse HTTP url");
        return false;
    }
}

static bool __read_request(_parser_t* parser, _http_message_t* http_message, _status_t* status) {
    _ASSERT(parser != NULL);
    _ASSERT(http_message != NULL);
    _ASSERT(status != NULL);

    return (__read_method(parser, &http_message->method, status)
        && __read_space(parser, status)
        && __read_url(parser, &http_message->url, status)
        && __read_space(parser, status)
        && __read_version(parser, &http_message->version, status));
}

static bool __read_response(_parser_t* parser, _http_message_t* http_message, _status_t* status) {
    _ASSERT(parser != NULL);
    _ASSERT(http_message != NULL);
    _ASSERT(status != NULL);

    return (__read_version(parser, &http_message->method, status)
        && __read_space(parser, status)
        && __read_status(parser, &http_message->status, status)
        && __read_space(parser, status)
        && __read_message(parser, &http_message->message, status));
}

static bool __read_first_line(_parser_t* parser, _http_message_t* http_message, _status_t* status) {
    _ASSERT(parser != NULL);
    _ASSERT(http_message != NULL);
    _ASSERT(status != NULL);

    if (http_message->type == _REQUEST_HTTP_MESSAGE_TYPE) {
        return __read_request(parser, http_message, status);
    } else if (http_message->type == _RESPONSE_HTTP_MESSAGE_TYPE) {
        return __read_response(parser, http_message, status);
    } else {
        _ABORT("Wrong code path");
        return false;
    }
}

static bool __read_header(_parser_t* parser, _http_header_t* header, _status_t* status) {
    _ASSERT(parser != NULL);
    _ASSERT(header != NULL);
    _ASSERT(status != NULL);

    if (_parser_read_until(parser, ": ", 2, &header->name.data, &header->name.size)) {
        if (_parser_seek(parser, 2)) {
            if (_parser_read_string(parser, &header->value.data, &header->value.size)) {
                return true;
            } else {
                _status_set(status, _FAILURE_STATUS_TYPE, "Unable to parse header value");
                return false;
            }
        } else {
            _status_set(status, _FAILURE_STATUS_TYPE, "Unexpected end of header data");
            return false;
        }
    } else {
        _status_set(status, _FAILURE_STATUS_TYPE, "Unable to parse header name");
        return false;
    }
}

static bool __read_headers(_parser_t* parser, _http_message_t* http_message, _status_t* status) {
    _ASSERT(parser != NULL);
    _ASSERT(http_message != NULL);
    _ASSERT(status != NULL);

    _http_header_t header = {};
    for (;;) {
        if (__read_header(parser, &header, status)) {
            _PUSH(http_message->headers, header);
        } else {
            break;
        }
        if (!_parser_read_exact(parser, "\r\n", 2)) {
            break;
        }
    }

    if (http_message->headers.size > 0) {
        return true;
    } else {
        _status_set(status, _FAILURE_STATUS_TYPE, "Unable to parse headers");
        return false;
    }
}

_http_message_t* _http_message_create(char const* data, int64_t size, _HTTP_MESSAGE_TYPE type, _status_t* status) {
    _ASSERT(data != NULL);
    _ASSERT(status != NULL);

    _http_message_t* http_message = _NEW(_http_message_t, { .type = type });

    _parser_t* parser = _parser_create(data, size);
    if (__read_first_line(parser, http_message, status)) {
        if (_parser_read_exact(parser, "\r\n", 2)) {
            if (__read_headers(parser, http_message, status)) {
                if (_parser_read_exact(parser, "\r\n\r\n", 4)) {
                    ;
                } else {
                    return http_message;
                }
            } else {
                _FREE(http_message);
                return NULL;
            }
        } else {
            _status_set(status, _FAILURE_STATUS_TYPE, "Unexpected end of message");
            _FREE(http_message);
            return NULL;
        }
    } else {
        _FREE(http_message);
        return NULL;
    }




    if (!_parser_end(parser)) {
        if (__read_first_line(parser, http_message, status)
        && __read_return(parser, status)
        && __read_headers(parser, http_message, status)
        && __read_return(parser, status)
        && __read_body(parser, http_message, status)) {
            _status_set(status, _SUCCESS_STATUS_TYPE, NULL);
        } else {

        }
    } else {
        _status_set(status, _FAILURE_STATUS_TYPE, "Empty http message");
    }

    return http_message;
}

void _http_message_destroy(_http_message_t* http_message) {
    _FREE(http_message->headers.data);
    _FREE(http_message);
}
