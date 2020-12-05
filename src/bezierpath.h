#ifndef _LIBCORE_BEZIERPATH_H
#define _LIBCORE_BEZIERPATH_H

typedef struct _BezierPath _BezierPath;

#ifdef __cplusplus
extern "C" {
#endif

_BezierPath* _BezierPath_create(_Color const*);
void _BezierPath_destroy(_BezierPath*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_BEZIERPATH_H */
