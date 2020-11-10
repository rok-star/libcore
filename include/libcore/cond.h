#ifndef _LIBCORE_COND_H
#define _LIBCORE_COND_H

#include <libcore/lock.h>

typedef struct _Cond _Cond;

_Cond* _Cond_create(void);
void _Cond_destroy(_Cond*);
void _Cond_notify(_Cond*);
void _Cond_notify_all(_Cond*);
void _Cond_wait(_Cond*, _Lock*);
void _Cond_wait_timeout(_Cond*, _Lock*, double);

#endif /* _LIBCORE_COND_H */
