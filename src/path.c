#include <stdbool.h>
#include <libcore/MACRO.h>
#include <libcore/path.h>

char* _path_resolve(char const* path) {
    _ASSERT(path != NULL);
    return NULL;
}

char* _path_normalize(char const* path) {
    _ASSERT(path != NULL);
    return NULL;
}

char* _path_join(char const** parts) {
    _ASSERT(parts != NULL);
    return NULL;
}

bool _path_exists(char const* path) {
	_ASSERT(path != NULL);
	_path_stat_t stat = {};
	_path_stat(path, &stat);
	return stat.exists;
}

bool _path_is_file(char const* path) {
	_ASSERT(path != NULL);
	_path_stat_t stat = {};
	_path_stat(path, &stat);
	return stat.file;
}

bool _path_is_diretory(char const* path) {
	_ASSERT(path != NULL);
	_path_stat_t stat = {};
	_path_stat(path, &stat);
	return stat.directory;
}