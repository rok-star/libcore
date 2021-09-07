#include <assert.h>
#include <libcore/core/MACRO.h>
#include <libcore/core/math.h>

// https://webglfundamentals.org/webgl/lessons/webgl-3d-geometry-lathe.html

static double __cubic_flatness(_point_t const* p1, _point_t const* p2, _point_t const* p3, _point_t const* p4) {
    assert(p1 != NULL);
    assert(p2 != NULL);
    assert(p3 != NULL);
    assert(p4 != NULL);
    double ux = ((3.0 * p2->x) - (2.0 * p1->x) - p4->x); ux *= ux;
    double uy = ((3.0 * p2->y) - (2.0 * p1->y) - p4->y); uy *= uy;
    double vx = ((3.0 * p3->x) - (2.0 * p4->x) - p1->x); vx *= vx;
    double vy = ((3.0 * p3->y) - (2.0 * p4->y) - p1->y); vy *= vy;
    if (ux < vx) ux = vx;
    if (uy < vy) uy = vy;
    return (ux + uy);
}

static void __cubic_points_add(_point_t const** points, double tolerance, _point_t** out, int* num, int* cap) {
    assert(points != NULL);
    assert(out != NULL);
    assert(num != NULL);
    assert(cap != NULL);
    if (__cubic_flatness(points[0], points[1], points[2], points[3]) < tolerance) {
        _PUSH_V((*out), (*num), (*cap), *points[0]);
        _PUSH_V((*out), (*num), (*cap), *points[3]);
    } else {
        double t = 0.5;
        _point_t q1 = _POINT_LERP(*points[0], *points[1], t);
        _point_t q2 = _POINT_LERP(*points[1], *points[2], t);
        _point_t q3 = _POINT_LERP(*points[2], *points[3], t);
        _point_t r1 = _POINT_LERP(q1, q2, t);
        _point_t r2 = _POINT_LERP(q2, q3, t);
        _point_t pt = _POINT_LERP(r1, r2, t);
        __cubic_points_add((_point_t const*[]){ points[0], &q1, &r1, &pt }, tolerance, out, num, cap);
        __cubic_points_add((_point_t const*[]){ &pt, &r2, &q3, points[3] }, tolerance, out, num, cap);
    }
}

_point_t _cubic_point(_point_t const* p1, _point_t const* p2, _point_t const* p3, _point_t const* p4, double t) {
    assert(p1 != NULL);
    assert(p2 != NULL);
    assert(p3 != NULL);
    assert(p4 != NULL);
    double tt = (1.0 - t);
    _point_t _1 = _POINT_MULT(*p1, (tt * tt * tt));
    _point_t _2 = _POINT_MULT(*p2, (3.0 * t * tt * tt));
    _point_t _3 = _POINT_MULT(*p3, (3.0 * tt * t * t));
    _point_t _4 = _POINT_MULT(*p4, (t * t * t));
    _point_t _5 = _POINT_ADD(_1, _2);
    _point_t _6 = _POINT_ADD(_5, _3);
    _point_t _7 = _POINT_ADD(_6, _4);
    return _7;
}

void _cubic_points(_point_t const* p1, _point_t const* p2, _point_t const* p3, _point_t const* p4, double fidelity, _point_t** out, int* num) {
    assert(p1 != NULL);
    assert(p2 != NULL);
    assert(p3 != NULL);
    assert(p4 != NULL);
    assert(out != NULL);
    assert(num != NULL);
    (*out) = NULL;
    (*num) = 0;
    int cap = 0;
    __cubic_points_add((_point_t const*[]){ p1, p2, p3, p4 }, (1.0 - fidelity) + 0.001, out, num, &cap);
}