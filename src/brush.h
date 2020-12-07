#ifndef _LIBCORE_BRUSH_H
#define _LIBCORE_BRUSH_H

#include <libcore/color.h>
#include <libcore/texture.h>

typedef enum {
	_COLOR_BRUSH_TYPE,
	_TEXTURE_BRUSH_TYPE
} _BRUSH_TYPE;

typedef struct _brush_t _brush_t;

#ifdef __cplusplus
extern "C" {
#endif

_brush_t* _brush_create_color(_color_t const*);
_brush_t* _brush_create_texture(_texture_t const*);
_BRUSH_TYPE _brush_type(_brush_t const*);
_color_t const* _brush_color(_brush_t const*);
_texture_t const* _brush_texture(_brush_t const*);
void _brush_destroy(_brush_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_BRUSH_H */
