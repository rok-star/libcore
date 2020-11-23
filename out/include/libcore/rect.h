#ifndef _LIBCORE_RECT_H
#define _LIBCORE_RECT_H

#include <libcore/point.h>
#include <libcore/size.h>

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

#define _RECT_MAX_X(a) ((a).origin.x + (a).size.width)
#define _RECT_MAX_Y(a) ((a).origin.y + (a).size.height)

#define _TO_RECT_T(T, a) ({ \
	__typeof__(a) __aa = a; \
	(T){ \
		_TO_POINT_T(__typeof__(__aa.origin), __aa.origin), \
		_TO_SIZE_T(__typeof__(__aa.size), __aa.size) \
	}; \
})

#define _TO_RECT(a) _TO_RECT_T(_Rect, a)
#define _TO_RECT_U(a) _TO_RECT_T(_RectU, a)
#define _TO_RECT_F(a) _TO_RECT_T(_RectF, a)

#endif /* _LIBCORE_RECT_H */
