#include <libcore/MACRO.h>
#include <libcore/layer.h>

typedef struct _layer_t {
	_rect_t rect;
	_texture_t const* texture;
	_TEXTURE_LAYOUT texture_layout;
	double frame_width;
	_brush_t* frame_brush;
	_brush_t* background_brush;
	_layer_t* parent;
	struct {
		_layer_t** data;
		int size;
		int capacity;
	} children;
} _layer_t;

_layer_t* _layer_create(void) {
	return _NEW(_layer_t, {});
}

void _layer_destroy(_layer_t* layer) {
	_ASSERT(layer != NULL);
	if (layer->frame_brush != NULL) {
		_brush_destroy(layer->frame_brush);
	}
	if (layer->background_brush != NULL) {
		_brush_destroy(layer->background_brush);
	}
	_FREE(layer);
}

void _layer_set_origin(_layer_t* layer, _point_t const* origin) {
	_ASSERT(layer != NULL);
	_ASSERT(origin != NULL);
	layer->rect.origin = *origin;
}

void _layer_set_size(_layer_t* layer, _size_t const* size) {
	_ASSERT(layer != NULL);
	_ASSERT(size != NULL);
	layer->rect.size = *size;
}

void _layer_set_texture(_layer_t* layer, _texture_t const* texture) {
	_ASSERT(layer != NULL);
	_ASSERT(texture != NULL);
	layer->texture = texture;
}

void _layer_set_texture_layout(_layer_t* layer, _TEXTURE_LAYOUT layout) {
	_ASSERT(layer != NULL);
	layer->texture_layout = layout;
}

void _layer_set_frame_width(_layer_t* layer, double width) {
	_ASSERT(layer != NULL);
	layer->frame_width = width;
}

void _layer_set_frame_color(_layer_t* layer, _color_t const* color) {
	_ASSERT(layer != NULL);
	if (layer->frame_brush != NULL) {
		_brush_destroy(layer->frame_brush);
	}
	if (color != NULL) {
		layer->frame_brush = _brush_create_color(color);
	}
}

void _layer_set_background_color(_layer_t* layer, _color_t const* color) {
	_ASSERT(layer != NULL);
	if (layer->background_brush != NULL) {
		_brush_destroy(layer->background_brush);
	}
	if (color != NULL) {
		layer->background_brush = _brush_create_color(color);
	}
}

_point_t const* _layer_origin(_layer_t const* layer) {
	_ASSERT(layer != NULL);
	return &layer->rect.origin;
}

_size_t const* _layer_size(_layer_t const* layer) {
	_ASSERT(layer != NULL);
	return &layer->rect.size;
}

_texture_t const* _layer_texture(_layer_t const* layer) {
	_ASSERT(layer != NULL);
	return layer->texture;
}

double _layer_frame_width(_layer_t const* layer) {
	_ASSERT(layer != NULL);
	return layer->frame_width;
}

_color_t const* _layer_frame_color(_layer_t const* layer) {
	_ASSERT(layer != NULL);
	if (layer->frame_brush != NULL) {
		return _brush_color(layer->frame_brush);
	} else {
		return NULL;
	}
}

_color_t const* _layer_background_color(_layer_t const* layer) {
	_ASSERT(layer != NULL);
	if (layer->background_brush != NULL) {
		return _brush_color(layer->background_brush);
	} else {
		return NULL;
	}
}

_layer_t* _layer_parent(_layer_t const* layer) {
	_ASSERT(layer != NULL);
	return layer->parent;
}

_layer_t* _layer_children_item(_layer_t const* layer, int index) {
	_ASSERT(layer != NULL);
	_ASSERT((index >= 0) && (index < layer->children.size));
	return layer->children.data[index];
}

int _layer_children_count(_layer_t const* layer) {
	_ASSERT(layer != NULL);
	return layer->children.size;
}

void _layer_add_child(_layer_t* layer, _layer_t* child) {
	_ASSERT(layer != NULL);
	_ASSERT(child != NULL);
	_layer_remove_from_parent(child);
	_PUSH(layer->children, child);
}

bool _layer_remove_child(_layer_t* layer, _layer_t* child) {
	_ASSERT(layer != NULL);
	_ASSERT(child != NULL);
	int index = _INDEX_OF(layer->children, child);
	if (index > -1) {
		_REMOVE(layer->children, index);
		child->parent = NULL;
		return true;
	} else {
		return false;
	}
}

bool _layer_remove_from_parent(_layer_t* layer) {
	_ASSERT(layer != NULL);
	if (layer->parent != NULL) {
		int index = _INDEX_OF(layer->parent->children, layer);
		_REMOVE(layer->parent->children, index);
		layer->parent = NULL;
		return true;
	} else {
		return false;
	}
}

void _layer_paint(_layer_t const* layer, _context_t const* context, _point_t const* point) {
	_ASSERT(layer != NULL);
	_ASSERT(context != NULL);
	_rect_t rect = {
		.origin = _POINT_ADD(layer->rect.origin, ((point != NULL) ? *point : ((_point_t){ 0, 0 }))),
		.size = layer->rect.size
	};
	if (layer->background_brush != NULL) {
		_context_fill_rect(context, &rect, layer->background_brush, NULL);
	}
	if ((layer->frame_brush != NULL)
	&& (layer->frame_width > 0)) {
		_rect_t rect_ = rect;
		rect_.origin.x += (layer->frame_width / 2);
		rect_.origin.y += (layer->frame_width / 2);
		rect_.size.width -= layer->frame_width;
		rect_.size.height -= layer->frame_width;
		_context_stroke_rect(context, &rect_, layer->frame_width, layer->frame_brush, NULL);
	}
	if (layer->texture != NULL) {
		_size_t size = *_texture_size(layer->texture);

		#define __COVER() { \
			double __ratio = _MAX((rect.size.width / size.width), (rect.size.height / size.height)); \
			size.width *= __ratio; \
			size.height *= __ratio; \
		}
		#define __OVERFLOW() _point_t __overflow = { (size.width - rect.size.width), (size.height - rect.size.height) }
		#define __X_BEGIN() rect.origin.x
		#define __X_CENTER() (rect.origin.x + ((__overflow.x / 2.0) * -1.0))
		#define __X_END() (rect.origin.x + (__overflow.x * -1.0))
		#define __Y_BEGIN() rect.origin.y
		#define __Y_CENTER() (rect.origin.y + ((__overflow.y / 2.0) * -1.0))
		#define __Y_END() (rect.origin.y + (__overflow.y * -1.0))

		if (layer->texture_layout == _DEFAULT_TEXTURE_LAYOUT) {
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ rect.origin, size }, NULL);
		} else if (layer->texture_layout == _STRETCH_TEXTURE_LAYOUT) {
			_context_draw_texture(context, layer->texture, NULL, &rect, NULL);
		} else if (layer->texture_layout == _BEGIN_BEGIN_TEXTURE_LAYOUT) {
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_BEGIN(), __Y_BEGIN() }, size }, NULL);
		} else if (layer->texture_layout == _BEGIN_CENTER_TEXTURE_LAYOUT) {
			__OVERFLOW();
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_BEGIN(), __Y_CENTER() }, size }, NULL);
		} else if (layer->texture_layout == _BEGIN_END_TEXTURE_LAYOUT) {
			__OVERFLOW();
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_BEGIN(), __Y_END() }, size }, NULL);
		} else if (layer->texture_layout == _CENTER_BEGIN_TEXTURE_LAYOUT) {
			__OVERFLOW();
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_CENTER(), __Y_BEGIN() }, size }, NULL);
		} else if (layer->texture_layout == _CENTER_CENTER_TEXTURE_LAYOUT) {
			__OVERFLOW();
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_CENTER(), __Y_CENTER() }, size }, NULL);
		} else if (layer->texture_layout == _CENTER_END_TEXTURE_LAYOUT) {
			__OVERFLOW();
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_CENTER(), __Y_END() }, size }, NULL);
		} else if (layer->texture_layout == _END_BEGIN_TEXTURE_LAYOUT) {
			__OVERFLOW();
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_END(), __Y_BEGIN() }, size }, NULL);
		} else if (layer->texture_layout == _END_CENTER_TEXTURE_LAYOUT) {
			__OVERFLOW();
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_END(), __Y_CENTER() }, size }, NULL);
		} else if (layer->texture_layout == _END_END_TEXTURE_LAYOUT) {
			__OVERFLOW();
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_END(), __Y_END() }, size }, NULL);
		} else if (layer->texture_layout == _COVER_BEGIN_BEGIN_TEXTURE_LAYOUT) {
			__COVER();
			__OVERFLOW();
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_BEGIN(), __Y_BEGIN() }, size }, NULL);
		} else if (layer->texture_layout == _COVER_BEGIN_CENTER_TEXTURE_LAYOUT) {
			__COVER();
			__OVERFLOW();
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_BEGIN(), __Y_CENTER() }, size }, NULL);
		} else if (layer->texture_layout == _COVER_BEGIN_END_TEXTURE_LAYOUT) {
			__COVER();
			__OVERFLOW();
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_BEGIN(), __Y_END() }, size }, NULL);
		} else if (layer->texture_layout == _COVER_CENTER_BEGIN_TEXTURE_LAYOUT) {
			__COVER();
			__OVERFLOW();
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_CENTER(), __Y_BEGIN() }, size }, NULL);
		} else if (layer->texture_layout == _COVER_CENTER_CENTER_TEXTURE_LAYOUT) {
			__COVER();
			__OVERFLOW();
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_CENTER(), __Y_CENTER() }, size }, NULL);
		} else if (layer->texture_layout == _COVER_CENTER_END_TEXTURE_LAYOUT) {
			__COVER();
			__OVERFLOW();
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_CENTER(), __Y_END() }, size }, NULL);
		} else if (layer->texture_layout == _COVER_END_BEGIN_TEXTURE_LAYOUT) {
			__COVER();
			__OVERFLOW();
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_END(), __Y_BEGIN() }, size }, NULL);
		} else if (layer->texture_layout == _COVER_END_CENTER_TEXTURE_LAYOUT) {
			__COVER();
			__OVERFLOW();
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_END(), __Y_CENTER() }, size }, NULL);
		} else if (layer->texture_layout == _COVER_END_END_TEXTURE_LAYOUT) {
			__COVER();
			__OVERFLOW();
			_context_draw_texture(context, layer->texture, NULL, &(_rect_t){ { __X_END(), __Y_END() }, size }, NULL);
		}

		#undef __OVERFLOW
		#undef __X_BEGIN
		#undef __X_CENTER
		#undef __X_END
		#undef __Y_BEGIN
		#undef __Y_CENTER
		#undef __Y_END
	}
	for (int i = 0; i < layer->children.size; i++) {
		_layer_paint(layer->children.data[i], context, &rect.origin);
	}
}
