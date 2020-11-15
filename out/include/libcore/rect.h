#ifndef _LIBCORE_RECT_H
#define _LIBCORE_RECT_H

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

#endif /* _LIBCORE_RECT_H */
