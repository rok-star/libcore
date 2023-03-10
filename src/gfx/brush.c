#include <assert.h>
#include <libcore/core/MACRO.h>
#include <libcore/gfx/brush.h>

typedef struct _brush_t {
    _BRUSH_TYPE type;
    _color_t const* color;
    _texture_t const* texture;
} _brush_t;

_brush_t* _brush_create_color(_color_t const* color) {
    assert(color != NULL);
    return _NEW(_brush_t, {
        .type = _COLOR_BRUSH_TYPE,
        .color = color
    });
}

_brush_t* _brush_create_texture(_texture_t const* texture) {
    assert(texture != NULL);
    return _NEW(_brush_t, {
        .type = _TEXTURE_BRUSH_TYPE,
        .texture = texture
    });
}

_BRUSH_TYPE _brush_type(_brush_t const* brush) {
    assert(brush != NULL);
    return brush->type;
}

_color_t const* _brush_color(_brush_t const* brush) {
    assert(brush != NULL);
    return brush->color;
}

_texture_t const* _brush_texture(_brush_t const* brush) {
    assert(brush != NULL);
    return brush->texture;
}

void _brush_destroy(_brush_t* brush) {
    assert(brush != NULL);
    _FREE(brush);
}
