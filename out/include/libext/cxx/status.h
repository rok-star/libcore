#pragma once

#include <libext/status.h>

namespace ext {

enum class status_type { success, failure };

class status {
private:
	ext::string _message;
	ext::status_type _type;
public:
	status(_status_t const& status) {
		_type = ((status.type == _SUCCESS_STATUS_TYPE)
				? ext::status_type::success
				: ext::status_type::failure);
		_message = status.message;
	}
	status() = default;
	status(ext::status const& status) = default;
	status(ext::status && status) = default;
	ext::status& operator=(ext::status const& status) = default;
	ext::status& operator=(ext::status && status) = default;
	ext::string const& message() const {
		return _message;
	}
	ext::status_type const& type() const {
		return _type;
	}
};

} /* namespace ext */