#ifndef _LIBCORE_CONTEXT_H
#define _LIBCORE_CONTEXT_H

#include <libcore/core/math.h>
#include <libcore/gfx/brush.h>
#include <libcore/gfx/bezierpath.h>
#include <libcore/gfx/texture.h>
#include <libcore/ui/window.h>

typedef enum {
    _LEFTTOP_CONTEXT_ORIGIN,
    _LEFTBOTTOM_CONTEXT_ORIGIN
} _CONTEXT_ORIGIN;

typedef struct _context_t _context_t;

#ifdef __cplusplus
extern "C" {
#endif

_context_t* _context_create_texture(_texture_t const*);
_context_t* _context_create_window(_window_t const*);
void _context_destroy(_context_t*);
void _context_begin_paint(_context_t*);
void _context_end_paint(_context_t*);
float _context_scale(_context_t const*);
_size_t const* _context_size(_context_t const*);
_rect_t const* _context_clip(_context_t const*);
_CONTEXT_ORIGIN _context_origin(_context_t const*);
void _context_set_scale(_context_t*,float);
void _context_set_clip(_context_t*,_rect_t const*);
void _context_set_origin(_context_t*,_CONTEXT_ORIGIN);
void _context_draw_vertices(_context_t const*,float const*,int,bool,_brush_t const*);
void _context_draw_texture(_context_t const*,_texture_t const*,_rect_t const*,_rect_t const*,_color_t const*);
void _context_stroke_line(_context_t const*,_point_t const*,_point_t const*,double,_LINE_CAP,_brush_t const*);
void _context_stroke_rect(_context_t const*,_rect_t const*,double,_brush_t const*);
void _context_stroke_path(_context_t const*,_bezier_path_t const*,double,_brush_t const*);
void _context_stroke_ellipse(_context_t const*,_rect_t const*,double,_brush_t const*);
void _context_fill_rect(_context_t const*,_rect_t const*,_brush_t const*);
void _context_fill_path(_context_t const*,_bezier_path_t const*,_brush_t const*);
void _context_fill_ellipse(_context_t const*,_rect_t const*,_brush_t const*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_CONTEXT_H */
