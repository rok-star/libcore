#ifndef _LIBCORE_MATH_H
#define _LIBCORE_MATH_H

#define _LERP(a, b, t) (a + (b - a) * t)

typedef struct _point_t {
	int x;
	int y;
} _point_t;

typedef struct _pointf_t {
	double x;
	double y;
} _pointf_t;

#define _POINT_I(a) ((_point_t){ (a).width, (a).height })
#define _POINT_F(a) ((_pointf_t){ (a).width, (a).height })
#define _POINT_ADD(a, b) ((__typeof__(a)){ ((a).x + (b).x), ((a).y + (b).y) })
#define _POINT_MULT(a, b) ((__typeof__(a)){ ((a).x * (b)), ((a).y * (b)) })
#define _POINT_DIV(a, b) ((__typeof__(a)){ ((a).x / (b)), ((a).y / (b)) })
#define _POINT_LERP(a, b, t) ((__typeof__(a)){ _LERP((a).x, (b).x, t), _LERP((a).y, (b).y, t) });

typedef struct _size_t {
	int width;
	int height;
} _size_t;

typedef struct _sizef_t {
	double width;
	double height;
} _sizef_t;

#define _SIZE_I(a) ((_size_t){ (a).width, (a).height })
#define _SIZE_F(a) ((_sizef_t){ (a).width, (a).height })
#define _SIZE_ADD(a, b) ((__typeof__(a)){ ((a).width + (b).width), ((a).height + (b).height) })
#define _SIZE_MULT(a, b) ((__typeof__(a)){ ((a).width * (b)), ((a).height * (b)) })
#define _SIZE_DIV(a, b) ((__typeof__(a)){ ((a).width / (b)), ((a).height / (b)) })

typedef struct _rect_t {
	_point_t origin;
	_size_t size;
} _rect_t;

typedef struct _rectf_t {
	_pointf_t origin;
	_sizef_t size;
} _rectf_t;

#define _RECT_I(a) ((_rect_t){ _POINT_I((a).origin), _SIZE_I((a).size) })
#define _RECT_F(a) ((_rectf_t){ _POINT_F((a).origin), _SIZE_F((a).size) })
#define _RECT_MAX_X(a) ((a).origin.x + (a).size.width)
#define _RECT_MAX_Y(a) ((a).origin.y + (a).size.height)

_pointf_t _cubic_point(_pointf_t const*,_pointf_t const*,_pointf_t const*,_pointf_t const*,double);
void _cubic_points(_pointf_t const*,_pointf_t const*,_pointf_t const*,_pointf_t const*,double,_pointf_t**,int*);

_pointf_t _quad_point(_pointf_t const*,_pointf_t const*,_pointf_t const*,double);
void _quad_points(_pointf_t const*,_pointf_t const*,_pointf_t const*,double,_pointf_t**,int*);

_pointf_t _conic_point(_pointf_t const*,_pointf_t const*,_pointf_t const*,double,double);
void _conic_points(_pointf_t const*,_pointf_t const*,_pointf_t const*,double,double,_pointf_t**,int*);

#endif /* _LIBCORE_MATH_H */
