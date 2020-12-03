#ifndef _LIBCORE_MATH_H
#define _LIBCORE_MATH_H

#define _LERP(a, b, t) (a + (b - a) * t)

typedef struct _Point {
	int x;
	int y;
} _Point;

typedef struct _PointU {
	unsigned x;
	unsigned y;
} _PointU;

typedef struct _PointF {
	double x;
	double y;
} _PointF;

#define _POINT_I(a) ((_Point){ (a).width, (a).height })
#define _POINT_U(a) ((_PointU){ (a).width, (a).height })
#define _POINT_F(a) ((_PointF){ (a).width, (a).height })
#define _POINT_ADD(a, b) ((__typeof__(a)){ ((a).x + (b).x), ((a).y + (b).y) })
#define _POINT_MULT(a, b) ((__typeof__(a)){ ((a).x * (b)), ((a).y * (b)) })
#define _POINT_LERP(a, b, t) ((__typeof__(a)){ _LERP((a).x, (b).x, t), _LERP((a).y, (b).y, t) });
#define _POINT_FLATNESS(a, b, c, d) ({ \
	double __ux = ((3.0 * (b).x) - (2.0 * (a).x) - (d).x); __ux *= __ux; \
	double __uy = ((3.0 * (b).y) - (2.0 * (a).y) - (d).y); __uy *= __uy; \
	double __vx = ((3.0 * (c).x) - (2.0 * (d).x) - (a).x); __vx *= __vx; \
	double __vy = ((3.0 * (c).y) - (2.0 * (d).y) - (a).y); __vy *= __vy; \
	if (__ux < __vx) __ux = __vx; \
	if (__uy < __vy) __uy = __vy; \
	(__ux + __uy); \
})

typedef struct _Size {
	int width;
	int height;
} _Size;

typedef struct _SizeU {
	unsigned width;
	unsigned height;
} _SizeU;

typedef struct _SizeF {
	double width;
	double height;
} _SizeF;

#define _SIZE_I(a) ((_Size){ (a).width, (a).height })
#define _SIZE_U(a) ((_SizeU){ (a).width, (a).height })
#define _SIZE_F(a) ((_SizeF){ (a).width, (a).height })

typedef struct _Rect {
	_Point origin;
	_Size size;
} _Rect;

typedef struct _RectU {
	_PointU origin;
	_SizeU size;
} _RectU;

typedef struct _RectF {
	_PointF origin;
	_SizeF size;
} _RectF;

#define _RECT_I(a) ((_Rect){ _POINT_I((a).origin), _SIZE_I((a).size) })
#define _RECT_U(a) ((_RectU){ _POINT_U((a).origin), _SIZE_U((a).size) })
#define _RECT_F(a) ((_RectF){ _POINT_F((a).origin), _SIZE_F((a).size) })
#define _RECT_MAX_X(a) ((a).origin.x + (a).size.width)
#define _RECT_MAX_Y(a) ((a).origin.y + (a).size.height)

_PointF _bezier_point(_PointF const*,_PointF const*,_PointF const*,_PointF const*,double);
void _bezier_points(_PointF const*,_PointF const*,_PointF const*,_PointF const*,double,_PointF**,int*);

#endif /* _LIBCORE_MATH_H */
