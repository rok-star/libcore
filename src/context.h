#ifndef _LIBCORE_CONTEXT_H
#define _LIBCORE_CONTEXT_H

#include <libcore/math.h>
#include <libcore/brush.h>
#include <libcore/bezierpath.h>

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
_Size _Context_size(_Context*);
_Rect _Context_clip(_Context*);
_CONTEXT_ORIGIN _Context_origin(_Context*);
float _Context_scale(_Context*);
void _Context_set_clip(_Context*,_Rect const*);
void _Context_set_origin(_Context*,_CONTEXT_ORIGIN);
void _Context_set_scale(_Context*,float);
void _Context_draw_vertices(_Context*,float const*,int,bool,_Brush const*);
void _Context_draw_texture(_Context*,_Texture const*,_RectF const*,_RectF const*,_Color const*);
void _Context_stroke_line(_Context*,_PointF const*,_PointF const*,double,_Brush const*);
void _Context_stroke_rect(_Context*,_RectF const*,double,_Brush const*);
void _Context_stroke_path(_Context*,_BezierPath const*,double,_Brush const*);
void _Context_stroke_ellipse(_Context*,_RectF const*,double,_Brush const*);
void _Context_fill_rect(_Context*,_RectF const*,_Brush const*);
void _Context_fill_path(_Context*,_BezierPath const*,_Brush const*);
void _Context_fill_ellipse(_Context*,_RectF const*,_Brush const*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_CONTEXT_H */
