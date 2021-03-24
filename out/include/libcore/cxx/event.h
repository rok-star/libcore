#pragma once

#include <functional>
#include <libcore/cxx/array.h>

namespace ext {

template<typename... A>
class event_source {
public:
	void trigger(A... args) {
		;
	}
};

} /* namespace ext */