#ifndef _LIBCORE_MATH_H
#define _LIBCORE_MATH_H

#define _LERP(a, b, t) (a + (b - a) * t)

typedef struct _point_t {
	double x;
	double y;
} _point_t;

#define _POINT_ADD(a, b) ((_point_t){ ((a).x + (b).x), ((a).y + (b).y) })
#define _POINT_MULT(a, b) ((_point_t){ ((a).x * (b)), ((a).y * (b)) })
#define _POINT_DIV(a, b) ((_point_t){ ((a).x / (b)), ((a).y / (b)) })
#define _POINT_LERP(a, b, t) ((_point_t){ _LERP((a).x, (b).x, t), _LERP((a).y, (b).y, t) });

typedef struct _size_t {
	double width;
	double height;
} _size_t;

#define _SIZE_ADD(a, b) ((_size_t){ ((a).width + (b).width), ((a).height + (b).height) })
#define _SIZE_MULT(a, b) ((_size_t){ ((a).width * (b)), ((a).height * (b)) })
#define _SIZE_DIV(a, b) ((_size_t){ ((a).width / (b)), ((a).height / (b)) })

typedef struct _rect_t {
	_point_t origin;
	_size_t size;
} _rect_t;

#define _RECT_MAX_X(a) ((a).origin.x + (a).size.width)
#define _RECT_MAX_Y(a) ((a).origin.y + (a).size.height)

#ifdef __cplusplus
extern "C" {
#endif

_point_t _cubic_point(_point_t const*,_point_t const*,_point_t const*,_point_t const*,double);
void _cubic_points(_point_t const*,_point_t const*,_point_t const*,_point_t const*,double,_point_t**,int*);

_point_t _quad_point(_point_t const*,_point_t const*,_point_t const*,double);
void _quad_points(_point_t const*,_point_t const*,_point_t const*,double,_point_t**,int*);

_point_t _conic_point(_point_t const*,_point_t const*,_point_t const*,double,double);
void _conic_points(_point_t const*,_point_t const*,_point_t const*,double,double,_point_t**,int*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_MATH_H */
