#include <libcore/MACRO.h>
#include <libcore/math.h>

// https://webglfundamentals.org/webgl/lessons/webgl-3d-geometry-lathe.html

_PointF _bezier_point(_PointF const* p1, _PointF const* p2, _PointF const* p3, _PointF const* p4, float t) {
	_ASSERT(p1 != NULL);
	_ASSERT(p2 != NULL);
	_ASSERT(p3 != NULL);
	_ASSERT(p4 != NULL);
	float tt = (1.0 - t);
	_PointF _1 = _POINT_MULT(*p1, (tt * tt * tt));
    _PointF _2 = _POINT_MULT(*p2, (3.0 * t * tt * tt));
    _PointF _3 = _POINT_MULT(*p3, (3.0 * tt * t * t));
    _PointF _4 = _POINT_MULT(*p4, (t * t * t));
    _PointF _5 = _POINT_ADD(_1, _2);
    _PointF _6 = _POINT_ADD(_5, _3);
    _PointF _7 = _POINT_ADD(_6, _4);
    return _7;
}

void _bezier_points(_PointF const* p1, _PointF const* p2, _PointF const* p3, _PointF const* p4, float fidelity, _PointF* out, int* num) {
	_ASSERT(p1 != NULL);
	_ASSERT(p2 != NULL);
	_ASSERT(p3 != NULL);
	_ASSERT(p4 != NULL);
	_ASSERT(out != NULL);
	_ASSERT(num != NULL);
	if (_POINT_FLATNESS(*p1, *p2, *p3, *p4) < (1.0 - fidelity)) {
		(*num) = 2;
		(*out) = _ALLOC(_PointF, (*num));
		(*out)[0] = (*p1);
		(*out)[1] = (*p4);
		return;
	} else {
		float t = 0.5;
		_PointF q1 = _POINT_LERP(*p1, *p2, t);
		_PointF q2 = _POINT_LERP(*p2, *p3, t);
		_PointF q3 = _POINT_LERP(*p3, *p4, t);
		_PointF r1 = _POINT_LERP(q1, q2, t);
		_PointF r2 = _POINT_LERP(q2, q3, t);
		_PointF pt = _POINT_LERP(r1, r2, t);
	}
}