#ifndef _LIBCORE_BEZIERPATH_H
#define _LIBCORE_BEZIERPATH_H

#include <libcore/line.h>

typedef struct _BezierPath _BezierPath;

#ifdef __cplusplus
extern "C" {
#endif

_BezierPath* _BezierPath_create(void);
void _BezierPath_destroy(_BezierPath*);
void _BezierPath_set_line_cap(_BezierPath*,_LINE_CAP);
void _BezierPath_set_line_join(_BezierPath*,_LINE_JOIN);
void _BezierPath_set_closed(_BezierPath*,bool);
void _BezierPath_move_to(_BezierPath*,_PointF const*);
void _BezierPath_line_to(_BezierPath*,_PointF const*);
void _BezierPath_cubic_to(_BezierPath*,_PointF const*,_PointF const*,_PointF const*);
void _BezierPath_quad_to(_BezierPath*,_PointF const*,_PointF const*);
void _BezierPath_conic_to(_BezierPath*,_PointF const*,_PointF const*,double);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_BEZIERPATH_H */
