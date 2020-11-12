#ifndef _LIBCORE_CONTEXT_H
#define _LIBCORE_CONTEXT_H

#include <libcore/rect.h>
#include <libcore/color.h>
#include <libcore/window.h>
#include <libcore/texture.h>

typedef struct _Context _Context;

#ifdef __cplusplus
extern "C" {
#endif

_Context* _Context_create_window(_Window*);
_Context* _Context_create_texture(_Window*);
void _Context_begin_paint(_Context*);
void _Context_end_paint(_Context*);
void _Context_set_origin(_Context*,int);
void _Context_set_clip(_Context*,_RectF*);
void _Context_fill_rect_color(_Context*,_RectF*,_Color*);
void _Context_fill_rect_texture(_Context*,_RectF*,_Texture*);
void _Context_frame_rect_color(_Context*,_RectF*,_Color*,float);
void _Context_draw_texture(_Context*,_Texture*,_Rect*,_Rect*,_Color*);
void _Context_destroy(_Context*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_CONTEXT_H */
