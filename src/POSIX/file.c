#ifdef __linux__
    #define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <libcore/MACRO.h>
#include <libcore/file.h>

static void __file_write(char const* path, uint8_t const* data, int64_t size, bool append, _status_t* status) {
	assert(path != NULL);
	assert(data != NULL);
	FILE* file = fopen(path, (append ? "a" : "w"));
	if (file != NULL) {
		if (fwrite(data, 1, size, file) == (size_t)size) {
			_status_set(status, _SUCCESS_STATUS_TYPE, NULL);
		} else {
			char* message = _FORMAT("File bad write \"%s\"", path);
			_status_set(status, _FAILURE_STATUS_TYPE, message);
			_FREE(message);
		}
		fclose(file);
	} else {
		char* message = _FORMAT("Unable to open file \"%s\"", path);
		_status_set(status, _FAILURE_STATUS_TYPE, message);
		_FREE(message);
	}
}

void _file_write(char const* path, uint8_t const* data, int64_t size, _status_t* status) {
	assert(path != NULL);
	assert(data != NULL);
	__file_write(path, data, size, false, status);
}

void _file_append(char const* path, uint8_t const* data, int64_t size, _status_t* status) {
	assert(path != NULL);
	assert(data != NULL);
	__file_write(path, data, size, true, status);
}

void _file_read(char const* path, uint8_t** data, int64_t* size, _status_t* status) {
	assert(path != NULL);
	assert(data != NULL);
	assert(size != NULL);
	FILE* file = fopen(path, "rb");
	if (file != NULL) {
		fseek(file, 0, SEEK_END);
		(*size) = ftell(file);
		(*data) = _ALLOC(uint8_t, (*size) + 1);
		rewind(file);
		if (fread((*data), 1, (*size), file) == (size_t)(*size)) {
			_status_set(status, _SUCCESS_STATUS_TYPE, NULL);
		} else {
			char* message = _FORMAT("File bad read \"%s\"", path);
			_status_set(status, _FAILURE_STATUS_TYPE, message);
			_FREE(message);
		}
		fclose(file);
	} else {
		char* message = _FORMAT("Unable to open file \"%s\"", path);
		_status_set(status, _FAILURE_STATUS_TYPE, message);
		_FREE(message);
	}
}

void _file_remove(char const* path, _status_t* status) {
	assert(path != NULL);

}