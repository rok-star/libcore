#ifndef _LIBCORE_SIZE_H
#define _LIBCORE_SIZE_H

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

#endif /* _LIBCORE_SIZE_H */
