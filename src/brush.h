#ifndef _LIBCORE_BRUSH_H
#define _LIBCORE_BRUSH_H

#include <libcore/color.h>
#include <libcore/texture.h>

typedef enum {
	_SOLID_BRUSH_TYPE,
	_TEXTURE_BRUSH_TYPE,
	_GRADIENT_BRUSH_TYPE
} _BRUSH_TYPE;

typedef struct _BrushOptions {
	_Color* color;
	_Texture* texture;
	_Color** colors;
} _BrushOptions;

typedef struct _Brush _Brush;

#ifdef __cplusplus
extern "C" {
#endif

_Brush* _Brush_create(_BRUSH_TYPE,_BrushOptions*);
void _Brush_destroy(_Brush*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_BRUSH_H */
