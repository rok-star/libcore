#ifndef _LIBEXT_STATUS_H
#define _LIBEXT_STATUS_H

#define _STATUS_MAX_MESSAGE 255

#define _STATUS_SET_FORMAT(status, type, ...) { \
    char __message[_STATUS_MAX_MESSAGE] = {}; \
    snprintf(__message, _STATUS_MAX_MESSAGE, __VA_ARGS__); \
    _status_set(status, type, __message); \
}

typedef enum {
	_SUCCESS_STATUS_TYPE,
	_FAILURE_STATUS_TYPE
} _STATUS_TYPE;

typedef struct _status_t {
    _STATUS_TYPE type;
    char message[_STATUS_MAX_MESSAGE];
} _status_t;

#ifdef __cplusplus
extern "C" {
#endif

void _status_print(_status_t const*);
void _status_set(_status_t*,_STATUS_TYPE,char const*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBEXT_STATUS_H */
