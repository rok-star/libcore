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

#define _TO_SIZE_T(T, a) ({ \
	__typeof__(a) __aa = a; \
	(T){ __aa.width, __aa.height }; \
})

#define _TO_SIZE(a) _TO_SIZE_T(_Size, a)
#define _TO_SIZE_U(a) _TO_SIZE_T(_SizeU, a)
#define _TO_SIZE_F(a) _TO_SIZE_T(_SizeF, a)

#endif /* _LIBCORE_SIZE_H */
