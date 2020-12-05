#include <stdio.h>
#include <stdlib.h>
#include <libcore/MACRO.h>
#include <libcore/brush.h>

typedef struct _Brush {
	_BRUSH_TYPE type;
	_Color const* color;
	_Texture const* texture;
} _Brush;

_Brush* _Brush_create_color(_Color const* color) {
	_ASSERT(color != NULL);
	return _NEW(_Brush, {
		.type = _COLOR_BRUSH_TYPE,
		.color = color
	});
}

_Brush* _Brush_create_texture(_Texture const* texture) {
	_ASSERT(texture != NULL);
	return _NEW(_Brush, {
		.type = _TEXTURE_BRUSH_TYPE,
		.texture = texture
	});
}

_BRUSH_TYPE _Brush_type(_Brush const* brush) {
	_ASSERT(brush != NULL);
	return brush->type;
}

_Color const* _Brush_color(_Brush const* brush) {
	_ASSERT(brush != NULL);
	return brush->color;
}

_Texture const* _Brush_texture(_Brush const* brush) {
	_ASSERT(brush != NULL);
	return brush->texture;
}

void _Brush_destroy(_Brush* brush) {
	_ASSERT(brush != NULL);
	_FREE(brush);
}
