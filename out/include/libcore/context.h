#ifndef _LIBCORE_CONTEXT_H
#define _LIBCORE_CONTEXT_H

#include <libcore/math.h>
#include <libcore/brush.h>
#include <libcore/line.h>
#include <libcore/bezierpath.h>
#include <libcore/texture.h>
#include <libcore/window.h>

typedef enum {
	_LEFTTOP_CONTEXT_ORIGIN,
	_LEFTBOTTOM_CONTEXT_ORIGIN
} _CONTEXT_ORIGIN;

typedef struct _Transform {
	double x;
	double y;
	double scale;
} _Transform;

typedef struct _Context _Context;

#ifdef __cplusplus
extern "C" {
#endif

_Context* _Context_create_texture(_Texture const*);
_Context* _Context_create_window(_Window const*);
void _Context_destroy(_Context*);
void _Context_begin_paint(_Context*);
void _Context_end_paint(_Context*);
_Size _Context_size(_Context const*);
_Rect _Context_clip(_Context const*);
_CONTEXT_ORIGIN _Context_origin(_Context const*);
void _Context_set_clip(_Context*,_Rect const*);
void _Context_set_origin(_Context*,_CONTEXT_ORIGIN);
void _Context_draw_vertices(_Context const*,float const*,int,bool,_Brush const*);
void _Context_draw_texture(_Context const*,_Texture const*,_RectF const*,_RectF const*,_Color const*);
void _Context_stroke_line(_Context const*,_PointF const*,_PointF const*,double,_LINE_CAP,_Brush const*,_Transform const*);
void _Context_stroke_rect(_Context const*,_RectF const*,double,_Brush const*,_Transform const*);
void _Context_stroke_path(_Context const*,_BezierPath const*,double,_Brush const*,_Transform const*);
void _Context_stroke_ellipse(_Context const*,_RectF const*,double,_Brush const*,_Transform const*);
void _Context_fill_rect(_Context const*,_RectF const*,_Brush const*,_Transform const*);
void _Context_fill_path(_Context const*,_BezierPath const*,_Brush const*,_Transform const*);
void _Context_fill_ellipse(_Context const*,_RectF const*,_Brush const*,_Transform const*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_CONTEXT_H */
