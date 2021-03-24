#pragma once

#include <libcore/file.h>
#include <libcore/cxx/base.h>
#include <libcore/cxx/array.h>
#include <libcore/cxx/string.h>
#include <libcore/cxx/status.h>

namespace ext {

ext::array<uint8_t> file_read(char const* path, ext::status& status) {
	ASSERT(path != nullptr);
	_status_t status_ = {};
	uint8_t* data = nullptr;
	int64_t size = 0;
	_file_read(path, &data, &size, &status_);
	status = ext::status(status_);
	return ext::array<uint8_t>::with_data(data, size);
}

ext::array<uint8_t> file_read(ext::string path, ext::status& status) {
	return ext::file_read(path.c_string(), status);
}

} /* namespace ext */