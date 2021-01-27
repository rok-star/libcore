#ifndef _LIBEXT_DISPATCH_QUEUE_H
#define _LIBEXT_DISPATCH_QUEUE_H

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
void _dispatch_queue_runloop(_dispatch_queue_t*,bool*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBEXT_DISPATCH_QUEUE_H */
