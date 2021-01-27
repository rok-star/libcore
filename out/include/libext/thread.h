#ifndef _LIBEXT_THREAD_H
#define _LIBEXT_THREAD_H

#include <stdint.h>

typedef struct _thread_t _thread_t;

#ifdef __cplusplus
extern "C" {
#endif

_thread_t* _thread_create(void(*)(void*),void*);
void _thread_destroy(_thread_t*);
void _thread_join(_thread_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBEXT_THREAD_H */
