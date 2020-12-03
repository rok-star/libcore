#ifndef _LIBCORE_CONTEXT_H
#define _LIBCORE_CONTEXT_H

#include <libcore/math.h>
#include <libcore/brush.h>

typedef enum {
	_WINDOW_CONTEXT_TYPE,
	_TEXTURE_CONTEXT_TYPE
} _CONTEXT_TYPE;

typedef enum {
	_LEFTTOP_CONTEXT_ORIGIN,
	_LEFTBOTTOM_CONTEXT_ORIGIN
} _CONTEXT_ORIGIN;

extern _Point const* _PATH_SPLINE;

typedef struct _Context _Context;

#ifdef __cplusplus
extern "C" {
#endif

_Context* _Context_create(_CONTEXT_TYPE,void*);
void _Context_destroy(_Context*);
void _Context_begin_paint(_Context*);
void _Context_end_paint(_Context*);
void _Context_set_clip_path(_Context*,_PointF const**,int);
void _Context_set_clip_rect(_Context*,_Rect const*);
void _Context_set_origin(_Context*,_CONTEXT_ORIGIN);
void _Context_stroke_rect(_Context*,_RectF const*,double,_Brush const*);
void _Context_stroke_ellipse(_Context*,_RectF const*,double,_Brush const*);
void _Context_fill_path(_Context*,_PointF const**,int,_Brush const*);
void _Context_fill_rect(_Context*,_RectF const*,_Brush const*);
void _Context_fill_ellipse(_Context*,_RectF const*,_Brush const*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_CONTEXT_H */
