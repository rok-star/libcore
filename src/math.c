#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libcore/MACRO.h>
#include <libcore/math.h>

// https://webglfundamentals.org/webgl/lessons/webgl-3d-geometry-lathe.html

_PointF _cubic_point(_PointF const* p1, _PointF const* p2, _PointF const* p3, _PointF const* p4, double t) {
	_ASSERT(p1 != NULL);
	_ASSERT(p2 != NULL);
	_ASSERT(p3 != NULL);
	_ASSERT(p4 != NULL);
	double tt = (1.0 - t);
	_PointF _1 = _POINT_MULT(*p1, (tt * tt * tt));
    _PointF _2 = _POINT_MULT(*p2, (3.0 * t * tt * tt));
    _PointF _3 = _POINT_MULT(*p3, (3.0 * tt * t * t));
    _PointF _4 = _POINT_MULT(*p4, (t * t * t));
    _PointF _5 = _POINT_ADD(_1, _2);
    _PointF _6 = _POINT_ADD(_5, _3);
    _PointF _7 = _POINT_ADD(_6, _4);
    return _7;
}

double __cubic_flatness(_PointF const* p1, _PointF const* p2, _PointF const* p3, _PointF const* p4) {
	_ASSERT(p1 != NULL);
	_ASSERT(p2 != NULL);
	_ASSERT(p3 != NULL);
	_ASSERT(p4 != NULL);
	double ux = ((3.0 * p2->x) - (2.0 * p1->x) - p4->x); ux *= ux;
	double uy = ((3.0 * p2->y) - (2.0 * p1->y) - p4->y); uy *= uy;
	double vx = ((3.0 * p3->x) - (2.0 * p4->x) - p1->x); vx *= vx;
	double vy = ((3.0 * p3->y) - (2.0 * p4->y) - p1->y); vy *= vy;
	if (ux < vx) ux = vx;
	if (uy < vy) uy = vy;
	return (ux + uy);
}

void __cubic_points_add(_PointF const** points, double tolerance, _PointF** out, int* num, int* cap) {
	_ASSERT(points != NULL);
	_ASSERT(out != NULL);
	_ASSERT(num != NULL);
	_ASSERT(cap != NULL);
	if (__cubic_flatness(points[0], points[1], points[2], points[3]) < tolerance) {
		_PUSH((*out), (*num), (*cap), *points[0]);
		_PUSH((*out), (*num), (*cap), *points[3]);
	} else {
		double t = 0.5;
		_PointF q1 = _POINT_LERP(*points[0], *points[1], t);
		_PointF q2 = _POINT_LERP(*points[1], *points[2], t);
		_PointF q3 = _POINT_LERP(*points[2], *points[3], t);
		_PointF r1 = _POINT_LERP(q1, q2, t);
		_PointF r2 = _POINT_LERP(q2, q3, t);
		_PointF pt = _POINT_LERP(r1, r2, t);
		__cubic_points_add((_PointF const*[]){ points[0], &q1, &r1, &pt }, tolerance, out, num, cap);
		__cubic_points_add((_PointF const*[]){ &pt, &r2, &q3, points[3] }, tolerance, out, num, cap);
	}
}

void _cubic_points(_PointF const* p1, _PointF const* p2, _PointF const* p3, _PointF const* p4, double fidelity, _PointF** out, int* num) {
	_ASSERT(p1 != NULL);
	_ASSERT(p2 != NULL);
	_ASSERT(p3 != NULL);
	_ASSERT(p4 != NULL);
	_ASSERT(out != NULL);
	_ASSERT(num != NULL);
	(*out) = NULL;
	(*num) = 0;
	int cap = 0;
	__cubic_points_add((_PointF const*[]){ p1, p2, p3, p4 }, (1.0 - fidelity) + 0.001, out, num, &cap);
}