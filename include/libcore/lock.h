#ifndef _LIBCORE_LOCK_H
#define _LIBCORE_LOCK_H

typedef struct _Lock _Lock;

_Lock* _Lock_create(void);
void _Lock_destroy(_Lock*);
void _Lock_acquire(_Lock*);
void _Lock_release(_Lock*);

#endif /* _LIBCORE_LOCK_H */
