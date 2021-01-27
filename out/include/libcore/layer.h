#ifndef _LIBEXT_LAYER_H
#define _LIBEXT_LAYER_H

#include <libext/math.h>
#include <libext/context.h>
#include <libext/texture.h>

typedef enum {
	_DEFAULT_TEXTURE_LAYOUT,
	_STRETCH_TEXTURE_LAYOUT,
	_BEGIN_BEGIN_TEXTURE_LAYOUT,
	_BEGIN_CENTER_TEXTURE_LAYOUT,
	_BEGIN_END_TEXTURE_LAYOUT,
	_CENTER_BEGIN_TEXTURE_LAYOUT,
	_CENTER_CENTER_TEXTURE_LAYOUT,
	_CENTER_END_TEXTURE_LAYOUT,
	_END_BEGIN_TEXTURE_LAYOUT,
	_END_CENTER_TEXTURE_LAYOUT,
	_END_END_TEXTURE_LAYOUT,
	_COVER_BEGIN_BEGIN_TEXTURE_LAYOUT,
	_COVER_BEGIN_CENTER_TEXTURE_LAYOUT,
	_COVER_BEGIN_END_TEXTURE_LAYOUT,
	_COVER_CENTER_BEGIN_TEXTURE_LAYOUT,
	_COVER_CENTER_CENTER_TEXTURE_LAYOUT,
	_COVER_CENTER_END_TEXTURE_LAYOUT,
	_COVER_END_BEGIN_TEXTURE_LAYOUT,
	_COVER_END_CENTER_TEXTURE_LAYOUT,
	_COVER_END_END_TEXTURE_LAYOUT,
	_REPEAT_BEGIN_BEGIN_TEXTURE_LAYOUT,
	_REPEAT_BEGIN_CENTER_TEXTURE_LAYOUT,
	_REPEAT_BEGIN_END_TEXTURE_LAYOUT,
	_REPEAT_CENTER_BEGIN_TEXTURE_LAYOUT,
	_REPEAT_CENTER_CENTER_TEXTURE_LAYOUT,
	_REPEAT_CENTER_END_TEXTURE_LAYOUT,
	_REPEAT_END_BEGIN_TEXTURE_LAYOUT,
	_REPEAT_END_CENTER_TEXTURE_LAYOUT,
	_REPEAT_END_END_TEXTURE_LAYOUT
} _TEXTURE_LAYOUT;

typedef struct _layer_t _layer_t;

#ifdef __cplusplus
extern "C" {
#endif

_layer_t* _layer_create(void);
void _layer_destroy(_layer_t*);
void _layer_set_center(_layer_t*,_point_t const*);
void _layer_set_origin(_layer_t*,_point_t const*);
void _layer_set_size(_layer_t*,_size_t const*);
void _layer_set_texture(_layer_t*,_texture_t const*);
void _layer_set_texture_layout(_layer_t*,_TEXTURE_LAYOUT);
void _layer_set_texture_scale(_layer_t*,double);
void _layer_set_texture_stretch_indent(_layer_t*,int,int);
void _layer_set_frame_width(_layer_t*,double);
void _layer_set_frame_color(_layer_t*,_color_t const*);
void _layer_set_background_color(_layer_t*,_color_t const*);
_point_t const* _layer_center(_layer_t const*);
_point_t const* _layer_origin(_layer_t const*);
_size_t const* _layer_size(_layer_t const*);
_texture_t const* _layer_texture(_layer_t const*);
double _layer_frame_width(_layer_t const*);
_color_t const* _layer_frame_color(_layer_t const*);
_color_t const* _layer_background_color(_layer_t const*);
_layer_t* _layer_next(_layer_t const*);
_layer_t* _layer_prev(_layer_t const*);
_layer_t* _layer_parent(_layer_t const*);
_layer_t* _layer_children_item(_layer_t const*,int);
int _layer_children_count(_layer_t const*);
void _layer_append_child(_layer_t*,_layer_t*);
void _layer_prepend_child(_layer_t*,_layer_t*);
void _layer_insert_after(_layer_t*,_layer_t*,_layer_t const*);
void _layer_insert_before(_layer_t*,_layer_t*,_layer_t const*);
bool _layer_remove_child(_layer_t*,_layer_t*);
bool _layer_remove_from_parent(_layer_t*);
void _layer_path(_layer_t const*,_layer_t**,int*);
void _layer_paint(_layer_t const*,_context_t*,_point_t const*);
_layer_t* _layer_hittest(_layer_t const*,_point_t const*);


#ifdef __cplusplus
}
#endif

#endif /* _LIBEXT_LAYER_H */
