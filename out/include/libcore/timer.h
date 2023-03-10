#ifndef _LIBCORE_TIMER_H
#define _LIBCORE_TIMER_H

typedef struct _timer_t _timer_t;

#ifdef __cplusplus
extern "C" {
#endif

_timer_t* _timer_create(double,void(*)(void*),void*);
void _timer_destroy(_timer_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_TIMER_H */
