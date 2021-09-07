#include <assert.h>
#include <libcore/value.h>
#include <libcore/string.h>
#include <libcore/MACRO.h>

typedef struct _value_entry_t {
	_VALUE_ENTRY_TYPE type;
	char* path;
	char* string;
	double number;
	double date;
} _value_entry_t;

typedef struct _value_t {
	_VALUE_TYPE type;
	_value_t* owner;
	void* data;
} _value_t;

typedef struct _value_array_t {
	_value_t** data;
	int64_t size;
	int64_t capacity;
} _value_array_t;

typedef struct _value_keyvalue_t {
	char* key;
	_value_t* value;
} _value_keyvalue_t;

typedef struct _value_keyvalue_array_t {
	_value_keyvalue_t* data;
	int64_t size;
	int64_t capacity;
} _value_keyvalue_array_t;

_value_entry_t* _value_entry_create_null(char const* path, int64_t len) {
	assert(path != NULL);
	return _NEW(_value_entry_t, {
		.type = _NULL_VALUE_ENTRY_TYPE,
		.path = _string_clone(path, len)
	});
}

_value_entry_t* _value_entry_create_string(char const* path, int64_t len, char const* string, int64_t len2) {
	assert(path != NULL);
	assert(string != NULL);
	return _NEW(_value_entry_t, {
		.type = _STRING_VALUE_ENTRY_TYPE,
		.path = _string_clone(path, len),
		.string = ((len > 0) ? _string_clone(string, len2) : (char*)_ALLOC(char, 1))
	});
}

_value_entry_t* _value_entry_create_number(char const* path, int64_t len, double number) {
	assert(path != NULL);
	return _NEW(_value_entry_t, {
		.type = _NUMBER_VALUE_ENTRY_TYPE,
		.path = _string_clone(path, len),
		.number = number
	});
}

_value_entry_t* _value_entry_create_date(char const* path, int64_t len, double date) {
	assert(path != NULL);
	return _NEW(_value_entry_t, {
		.type = _DATE_VALUE_ENTRY_TYPE,
		.path = _string_clone(path, len),
		.date = date
	});
}

_VALUE_ENTRY_TYPE _value_entry_type(_value_entry_t* entry) {
	assert(entry != NULL);
	return entry->type;
}

char const* _value_entry_path(_value_entry_t* entry) {
	assert(entry != NULL);
	return entry->path;
}

char const* _value_entry_string(_value_entry_t* entry) {
	assert(entry != NULL);
	assert(entry->type == _STRING_VALUE_ENTRY_TYPE);
	return entry->string;
}

double _value_entry_number(_value_entry_t* entry) {
	assert(entry != NULL);
	assert(entry->type == _NUMBER_VALUE_ENTRY_TYPE);
	return entry->number;
}

double _value_entry_date(_value_entry_t* entry) {
	assert(entry != NULL);
	assert(entry->type == _DATE_VALUE_ENTRY_TYPE);
	return entry->date;
}

void _value_entry_destroy(_value_entry_t* entry) {
	assert(entry != NULL);
	_FREE(entry->string);
	_FREE(entry);
}

_value_t* _value_clone(_value_t const* value) {
	assert(value != NULL);
	if (value->type == _NULL_VALUE_TYPE) {
		return _NEW(_value_t, {
			.type = _NULL_VALUE_TYPE
		});
	} else if (value->type == _STRING_VALUE_TYPE) {
		return _NEW(_value_t, {
			.type = _STRING_VALUE_TYPE,
			.data = (void*)_string_clone(
				(char const*)value->data,
				strlen((char const*)value->data)
			)
		});
	} else if (value->type == _NUMBER_VALUE_TYPE) {
		return _NEW(_value_t, {
			.type = _NUMBER_VALUE_TYPE,
			.data = (void*)_NEW(double, *(double*)value->data)
		});
	} else if (value->type == _DATE_VALUE_TYPE) {
		return _NEW(_value_t, {
			.type = _DATE_VALUE_TYPE,
			.data = (void*)_NEW(double, *(double*)value->data)
		});
	} else if (value->type == _BOOL_VALUE_TYPE) {
		return _NEW(_value_t, {
			.type = _BOOL_VALUE_TYPE,
			.data = (void*)_NEW(int, (*(int*)value->data) == 1)
		});
	} else if (value->type == _ARRAY_VALUE_TYPE) {
		_value_array_t* src = (_value_array_t*)_NOTNULL(value->data);
		_value_array_t* dst = _NEW(_value_array_t, {});
		_RESERVE(*dst, src->size);
		for (int64_t i = 0; i < src->size; i++) {
			assert(src->data[i] != NULL);
			_value_t* copy = _value_clone(src->data[i]);
			_PUSH(*dst, copy);
		}
		return _NEW(_value_t, {
			.type = _ARRAY_VALUE_TYPE,
			.data = (void*)dst
		});
	} else if (value->type == _MAP_VALUE_TYPE) {
		_value_keyvalue_array_t* src = (_value_keyvalue_array_t*)_NOTNULL(value->data);
		_value_keyvalue_array_t* dst = _NEW(_value_keyvalue_array_t, {});
		_RESERVE(*dst, src->size);
		for (int64_t i = 0; i < src->size; i++) {
			assert(src->data[i].key != NULL);
			assert(src->data[i].value != NULL);
			_value_keyvalue_t pair = {
				.key = _string_clone(src->data[i].key, strlen(src->data[i].key)),
				.value = _value_clone(src->data[i].value)
			};
			_PUSH(*dst, pair);
		}
		return _NEW(_value_t, {
			.type = _MAP_VALUE_TYPE,
			.data = (void*)dst
		});
	} else {
		_ABORT("%s\n", "unexpected value type");
	}
}

_value_t* _value_create_null(void) {
	return _NEW(_value_t, {
		.type = _NULL_VALUE_TYPE
	});
}

_value_t* _value_create_string(char const* string, int64_t len) {
	return _NEW(_value_t, {
		.type = _STRING_VALUE_TYPE,
		.data = (void*)_string_clone(string, len)
	});
}

_value_t* _value_create_number(double number) {
	return _NEW(_value_t, {
		.type = _NUMBER_VALUE_TYPE,
		.data = (void*)_NEW(double, number)
	});
}

_value_t* _value_create_date(double date) {
	return _NEW(_value_t, {
		.type = _DATE_VALUE_TYPE,
		.data = (void*)_NEW(double, date)
	});
}

_value_t* _value_create_bool(bool val) {
	return _NEW(_value_t, {
		.type = _BOOL_VALUE_TYPE,
		.data = (void*)_NEW(int, (val ? 1 : 0))
	});
}

_value_t* _value_create_array(void) {
	return _NEW(_value_t, {
		.type = _ARRAY_VALUE_TYPE,
		.data = (void*)_NEW(_value_array_t, {})
	});
}

_value_t* _value_create_map(void) {
	return _NEW(_value_t, {
		.type = _MAP_VALUE_TYPE,
		.data = (void*)_NEW(_value_keyvalue_array_t, {})
	});
}

_value_t* _value_create_entries(_value_entry_t const** entries, int64_t size) {
	return NULL;
}

void _value_destroy(_value_t* value) {
	assert(value != NULL);
	_value_set_null(value);
	_FREE(value);
}

void _value_set_null(_value_t* value) {
	assert(value != NULL);
	if (value->type == _ARRAY_VALUE_TYPE) {
		_value_array_t* array = (_value_array_t*)_NOTNULL(value->data);
		for (int64_t i = 0; i < array->size; i++) {
			assert(array->data[i] != NULL);
			_value_destroy(array->data[i]);
		}
		_FREE(array->data);
	} else if (value->type == _MAP_VALUE_TYPE) {
		_value_keyvalue_array_t* array = (_value_keyvalue_array_t*)_NOTNULL(value->data);
		for (int64_t i = 0; i < array->size; i++) {
			assert(array->data[i].key != NULL);
			assert(array->data[i].value != NULL);
			_FREE(array->data[i].key);
			_value_destroy(array->data[i].value);
		}
		_FREE(array->data);
	}
	_FREE(value->data);
	value->type = _NULL_VALUE_TYPE;
}

void _value_set_string(_value_t* value, char const* string, int64_t len) {
	assert(value != NULL);
	_value_set_null(value);
	value->type = _STRING_VALUE_TYPE;
	value->data = (void*)_string_clone(string, len);
}

void _value_set_number(_value_t* value, double number) {
	assert(value != NULL);
	_value_set_null(value);
	value->type = _NUMBER_VALUE_TYPE;
	value->data = (void*)_NEW(double, number);
}

void _value_set_date(_value_t* value, double date) {
	assert(value != NULL);
	_value_set_null(value);
	value->type = _DATE_VALUE_TYPE;
	value->data = (void*)_NEW(double, date);
}

void _value_set_bool(_value_t* value, bool val) {
	assert(value != NULL);
	_value_set_null(value);
	value->type = _BOOL_VALUE_TYPE;
	value->data = (void*)_NEW(int, (val ? 1 : 0));
}

void _value_set_array(_value_t* value) {
	assert(value != NULL);
	_value_set_null(value);
	value->type = _ARRAY_VALUE_TYPE;
	value->data = (void*)_NEW(_value_array_t, {});
}

void _value_set_map(_value_t* value) {
	assert(value != NULL);
	_value_set_null(value);
	value->type = _MAP_VALUE_TYPE;
	value->data = (void*)_NEW(_value_keyvalue_array_t, {});
}

bool _value_detach(_value_t* value) {
	assert(value != NULL);
	if (value->owner != NULL) {
		if (value->owner->type == _ARRAY_VALUE_TYPE) {
			_value_array_t* array = (_value_array_t*)_NOTNULL(value->owner->data);
			for (int64_t i = 0; i < array->size; i++) {
				if (array->data[i] == value) {
					_REMOVE_INDEX(*array, i);
					break;
				}
			}
		} else if (value->owner->type == _MAP_VALUE_TYPE) {
			_value_keyvalue_array_t* map = (_value_keyvalue_array_t*)_NOTNULL(value->owner->data);
			for (int64_t i = 0; i < map->size; i++) {
				if (map->data[i].value == value) {
					_REMOVE_INDEX(*map, i);
					break;
				}
			}
		}
		value->owner = NULL;
		return true;
	} else {
		return false;
	}
}

_VALUE_TYPE _value_type(_value_t const* value) {
	assert(value != NULL);
	return value->type;
}

char const* _value_string(_value_t const* value) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _STRING_VALUE_TYPE);
	return (char const*)value->data;
}

double _value_number(_value_t const* value) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _NUMBER_VALUE_TYPE);
	return *(double*)value->data;
}

double _value_date(_value_t const* value) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _DATE_VALUE_TYPE);
	return *(double*)value->data;
}

bool _value_bool(_value_t const* value) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _BOOL_VALUE_TYPE);
	return ((*(int*)value->data) == 1);
}

int64_t _value_array_count(_value_t const* value) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _ARRAY_VALUE_TYPE);
	return ((_value_array_t*)value->data)->size;
}

_value_t* _value_array_get(_value_t const* value, int64_t index) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _ARRAY_VALUE_TYPE);
	assert(index >= 0);
	_value_array_t* array = (_value_array_t*)_NOTNULL(value->data);
	assert(index < array->size);
	return _NOTNULL(array->data[index]);
}

_value_t* _value_array_move(_value_t const* value, int64_t index) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _ARRAY_VALUE_TYPE);
	assert(index >= 0);
	_value_array_t* array = (_value_array_t*)_NOTNULL(value->data);
	assert(index < array->size);
	_value_t* ret = array->data[index];
	_REMOVE_INDEX(*array, index);
	return _NOTNULL(ret);
}

void _value_array_remove(_value_t* value, int64_t index) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _ARRAY_VALUE_TYPE);
	assert(index >= 0);
	_value_array_t* array = (_value_array_t*)_NOTNULL(value->data);
	assert(index < array->size);
	_REMOVE_INDEX(*array, index);
}

void _value_array_set_copy(_value_t* value, int64_t index, _value_t const* item) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _ARRAY_VALUE_TYPE);
	assert(index >= 0);
	assert(item != NULL);
	_value_array_t* array = (_value_array_t*)_NOTNULL(value->data);
	assert(index < array->size);
	_value_destroy(_NOTNULL(array->data[index]));
	array->data[index] = _value_clone(item);
}

void _value_array_set_move(_value_t* value, int64_t index, _value_t* item) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _ARRAY_VALUE_TYPE);
	assert(index >= 0);
	assert(item != NULL);
	_value_array_t* array = (_value_array_t*)_NOTNULL(value->data);
	assert(index < array->size);
	_value_destroy(_NOTNULL(array->data[index]));
	array->data[index] = item;
}

void _value_array_push_copy(_value_t* value, _value_t const* item) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _ARRAY_VALUE_TYPE);
	assert(item != NULL);
	_value_array_t* array = (_value_array_t*)_NOTNULL(value->data);
	_value_t* copy = _value_clone(item);
	_PUSH(*array, copy);
}

void _value_array_push_move(_value_t* value, _value_t* item) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _ARRAY_VALUE_TYPE);
	assert(item != NULL);
	_value_array_t* array = (_value_array_t*)_NOTNULL(value->data);
	_PUSH(*array, item);
}

void _value_array_unshift_copy(_value_t* value, _value_t const* item) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _ARRAY_VALUE_TYPE);
	_value_array_t* array = (_value_array_t*)_NOTNULL(value->data);
	_value_t* copy = _value_clone(item);
	_UNSHIFT(*array, copy);
}

void _value_array_unshift_move(_value_t* value, _value_t* item) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _ARRAY_VALUE_TYPE);
	_value_array_t* array = (_value_array_t*)_NOTNULL(value->data);
	_UNSHIFT(*array, item);
}

_value_t* _value_array_pop(_value_t* value) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _ARRAY_VALUE_TYPE);
	_value_array_t* array = (_value_array_t*)_NOTNULL(value->data);
	_value_t* ret = _POP(*array);
	return _NOTNULL(ret);
}

_value_t* _value_array_shift(_value_t* value) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _ARRAY_VALUE_TYPE);
	_value_array_t* array = (_value_array_t*)_NOTNULL(value->data);
	_value_t* ret = _SHIFT(*array);
	return _NOTNULL(ret);
}

void _value_array_clear(_value_t* value) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _ARRAY_VALUE_TYPE);
	_value_array_t* array = (_value_array_t*)_NOTNULL(value->data);
	for (int64_t i = 0; i < array->size; i++) {
		_value_destroy(_NOTNULL(array->data[i]));
	}
	_CLEAR(*array);
}

int64_t _value_map_key_count(_value_t const* value) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _MAP_VALUE_TYPE);
	_value_keyvalue_array_t* map = (_value_keyvalue_array_t*)_NOTNULL(value->data);
	return map->size;
}

char const* _value_map_key_item(_value_t const* value, int64_t index) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _MAP_VALUE_TYPE);
	assert(index >= 0);
	_value_keyvalue_array_t* map = (_value_keyvalue_array_t*)_NOTNULL(value->data);
	assert(index < map->size);
	return _NOTNULL(map->data[index].key);
}

void _value_map_set_copy(_value_t* value, char const* key, int64_t len, _value_t const* val) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _MAP_VALUE_TYPE);
	_value_keyvalue_array_t* map = (_value_keyvalue_array_t*)_NOTNULL(value->data);
	for (int64_t i = 0; i < map->size; i++) {
		if (_string_compare(map->data[i].key, strlen(map->data[i].key), key, len)) {
			_value_destroy(map->data[i].value);
			map->data[i].value = _value_clone(val);
			return;
		}
	}
	_value_keyvalue_t pair = {
		.key = _string_clone(key, len),
		.value = _value_clone(val)
	};
	_PUSH(*map, pair);
}

void _value_map_set_move(_value_t* value, char const* key, int64_t len, _value_t* val) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _MAP_VALUE_TYPE);
	_value_keyvalue_array_t* map = (_value_keyvalue_array_t*)_NOTNULL(value->data);
	for (int64_t i = 0; i < map->size; i++) {
		if (_string_compare(map->data[i].key, strlen(map->data[i].key), key, len)) {
			_value_destroy(map->data[i].value);
			map->data[i].value = val;
			return;
		}
	}
	_value_keyvalue_t pair = {
		.key = _string_clone(key, len),
		.value = val
	};
	_PUSH(*map, pair);
}

bool _value_map_has(_value_t* value, char const* key, int64_t len) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _MAP_VALUE_TYPE);
	_value_keyvalue_array_t* map = (_value_keyvalue_array_t*)_NOTNULL(value->data);
	for (int64_t i = 0; i < map->size; i++) {
		if (_string_compare(map->data[i].key, strlen(map->data[i].key), key, len)) {
			return true;
		}
	}
	return false;
}

_value_t* _value_map_get(_value_t* value, char const* key, int64_t len) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _MAP_VALUE_TYPE);
	_value_keyvalue_array_t* map = (_value_keyvalue_array_t*)_NOTNULL(value->data);
	for (int64_t i = 0; i < map->size; i++) {
		if (_string_compare(map->data[i].key, strlen(map->data[i].key), key, len)) {
			return map->data[i].value;
		}
	}
	_ABORT("key \"%s\" not found\n", key);
}

_value_t* _value_map_move(_value_t* value, char const* key, int64_t len) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _MAP_VALUE_TYPE);
	_value_keyvalue_array_t* map = (_value_keyvalue_array_t*)_NOTNULL(value->data);
	for (int64_t i = 0; i < map->size; i++) {
		if (_string_compare(map->data[i].key, strlen(map->data[i].key), key, len)) {
			_value_t* ret = map->data[i].value;
			_FREE(map->data[i].key);
			_REMOVE_INDEX(*map, i);
			return ret;
		}
	}
	_ABORT("key \"%s\" not found\n", key);
}

void _value_map_clear(_value_t* value) {
	assert(value != NULL);
	assert(value->data != NULL);
	assert(value->type == _MAP_VALUE_TYPE);
	_value_keyvalue_array_t* map = (_value_keyvalue_array_t*)_NOTNULL(value->data);
	for (int64_t i = 0; i < map->size; i++) {
		_FREE(map->data[i].key);
		_value_destroy(map->data[i].value);
	}
	_CLEAR(*map);
}

void _value_entries(_value_t const* value, _value_entry_t const*** entries, int64_t* size) {
	;
}