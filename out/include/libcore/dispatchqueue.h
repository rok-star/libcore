#ifndef _LIBCORE_DISPATCH_QUEUE_H
#define _LIBCORE_DISPATCH_QUEUE_H

typedef struct _dispatch_queue_t _dispatch_queue_t;

#ifdef __cplusplus
extern "C" {
#endif

_dispatch_queue_t* _dispatch_queue_create(void);
void _dispatch_queue_destroy(_dispatch_queue_t*);
void _dispatch_queue_post(_dispatch_queue_t*,void(*)(void*),void*);
bool _dispatch_queue_remove(_dispatch_queue_t*,void(*)(void*),void*);
bool _dispatch_queue_remove_proc(_dispatch_queue_t*,void(*)(void*));
bool _dispatch_queue_remove_param(_dispatch_queue_t*,void*);
void _dispatch_queue_process(_dispatch_queue_t*);
void _dispatch_queue_process_timeout(_dispatch_queue_t*,double);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_DISPATCH_QUEUE_H */
