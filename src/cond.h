#ifndef _LIBEXT_COND_H
#define _LIBEXT_COND_H

#include <libext/lock.h>

typedef struct _cond_t _cond_t;

#ifdef __cplusplus
extern "C" {
#endif

_cond_t* _cond_create(void);
void _cond_destroy(_cond_t*);
void _cond_notify(_cond_t*);
void _cond_notify_all(_cond_t*);
void _cond_wait(_cond_t*, _lock_t*);
void _cond_wait_timeout(_cond_t*, _lock_t*, double);

#ifdef __cplusplus
}
#endif

#endif /* _LIBEXT_COND_H */
