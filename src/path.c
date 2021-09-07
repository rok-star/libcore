#include <assert.h>
#include <stdbool.h>
#include <libcore/MACRO.h>
#include <libcore/path.h>

char* _path_resolve(char const* path) {
    assert(path != NULL);
    return NULL;
}

char* _path_normalize(char const* path) {
    assert(path != NULL);
    return NULL;
}

char* _path_join(char const** parts, int64_t num) {
    assert(parts != NULL);
    int64_t size = 0;
    for (int64_t i = 0; i < num; i++) {
    	size += (strlen(parts[i]) + 1);
    }
    char* ret = _ALLOC(char, size);
    char* dst = ret;
    for (int64_t i = 0; i < num; i++) {
    	int64_t len = strlen(parts[i]);
    	memcpy(dst, parts[i], len);
    	dst += len;
    	if (i < (num - 1)) {
    		memcpy(dst, _PATH_SEPARATOR, 1);
    		dst += 1;
    	}
    }
    return ret;
}

bool _path_exists(char const* path) {
	assert(path != NULL);
	_path_stat_t stat = {};
	_path_stat(path, &stat);
	return stat.exists;
}

bool _path_is_file(char const* path) {
	assert(path != NULL);
	_path_stat_t stat = {};
	_path_stat(path, &stat);
	return stat.file;
}

bool _path_is_diretory(char const* path) {
	assert(path != NULL);
	_path_stat_t stat = {};
	_path_stat(path, &stat);
	return stat.directory;
}