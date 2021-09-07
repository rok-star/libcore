#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <libcore/core/MACRO.h>
#include <libcore/gfx/layer.h>

typedef struct _layer_t {
	_rect_t rect;
	_point_t center;
	double opacity;
	_texture_t const* texture;
	_TEXTURE_LAYOUT texture_layout;
	double texture_scale;
	int stretch_indent_x;
	int stretch_indent_y;
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
	return _NEW(_layer_t, {
		.texture_scale = 1.0,
		.opacity = 1.0
	});
}

void _layer_destroy(_layer_t* layer) {
	assert(layer != NULL);
	if (layer->frame_brush != NULL) {
		_brush_destroy(layer->frame_brush);
	}
	if (layer->background_brush != NULL) {
		_brush_destroy(layer->background_brush);
	}
	_FREE(layer);
}

void _layer_set_center(_layer_t* layer, _point_t const* center) {
	assert(layer != NULL);
	assert(center != NULL);
	layer->center = *center;
	layer->rect.origin.x = (center->x - (layer->rect.size.width / 2.0));
	layer->rect.origin.y = (center->y - (layer->rect.size.height / 2.0));
}

void _layer_set_origin(_layer_t* layer, _point_t const* origin) {
	assert(layer != NULL);
	assert(origin != NULL);
	layer->rect.origin = *origin;
	layer->center.x = (layer->rect.origin.x + (layer->rect.size.width / 2.0));
	layer->center.y = (layer->rect.origin.y + (layer->rect.size.height / 2.0));
}

void _layer_set_size(_layer_t* layer, _size_t const* size) {
	assert(layer != NULL);
	assert(size != NULL);
	layer->rect.size = *size;
}

void _layer_set_texture(_layer_t* layer, _texture_t const* texture) {
	assert(layer != NULL);
	assert(texture != NULL);
	layer->texture = texture;
}

void _layer_set_texture_layout(_layer_t* layer, _TEXTURE_LAYOUT layout) {
	assert(layer != NULL);
	layer->texture_layout = layout;
}

void _layer_set_texture_scale(_layer_t* layer, double scale) {
	assert(layer != NULL);
	layer->texture_scale = scale;
}

void _layer_set_texture_stretch_indent(_layer_t* layer, int x, int y) {
	assert(layer != NULL);
	layer->stretch_indent_x = x;
	layer->stretch_indent_y = y;
}

void _layer_set_frame_width(_layer_t* layer, double width) {
	assert(layer != NULL);
	layer->frame_width = width;
}

void _layer_set_frame_color(_layer_t* layer, _color_t const* color) {
	assert(layer != NULL);
	if (layer->frame_brush != NULL) {
		_brush_destroy(layer->frame_brush);
	}
	if (color != NULL) {
		layer->frame_brush = _brush_create_color(color);
	}
}

void _layer_set_background_color(_layer_t* layer, _color_t const* color) {
	assert(layer != NULL);
	if (layer->background_brush != NULL) {
		_brush_destroy(layer->background_brush);
	}
	if (color != NULL) {
		layer->background_brush = _brush_create_color(color);
	}
}

_point_t const* _layer_center(_layer_t const* layer) {
	assert(layer != NULL);
	return &layer->center;
}

_point_t const* _layer_origin(_layer_t const* layer) {
	assert(layer != NULL);
	return &layer->rect.origin;
}

_size_t const* _layer_size(_layer_t const* layer) {
	assert(layer != NULL);
	return &layer->rect.size;
}

_texture_t const* _layer_texture(_layer_t const* layer) {
	assert(layer != NULL);
	return layer->texture;
}

double _layer_frame_width(_layer_t const* layer) {
	assert(layer != NULL);
	return layer->frame_width;
}

_color_t const* _layer_frame_color(_layer_t const* layer) {
	assert(layer != NULL);
	if (layer->frame_brush != NULL) {
		return _brush_color(layer->frame_brush);
	} else {
		return NULL;
	}
}

_color_t const* _layer_background_color(_layer_t const* layer) {
	assert(layer != NULL);
	if (layer->background_brush != NULL) {
		return _brush_color(layer->background_brush);
	} else {
		return NULL;
	}
}

_layer_t* _layer_parent(_layer_t const* layer) {
	assert(layer != NULL);
	return layer->parent;
}

_layer_t* _layer_children_item(_layer_t const* layer, int index) {
	assert(layer != NULL);
	assert((index >= 0) && (index < layer->children.size));
	return layer->children.data[index];
}

int _layer_children_count(_layer_t const* layer) {
	assert(layer != NULL);
	return layer->children.size;
}

void _layer_append_child(_layer_t* layer, _layer_t* child) {
	assert(layer != NULL);
	assert(child != NULL);
	_layer_remove_from_parent(child);
	_PUSH(layer->children, child);
}

bool _layer_remove_child(_layer_t* layer, _layer_t* child) {
	assert(layer != NULL);
	assert(child != NULL);
	int index = _INDEX_OF(layer->children, child);
	if (index > -1) {
		_REMOVE_INDEX(layer->children, index);
		child->parent = NULL;
		return true;
	} else {
		return false;
	}
}

bool _layer_remove_from_parent(_layer_t* layer) {
	assert(layer != NULL);
	if (layer->parent != NULL) {
		int index = _INDEX_OF(layer->parent->children, layer);
		_REMOVE_INDEX(layer->parent->children, index);
		layer->parent = NULL;
		return true;
	} else {
		return false;
	}
}

void _layer_paint(_layer_t const* layer, _context_t* context, _point_t const* point) {
	assert(layer != NULL);
	assert(context != NULL);
	_rect_t rect = {
		.origin = _POINT_ADD(layer->rect.origin, ((point != NULL) ? *point : ((_point_t){ 0, 0 }))),
		.size = layer->rect.size
	};

	rect.origin.x = round(rect.origin.x);
	rect.origin.y = round(rect.origin.y);
	rect.size.width = round(rect.size.width);
	rect.size.height = round(rect.size.height);

	if (layer->background_brush != NULL) {
		_context_fill_rect(context, &rect, layer->background_brush);
	}
	if ((layer->frame_brush != NULL)
	&& (layer->frame_width > 0)) {
		_rect_t rect_ = rect;
		rect_.origin.x += (layer->frame_width / 2);
		rect_.origin.y += (layer->frame_width / 2);
		rect_.size.width -= layer->frame_width;
		rect_.size.height -= layer->frame_width;
		_context_stroke_rect(context, &rect_, layer->frame_width, layer->frame_brush);
	}
	if ((layer->texture != NULL)
	&& (rect.size.width > 0)
	&& (rect.size.height > 0)) {
		_rect_t rect_ = { rect.origin, *_texture_size(layer->texture) };
		rect_.size.width *= layer->texture_scale;
		rect_.size.height *= layer->texture_scale;
		int cols = 1;
		int rows = 1;

		#define __COVER() { \
			double __ratio = _MAX((rect.size.width / rect_.size.width), (rect.size.height / rect_.size.height)); \
			rect_.size.width *= __ratio; \
			rect_.size.height *= __ratio; \
		}
		#define __REPEAT() { \
			cols = (int)ceil(rect.size.width / rect_.size.width); \
			rows = (int)ceil(rect.size.height / rect_.size.height); \
		}
		#define __OFFSET(X, Y) { \
			double __w = rect.size.width - (rect_.size.width * (double)cols); \
			double __h = rect.size.height - (rect_.size.height * (double)rows); \
			rect_.origin.x += (__w * X); \
			rect_.origin.y += (__h * Y); \
		}
		#define __DRAW_SIMPLE() { \
			_context_draw_texture(context, layer->texture, NULL, &rect_, NULL); \
		}
		#define __DRAW_REPEAT() { \
			_rect_t dst = rect_; \
			for (int i = 0; i < rows; i++) { \
				for (int j = 0; j < cols; j++) { \
					_context_draw_texture(context, layer->texture, NULL, &dst, NULL); \
					dst.origin.x += rect_.size.width; \
				} \
				dst.origin.x = rect_.origin.x; \
				dst.origin.y += rect_.size.height; \
			} \
		}

		if (layer->texture_layout == _DEFAULT_TEXTURE_LAYOUT) {
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _STRETCH_TEXTURE_LAYOUT) {
			if ((layer->stretch_indent_x > 0)
			|| (layer->stretch_indent_y > 0)) {
				double src_hor_p[3] = { 0, layer->stretch_indent_x, (rect_.size.width - layer->stretch_indent_x) };
				double src_ver_p[3] = { 0, layer->stretch_indent_y, (rect_.size.height - layer->stretch_indent_y) };
				double src_hor_w[3] = { layer->stretch_indent_x, (rect_.size.width - (layer->stretch_indent_x * 2)), layer->stretch_indent_x };
				double src_ver_w[3] = { layer->stretch_indent_y, (rect_.size.height - (layer->stretch_indent_y * 2)), layer->stretch_indent_y };
				double dst_hor_p[3] = { rect.origin.x, (rect.origin.x + layer->stretch_indent_x), (rect.origin.x + (rect.size.width - layer->stretch_indent_x)) };
				double dst_ver_p[3] = { rect.origin.y, (rect.origin.y + layer->stretch_indent_y), (rect.origin.y + (rect.size.height - layer->stretch_indent_y)) };
				double dst_hor_w[3] = { layer->stretch_indent_x, (rect.size.width - (layer->stretch_indent_x * 2)), layer->stretch_indent_x };
				double dst_ver_w[3] = { layer->stretch_indent_y, (rect.size.height - (layer->stretch_indent_y * 2)), layer->stretch_indent_y };

				_rect_t rc1_src = (_rect_t){ { src_hor_p[0], src_ver_p[0] }, { src_hor_w[0], src_ver_w[0] } };
				_rect_t rc1_dst = (_rect_t){ { dst_hor_p[0], dst_ver_p[0] }, { dst_hor_w[0], dst_ver_w[0] } };
				_rect_t rc2_src = (_rect_t){ { src_hor_p[1], src_ver_p[0] }, { src_hor_w[1], src_ver_w[0] } };
				_rect_t rc2_dst = (_rect_t){ { dst_hor_p[1], dst_ver_p[0] }, { dst_hor_w[1], dst_ver_w[0] } };
				_rect_t rc3_src = (_rect_t){ { src_hor_p[2], src_ver_p[0] }, { src_hor_w[2], src_ver_w[0] } };
				_rect_t rc3_dst = (_rect_t){ { dst_hor_p[2], dst_ver_p[0] }, { dst_hor_w[2], dst_ver_w[0] } };
				_rect_t rc4_src = (_rect_t){ { src_hor_p[0], src_ver_p[1] }, { src_hor_w[0], src_ver_w[1] } };
				_rect_t rc4_dst = (_rect_t){ { dst_hor_p[0], dst_ver_p[1] }, { dst_hor_w[0], dst_ver_w[1] } };
				_rect_t rc5_src = (_rect_t){ { src_hor_p[1], src_ver_p[1] }, { src_hor_w[1], src_ver_w[1] } };
				_rect_t rc5_dst = (_rect_t){ { dst_hor_p[1], dst_ver_p[1] }, { dst_hor_w[1], dst_ver_w[1] } };
				_rect_t rc6_src = (_rect_t){ { src_hor_p[2], src_ver_p[1] }, { src_hor_w[2], src_ver_w[1] } };
				_rect_t rc6_dst = (_rect_t){ { dst_hor_p[2], dst_ver_p[1] }, { dst_hor_w[2], dst_ver_w[1] } };
				_rect_t rc7_src = (_rect_t){ { src_hor_p[0], src_ver_p[2] }, { src_hor_w[0], src_ver_w[2] } };
				_rect_t rc7_dst = (_rect_t){ { dst_hor_p[0], dst_ver_p[2] }, { dst_hor_w[0], dst_ver_w[2] } };
				_rect_t rc8_src = (_rect_t){ { src_hor_p[1], src_ver_p[2] }, { src_hor_w[1], src_ver_w[2] } };
				_rect_t rc8_dst = (_rect_t){ { dst_hor_p[1], dst_ver_p[2] }, { dst_hor_w[1], dst_ver_w[2] } };
				_rect_t rc9_src = (_rect_t){ { src_hor_p[2], src_ver_p[2] }, { src_hor_w[2], src_ver_w[2] } };
				_rect_t rc9_dst = (_rect_t){ { dst_hor_p[2], dst_ver_p[2] }, { dst_hor_w[2], dst_ver_w[2] } };

				_context_draw_texture(context, layer->texture, &rc1_src, &rc1_dst, NULL);
				_context_draw_texture(context, layer->texture, &rc2_src, &rc2_dst, NULL);
				_context_draw_texture(context, layer->texture, &rc3_src, &rc3_dst, NULL);
				_context_draw_texture(context, layer->texture, &rc4_src, &rc4_dst, NULL);
				_context_draw_texture(context, layer->texture, &rc5_src, &rc5_dst, NULL);
				_context_draw_texture(context, layer->texture, &rc6_src, &rc6_dst, NULL);
				_context_draw_texture(context, layer->texture, &rc7_src, &rc7_dst, NULL);
				_context_draw_texture(context, layer->texture, &rc8_src, &rc8_dst, NULL);
				_context_draw_texture(context, layer->texture, &rc9_src, &rc9_dst, NULL);
			} else {
				rect_.size = rect.size;
				__DRAW_SIMPLE();
			}
		} else if (layer->texture_layout == _BEGIN_BEGIN_TEXTURE_LAYOUT) {
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _BEGIN_CENTER_TEXTURE_LAYOUT) {
			__OFFSET(0.0, 0.5);
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _BEGIN_END_TEXTURE_LAYOUT) {
			__OFFSET(0.0, 1.0);
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _CENTER_BEGIN_TEXTURE_LAYOUT) {
			__OFFSET(0.5, 0.0);
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _CENTER_CENTER_TEXTURE_LAYOUT) {
			__OFFSET(0.5, 0.5);
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _CENTER_END_TEXTURE_LAYOUT) {
			__OFFSET(0.5, 1.0);
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _END_BEGIN_TEXTURE_LAYOUT) {
			__OFFSET(1.0, 0.0);
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _END_CENTER_TEXTURE_LAYOUT) {
			__OFFSET(1.0, 0.5);
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _END_END_TEXTURE_LAYOUT) {
			__OFFSET(1.0, 1.0);
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _COVER_BEGIN_BEGIN_TEXTURE_LAYOUT) {
			__COVER();
		} else if (layer->texture_layout == _COVER_BEGIN_CENTER_TEXTURE_LAYOUT) {
			__COVER();
			__OFFSET(0.0, 0.5);
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _COVER_BEGIN_END_TEXTURE_LAYOUT) {
			__COVER();
			__OFFSET(0.0, 1.0);
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _COVER_CENTER_BEGIN_TEXTURE_LAYOUT) {
			__COVER();
			__OFFSET(0.5, 0.0);
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _COVER_CENTER_CENTER_TEXTURE_LAYOUT) {
			__COVER();
			__OFFSET(0.5, 0.5);
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _COVER_CENTER_END_TEXTURE_LAYOUT) {
			__COVER();
			__OFFSET(0.5, 1.0);
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _COVER_END_BEGIN_TEXTURE_LAYOUT) {
			__COVER();
			__OFFSET(1.0, 0.0);
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _COVER_END_CENTER_TEXTURE_LAYOUT) {
			__COVER();
			__OFFSET(1.0, 0.5);
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _COVER_END_END_TEXTURE_LAYOUT) {
			__COVER();
			__OFFSET(1.0, 1.0);
			__DRAW_SIMPLE();
		} else if (layer->texture_layout == _REPEAT_BEGIN_BEGIN_TEXTURE_LAYOUT) {
			__REPEAT();
			__DRAW_REPEAT();
		} else if (layer->texture_layout == _REPEAT_BEGIN_CENTER_TEXTURE_LAYOUT) {
			__REPEAT();
			__OFFSET(0.0, 0.5);
			__DRAW_REPEAT();
		} else if (layer->texture_layout == _REPEAT_BEGIN_END_TEXTURE_LAYOUT) {
			__REPEAT();
			__OFFSET(0.0, 1.0);
			__DRAW_REPEAT();
		} else if (layer->texture_layout == _REPEAT_CENTER_BEGIN_TEXTURE_LAYOUT) {
			__REPEAT();
			__OFFSET(0.5, 0.0);
			__DRAW_REPEAT();
		} else if (layer->texture_layout == _REPEAT_CENTER_CENTER_TEXTURE_LAYOUT) {
			__REPEAT();
			__OFFSET(0.5, 0.5);
			__DRAW_REPEAT();
		} else if (layer->texture_layout == _REPEAT_CENTER_END_TEXTURE_LAYOUT) {
			__REPEAT();
			__OFFSET(0.5, 1.0);
			__DRAW_REPEAT();
		} else if (layer->texture_layout == _REPEAT_END_BEGIN_TEXTURE_LAYOUT) {
			__REPEAT();
			__OFFSET(1.0, 0.0);
			__DRAW_REPEAT();
		} else if (layer->texture_layout == _REPEAT_END_CENTER_TEXTURE_LAYOUT) {
			__REPEAT();
			__OFFSET(1.0, 0.5);
			__DRAW_REPEAT();
		} else if (layer->texture_layout == _REPEAT_END_END_TEXTURE_LAYOUT) {
			__REPEAT();
			__OFFSET(1.0, 1.0);
			__DRAW_REPEAT();
		}

		#undef __COVER
		#undef __REPEAT
		#undef __OFFSET
		#undef __DRAW_SIMPLE
		#undef __DRAW_REPEAT
	}
	for (int i = 0; i < layer->children.size; i++) {
		_layer_paint(layer->children.data[i], context, &rect.origin);
	}
}
