#include <libcore/any.h>
#include <libcore/string.h>
#include <libcore/MACRO.h>

typedef struct _any_entry_t {
	_ANY_ENTRY_TYPE type;
	char* path;
	char* string;
	double number;
	double date;
} _any_entry_t;

typedef struct _any_t {
	_ANY_TYPE type;
	void* data;
} _any_t;

typedef struct _any_array_t {
	_any_t** data;
	int64_t size;
	int64_t capacity;
} _any_array_t;

typedef struct _any_keyvalue_t {
	char* key;
	_any_t* value;
} _any_keyvalue_t;

typedef struct _any_keyvalue_array_t {
	_any_keyvalue_t* data;
	int64_t size;
	int64_t capacity;
} _any_keyvalue_array_t;

_any_entry_t* _any_entry_create_null(char const* path, int64_t len) {
	_ASSERT(path != NULL);
	return _NEW(_any_entry_t, {
		.type = _NULL_ANY_ENTRY_TYPE,
		.path = _string_clone(path, len)
	});
}

_any_entry_t* _any_entry_create_string(char const* path, int64_t len, char const* string, int64_t len2) {
	_ASSERT(path != NULL);
	_ASSERT(string != NULL);
	return _NEW(_any_entry_t, {
		.type = _STRING_ANY_ENTRY_TYPE,
		.path = _string_clone(path, len),
		.string = ((len > 0) ? _string_clone(string, len2) : (char*)_ALLOC(char, 1))
	});
}

_any_entry_t* _any_entry_create_number(char const* path, int64_t len, double number) {
	_ASSERT(path != NULL);
	return _NEW(_any_entry_t, {
		.type = _NUMBER_ANY_ENTRY_TYPE,
		.path = _string_clone(path, len),
		.number = number
	});
}

_any_entry_t* _any_entry_create_date(char const* path, int64_t len, double date) {
	_ASSERT(path != NULL);
	return _NEW(_any_entry_t, {
		.type = _DATE_ANY_ENTRY_TYPE,
		.path = _string_clone(path, len),
		.date = date
	});
}

_ANY_ENTRY_TYPE _any_entry_type(_any_entry_t* entry) {
	_ASSERT(entry != NULL);
	return entry->type;
}

char const* _any_entry_path(_any_entry_t* entry) {
	_ASSERT(entry != NULL);
	return entry->path;
}

char const* _any_entry_string(_any_entry_t* entry) {
	_ASSERT(entry != NULL);
	_ASSERT(entry->type == _STRING_ANY_ENTRY_TYPE);
	return entry->string;
}

double _any_entry_number(_any_entry_t* entry) {
	_ASSERT(entry != NULL);
	_ASSERT(entry->type == _NUMBER_ANY_ENTRY_TYPE);
	return entry->number;
}

double _any_entry_date(_any_entry_t* entry) {
	_ASSERT(entry != NULL);
	_ASSERT(entry->type == _DATE_ANY_ENTRY_TYPE);
	return entry->date;
}

void _any_entry_destroy(_any_entry_t* entry) {
	_ASSERT(entry != NULL);
	_FREE(entry->string);
	_FREE(entry);
}

_any_t* _any_clone(_any_t const* any) {
	return NULL;
}

_any_t* _any_create_null(void) {
	return NULL;
}

_any_t* _any_create_string(char const* string) {
	return NULL;
}

_any_t* _any_create_number(double number) {
	return NULL;
}

_any_t* _any_create_date(double date) {
	return NULL;
}

_any_t* _any_create_array(void) {
	return NULL;
}

_any_t* _any_create_map(void) {
	return NULL;
}

_any_t* _any_create_entries(_any_entry_t const** entries, int64_t size) {
	return NULL;
}

void _any_destroy(_any_t* any) {

}

void _any_set_null(_any_t* any) {

}

void _any_set_string(_any_t* any, char const* string) {

}

void _any_set_number(_any_t* any, double number) {

}

void _any_set_date(_any_t* any, double date) {

}

void _any_set_array(_any_t* any) {

}

void _any_set_map(_any_t* any) {

}

_ANY_TYPE _any_type(_any_t const* any) {
	_ASSERT(any != NULL);
	return any->type;
}

char const* _any_string(_any_t const* any) {
	_ASSERT(any != NULL);
	_ASSERT(any->data != NULL);
	_ASSERT(any->type == _STRING_ANY_TYPE);
	return (char const*)any->data;
}

double _any_number(_any_t const* any) {
	_ASSERT(any != NULL);
	_ASSERT(any->data != NULL);
	_ASSERT(any->type == _NUMBER_ANY_TYPE);
	return *(double*)any->data;
}

double _any_date(_any_t const* any) {
	_ASSERT(any != NULL);
	_ASSERT(any->data != NULL);
	_ASSERT(any->type == _DATE_ANY_TYPE);
	return *(double*)any->data;
}

int64_t _any_array_count(_any_t const* any) {
	_ASSERT(any != NULL);
	_ASSERT(any->data != NULL);
	_ASSERT(any->type == _ARRAY_ANY_TYPE);
	return ((_any_array_t*)any->data)->size;
}

_any_t* _any_array_item(_any_t const* any, int64_t index) {
	_ASSERT(any != NULL);
	_ASSERT(any->data != NULL);
	_ASSERT(any->type == _ARRAY_ANY_TYPE);
	_ASSERT(index >= 0);
	_any_array_t* array = (_any_array_t*)any->data;
	_ASSERT(index < array->size);
	return array->data[index];
}

void _any_array_push(_any_t* any, _any_t const* item) {
	_ASSERT(any != NULL);
	_ASSERT(any->data != NULL);
	_ASSERT(any->type == _ARRAY_ANY_TYPE);
	_any_array_t* array = (_any_array_t*)any->data;
	_any_t* copy = _any_clone(any);
	_PUSH(*array, copy);
}

void _any_array_unshift(_any_t* any, _any_t const* item) {
	_ASSERT(any != NULL);
	_ASSERT(any->data != NULL);
	_ASSERT(any->type == _ARRAY_ANY_TYPE);
	_any_array_t* array = (_any_array_t*)any->data;
	_any_t* copy = _any_clone(any);
	_UNSHIFT(*array, copy);
}

_any_t* _any_array_pop(_any_t* any) {
	_ASSERT(any != NULL);
	_ASSERT(any->data != NULL);
	_ASSERT(any->type == _ARRAY_ANY_TYPE);
	_any_array_t* array = (_any_array_t*)any->data;
	return _POP(*array);
}

_any_t* _any_array_shift(_any_t* any) {
	_ASSERT(any != NULL);
	_ASSERT(any->data != NULL);
	_ASSERT(any->type == _ARRAY_ANY_TYPE);
	_any_array_t* array = (_any_array_t*)any->data;
	return _SHIFT(*array);
}

void _any_array_clear(_any_t* any) {
	_ASSERT(any != NULL);
	_ASSERT(any->data != NULL);
	_ASSERT(any->type == _ARRAY_ANY_TYPE);
	_any_array_t* array = (_any_array_t*)any->data;
	for (int64_t i = 0; i < array->size; i++) {
		_any_destroy(array->data[i]);
	}
	_CLEAR(*array);
}

int64_t _any_map_key_count(_any_t const* any) {
	_ASSERT(any != NULL);
	_ASSERT(any->data != NULL);
	_ASSERT(any->type == _MAP_ANY_TYPE);
	_any_keyvalue_array_t* map = (_any_keyvalue_array_t*)any->data;
	return map->size;
}

char const* _any_map_key_item(_any_t const* any, int64_t index) {
	_ASSERT(any != NULL);
	_ASSERT(any->data != NULL);
	_ASSERT(any->type == _MAP_ANY_TYPE);
	_ASSERT(index >= 0);
	_any_keyvalue_array_t* map = (_any_keyvalue_array_t*)any->data;
	_ASSERT(index < map->size);
	return map->data[index].key;
}

void _any_map_set(_any_t* any, char const* key, _any_t const* value) {
	_ASSERT(any != NULL);
	_ASSERT(any->data != NULL);
	_ASSERT(any->type == _MAP_ANY_TYPE);
	_any_keyvalue_array_t* map = (_any_keyvalue_array_t*)any->data;
	for (int64_t i = 0; i < map->size; i++) {
		if (strcmp(map->data[i].key, key) == 0) {
			_any_destroy(map->data[i].value);
			map->data[i].value = _any_clone(value);
			return;
		}
	}
	_any_keyvalue_t pair = {
		.key = _string_clone(key, strlen(key)),
		.value = _any_clone(value)
	};
	_PUSH(*map, pair);
}

bool _any_map_has(_any_t* any, char const* key) {
	_ASSERT(any != NULL);
	_ASSERT(any->data != NULL);
	_ASSERT(any->type == _MAP_ANY_TYPE);
	_any_keyvalue_array_t* map = (_any_keyvalue_array_t*)any->data;
	for (int64_t i = 0; i < map->size; i++) {
		if (strcmp(map->data[i].key, key) == 0) {
			return true;
		}
	}
	return false;
}

_any_t* _any_map_get(_any_t* any, char const* key) {
	_ASSERT(any != NULL);
	_ASSERT(any->data != NULL);
	_ASSERT(any->type == _MAP_ANY_TYPE);
	_any_keyvalue_array_t* map = (_any_keyvalue_array_t*)any->data;
	int64_t index = -1;
	for (int64_t i = 0; i < map->size; i++) {
		if (strcmp(map->data[i].key, key) == 0) {
			return map->data[i].value;
		}
	}
	_ABORT("key \"%s\" not found", key);
}

void _any_map_clear(_any_t* any) {
	_ASSERT(any != NULL);
	_ASSERT(any->data != NULL);
	_ASSERT(any->type == _MAP_ANY_TYPE);
	_any_keyvalue_array_t* map = (_any_keyvalue_array_t*)any->data;
	for (int64_t i = 0; i < map->size; i++) {
		_FREE(map->data[i].key);
		_any_destroy(map->data[i].value);
	}
	_CLEAR(*map);
}

void _any_entries(_any_t const* any, _any_entry_t const*** entries, int64_t* size) {
	;
}