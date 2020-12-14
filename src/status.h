#ifndef _LIBCORE_STATUS_H
#define _LIBCORE_STATUS_H

#define _STATUS_MAX_MESSAGE 255

typedef struct _status_t {
    int code;
    char message[_STATUS_MAX_MESSAGE];
} _status_t;

#ifdef __cplusplus
extern "C" {
#endif

void _status_print(_status_t const*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_STATUS_H */
