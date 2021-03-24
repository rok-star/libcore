#pragma once

#include <libcore/path.h>
#include <libcore/cxx/string.h>

namespace ext {

class stat {
private:
	_path_stat_t _stat;
public:
	stat(char const* pchar) {
		ASSERT(pchar != nullptr);
		_path_stat(pchar, &_stat);
	}
	bool exists() const {
		return _stat.exists;
	}
    bool file() const {
    	return _stat.file;
    }
    bool directory() const {
    	return _stat.directory;
    }
    double modified() const {
    	return _stat.modified;
    }
    int64_t size() const {
    	return _stat.size;
    }
};

ext::string path_cwd() {
	char* pchar = _path_cwd();
	return ext::string::with_pchar(pchar, strlen(pchar));
}

ext::string path_resolve(char const* pchar) {
	return ext::string();
}

ext::string path_resolve(ext::string const& string) {
	return ext::string();
}

ext::string path_normalize(char const* pchar) {
	return ext::string();
}

ext::string path_normalize(ext::string const& string) {
	return ext::string();
}

template<typename... A>
ext::string path_join(A const&... args) {
	return ext::string();
}

ext::stat path_stat(char const* pchar) {
	return ext::stat(pchar);
}

ext::stat path_stat(ext::string const& string) {
	return ext::stat(string.c_string());
}

} /* namespace ext */