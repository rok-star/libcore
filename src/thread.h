#ifndef _LIBCORE_THREAD_H
#define _LIBCORE_THREAD_H

typedef struct _Thread _Thread;

#ifdef __cplusplus
extern "C" {
#endif

_Thread* _Thread_create(void(*)(void*));
void _Thread_destroy(_Thread*);
void _Thread_cancel(_Thread*);
void _Thread_join(_Thread*);
uint64_t _Thread_id(_Thread*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_THREAD_H */
