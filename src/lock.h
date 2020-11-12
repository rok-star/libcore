#ifndef _LIBCORE_LOCK_H
#define _LIBCORE_LOCK_H

typedef struct _Lock _Lock;

#ifdef __cplusplus
extern "C" {
#endif

_Lock* _Lock_create(void);
void _Lock_destroy(_Lock*);
void _Lock_acquire(_Lock*);
void _Lock_release(_Lock*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_LOCK_H */
