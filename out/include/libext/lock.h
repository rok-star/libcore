#ifndef _LIBEXT_LOCK_H
#define _LIBEXT_LOCK_H

typedef struct _lock_t _lock_t;

#ifdef __cplusplus
extern "C" {
#endif

_lock_t* _lock_create(void);
void _lock_destroy(_lock_t*);
void _lock_acquire(_lock_t*);
void _lock_release(_lock_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBEXT_LOCK_H */
