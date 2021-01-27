#pragma once

#include <functional>
#include <libext/cxx/array.h>

namespace ext {

template<typename... A>
class event_source {
public:
	void trigger(A... args) {
		;
	}
};

} /* namespace ext */