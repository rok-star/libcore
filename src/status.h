#ifndef _LIBCORE_STATUS_H
#define _LIBCORE_STATUS_H

#define _STATUS_MAX_MESSAGE 255

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

#endif /* _LIBCORE_STATUS_H */
