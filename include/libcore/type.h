#ifndef _LIBCORE_TYPE_H
#define _LIBCORE_TYPE_H

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

#endif /* _LIBCORE_TYPE_H */
