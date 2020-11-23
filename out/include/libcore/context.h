#ifndef _LIBCORE_CONTEXT_H
#define _LIBCORE_CONTEXT_H

#include <libcore/rect.h>
#include <libcore/color.h>
#include <libcore/texture.h>

typedef enum {
	_WINDOW_CONTEXT,
	_TEXTURE_CONTEXT
} _CONTEXT;

typedef struct _Context _Context;

#ifdef __cplusplus
extern "C" {
#endif

_Context* _Context_create(_CONTEXT,void*);
void _Context_destroy(_Context*);
void _Context_begin_paint(_Context*);
void _Context_end_paint(_Context*);
void _Context_fill_rect(_Context*,_RectF*,_Color*);
void _Context_frame_rect(_Context*,_RectF*,_Color*,double);
void _Context_draw_texture(_Context*,_RectF*,_RectF*,_Texture*,_Color*);
void _Context_set_clip_rect(_Context*,_Rect*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_CONTEXT_H */
