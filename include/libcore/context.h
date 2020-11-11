#ifndef _LIBCORE_CONTEXT_H
#define _LIBCORE_CONTEXT_H

#include <libcore/type.h>
#include <libcore/window.h>
#include <libcore/texture.h>

typedef struct _Context _Context;

_Context* _Context_create_window(_Window*);
_Context* _Context_create_texture(_Window*);
void _Context_begin_paint(_Context*);
void _Context_end_paint(_Context*);
void _Context_set_origin(_Context*,int);
void _Context_set_clip(_Context*,_RectF*);
void _Context_destroy(_Context*);

#endif /* _LIBCORE_CONTEXT_H */
