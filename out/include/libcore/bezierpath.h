#ifndef _LIBCORE_BEZIERPATH_H
#define _LIBCORE_BEZIERPATH_H

#include <stdbool.h>
#include <libcore/math.h>

typedef enum {
    _BUTT_LINE_CAP,
    _SQUARE_LINE_CAP,
    _ROUND_LINE_CAP
} _LINE_CAP;

typedef enum {
    _ROUND_LINE_JOIN,
    _FLAT_LINE_JOIN
} _LINE_JOIN;

typedef struct _bezier_path_t _bezier_path_t;

#ifdef __cplusplus
extern "C" {
#endif

_bezier_path_t* _bezier_path_create(void);
void _bezier_path_destroy(_bezier_path_t*);
void _bezier_path_set_line_cap(_bezier_path_t*,_LINE_CAP);
void _bezier_path_set_line_join(_bezier_path_t*,_LINE_JOIN);
void _bezier_path_set_closed(_bezier_path_t*,bool);
void _bezier_path_move_to(_bezier_path_t*,_point_t const*);
void _bezier_path_line_to(_bezier_path_t*,_point_t const*);
void _bezier_path_cubic_to(_bezier_path_t*,_point_t const*,_point_t const*,_point_t const*);
void _bezier_path_quad_to(_bezier_path_t*,_point_t const*,_point_t const*);
void _bezier_path_conic_to(_bezier_path_t*,_point_t const*,_point_t const*,double);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_BEZIERPATH_H */
