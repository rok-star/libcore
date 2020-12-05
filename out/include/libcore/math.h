#ifndef _LIBCORE_MATH_H
#define _LIBCORE_MATH_H

#define _LERP(a, b, t) (a + (b - a) * t)

typedef struct _Point {
	int x;
	int y;
} _Point;

typedef struct _PointF {
	double x;
	double y;
} _PointF;

#define _POINT_I(a) ((_Point){ (a).width, (a).height })
#define _POINT_F(a) ((_PointF){ (a).width, (a).height })
#define _POINT_ADD(a, b) ((__typeof__(a)){ ((a).x + (b).x), ((a).y + (b).y) })
#define _POINT_MULT(a, b) ((__typeof__(a)){ ((a).x * (b)), ((a).y * (b)) })
#define _POINT_DIV(a, b) ((__typeof__(a)){ ((a).x / (b)), ((a).y / (b)) })
#define _POINT_LERP(a, b, t) ((__typeof__(a)){ _LERP((a).x, (b).x, t), _LERP((a).y, (b).y, t) });

typedef struct _Size {
	int width;
	int height;
} _Size;

typedef struct _SizeF {
	double width;
	double height;
} _SizeF;

#define _SIZE_I(a) ((_Size){ (a).width, (a).height })
#define _SIZE_F(a) ((_SizeF){ (a).width, (a).height })
#define _SIZE_ADD(a, b) ((__typeof__(a)){ ((a).width + (b).width), ((a).height + (b).height) })
#define _SIZE_MULT(a, b) ((__typeof__(a)){ ((a).width * (b)), ((a).height * (b)) })
#define _SIZE_DIV(a, b) ((__typeof__(a)){ ((a).width / (b)), ((a).height / (b)) })

typedef struct _Rect {
	_Point origin;
	_Size size;
} _Rect;

typedef struct _RectF {
	_PointF origin;
	_SizeF size;
} _RectF;

#define _RECT_I(a) ((_Rect){ _POINT_I((a).origin), _SIZE_I((a).size) })
#define _RECT_F(a) ((_RectF){ _POINT_F((a).origin), _SIZE_F((a).size) })
#define _RECT_MAX_X(a) ((a).origin.x + (a).size.width)
#define _RECT_MAX_Y(a) ((a).origin.y + (a).size.height)

_PointF _cubic_point(_PointF const*,_PointF const*,_PointF const*,_PointF const*,double);
void _cubic_points(_PointF const*,_PointF const*,_PointF const*,_PointF const*,double,_PointF**,int*);

_PointF _quad_point(_PointF const*,_PointF const*,_PointF const*,double);
void _quad_points(_PointF const*,_PointF const*,_PointF const*,double,_PointF**,int*);

_PointF _conic_point(_PointF const*,_PointF const*,_PointF const*,double,double);
void _conic_points(_PointF const*,_PointF const*,_PointF const*,double,double,_PointF**,int*);

#endif /* _LIBCORE_MATH_H */
