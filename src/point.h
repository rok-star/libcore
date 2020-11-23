#ifndef _LIBCORE_POINT_H
#define _LIBCORE_POINT_H

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

#define _TO_POINT_T(T, a) ({ \
	__typeof__(a) __aa = a; \
	(T){ __aa.width, __aa.height }; \
})

#define _TO_POINT(a) _TO_POINT_T(_Point, a)
#define _TO_POINT_U(a) _TO_POINT_T(_PointU, a)
#define _TO_POINT_F(a) _TO_POINT_T(_PointF, a)

#endif /* _LIBCORE_POINT_H */
