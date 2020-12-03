#ifndef _LIBCORE_BRUSH_H
#define _LIBCORE_BRUSH_H

#include <libcore/color.h>
#include <libcore/texture.h>

typedef enum {
	_COLOR_BRUSH_TYPE,
	_TEXTURE_BRUSH_TYPE
} _BRUSH_TYPE;

typedef struct _Brush _Brush;

#ifdef __cplusplus
extern "C" {
#endif

_Brush* _Brush_create_color(_Color const*);
_Brush* _Brush_create_texture(_Texture const*);
_BRUSH_TYPE _Brush_type(_Brush const*);
_Color const* _Brush_color(_Brush const*);
_Texture const* _Brush_texture(_Brush const*);
void _Brush_destroy(_Brush*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_BRUSH_H */
