#pragma once

#include <libext/math.h>
#include <libext/cxx/base.h>

namespace ext {

template<typename T>
struct point {
	T x;
	T y;
	_point_t point_t() const {
		return {
			static_cast<double>(x),
			static_cast<double>(y)
		};
	}
};

template<typename T>
struct size {
	T width;
	T height;
	_size_t size_t() const {
		return {
			static_cast<double>(width),
			static_cast<double>(height)
		};
	}
};

template<typename T>
struct rect {
	ext::point<T> origin;
	ext::size<T> size;
	_rect_t rect_t() const {
		return {
			{ origin.x, origin.y },
			{ size.width, size.height }
		};
	}
};

} /* namespace ext */