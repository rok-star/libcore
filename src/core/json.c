#include <assert.h>
#include <libcore/core/json.h>
#include <libcore/core/parser.h>
#include <libcore/core/MACRO.h>

typedef struct __string_const_t {
	char const* data;
	int64_t size;
} __string_const_t;

typedef struct __string_t {
	char* data;
	int64_t size;
} __string_t;

typedef struct __keyvalue_t {
	__string_const_t key;
	_value_t* value;
} __keyvalue_t;

static bool __parse_value(_parser_t*,_value_t**,_status_t*);

static bool __parse_keyvalue(_parser_t* parser, __keyvalue_t* keyvalue, _status_t* status) {
	assert(parser != NULL);
	assert(keyvalue != NULL);
	assert(status != NULL);
	if (_parser_read_exact(parser, "\"", 1)) {
		if (_parser_read_quoted(parser, '"', &keyvalue->key.data, &keyvalue->key.size)) {
			_parser_skip_whitespace(parser);
			if (_parser_read_exact(parser, ":", 1)) {
				if (!__parse_value(parser, &keyvalue->value, status)) {
					return false;
				}
			} else {
				_status_set(status, _FAILURE_STATUS_TYPE, "unexpected token");
				return false;
			}
		} else {
			_status_set(status, _FAILURE_STATUS_TYPE, "unexpected end of data");
			return false;
		}
	} else {
		_ABORT("keyvalue expected\n");
	}
	return true;
}

static bool __parse_object(_parser_t* parser, _value_t** value, _status_t* status) {
	assert(parser != NULL);
	assert(value != NULL);
	assert(status != NULL);
	(*value) = _value_create_map();
	if (_parser_read_exact(parser, "{", 1)) {
		_parser_skip_whitespace(parser);
		while (!_parser_read_exact(parser, "}", 1)) {
			if (_parser_peek_exact(parser, "\"", 1)) {
				__keyvalue_t keyvalue = {};
				if (__parse_keyvalue(parser, &keyvalue, status)) {
					_value_map_set_move((*value), keyvalue.key.data, keyvalue.key.size, keyvalue.value);
					_parser_skip_whitespace(parser);
					_parser_read_exact(parser, ",", 1);
					_parser_skip_whitespace(parser);
				} else {
					_value_destroy(*value);
					return false;
				}
			} else {
				_status_set(status, _FAILURE_STATUS_TYPE, "unexpected token");
				_value_destroy(*value);
				return false;
			}
		}
	} else {
		_ABORT("object expected\n");
	}
	return true;
}

static bool __parse_array(_parser_t* parser, _value_t** value, _status_t* status) {
	assert(parser != NULL);
	assert(value != NULL);
	assert(status != NULL);
	(*value) = _value_create_array();
	if (_parser_read_exact(parser, "[", 1)) {
		while (!_parser_read_exact(parser, "]", 1)) {
			_value_t* item = NULL;
			if (__parse_value(parser, &item, status)) {
				_value_array_push_move((*value), item);
				_parser_skip_whitespace(parser);
				_parser_read_exact(parser, ",", 1);
				_parser_skip_whitespace(parser);
			} else {
				_value_destroy(*value);
				return false;
			}
		}
	} else {
		_ABORT("array expected\n");
	}
	return true;
}

static bool __parse_string(_parser_t* parser, _value_t** value, _status_t* status) {
	assert(parser != NULL);
	assert(value != NULL);
	assert(status != NULL);
	if (_parser_peek_exact(parser, "\"", 1)) {
		__string_const_t src = {};
		if (_parser_read_quoted(parser, '"', &src.data, &src.size)) {
			assert(src.size >= 2);
			assert(src.data[0] == '"');
			assert(src.data[src.size - 1] == '"');
			__string_t dst = {
				.data = _ALLOC(char, (src.size - 1)),
				.size = (src.size - 1)
			};
			int64_t pos = 0;
			bool esc = false;
			for (int64_t i = 1; i < (src.size - 1); i++) {
				char char_ = src.data[i];
				if (esc) {
					if (char_ == 'b') {
						dst.data[pos++] = '\b';
					} else if (char_ == 'f') {
						dst.data[pos++] = '\f';
					} else if (char_ == 'n') {
						dst.data[pos++] = '\n';
					} else if (char_ == 'r') {
						dst.data[pos++] = '\r';
					} else if (char_ == 't') {
						dst.data[pos++] = '\t';
					} else if (char_ == '\\') {
						dst.data[pos++] = '\\';
					}
				} else {
					if (char_ == '\\') {
						esc = true;
					} else {
						dst.data[pos++] = char_;
						esc = false;
					}
				}
			}
			(*value) = _value_create_string(dst.data, dst.size);
			_FREE(dst.data);
			return true;
		} else {
			_status_set(status, _FAILURE_STATUS_TYPE, "unexpected end of data");
			return false;
		}
	}
	return true;
}

static bool __parse_value(_parser_t* parser, _value_t** value, _status_t* status) {
	assert(parser != NULL);
	assert(value != NULL);
	assert(status != NULL);
	_parser_skip_whitespace(parser);
	if (!_parser_end(parser)) {
		if (_parser_peek_exact(parser, "{", 1)) {
			if (!__parse_object(parser, value, status)) {
				return false;
			}
		} else if (_parser_peek_exact(parser, "[", 1)) {
			if (!__parse_array(parser, value, status)) {
				return false;
			}
		} else if (_parser_peek_exact(parser, "\"", 1)) {
			if (!__parse_string(parser, value, status)) {
				return false;
			}
		} else {
			(*value) = _value_create_number(777);
			return true;
		}
	} else {
		_status_set(status, _FAILURE_STATUS_TYPE, "unexpected end of JSON");
		return false;
	}
	return true;
}

_value_t* _json_parse(char const* string, int64_t len, _status_t* status) {
	assert(string != NULL);
	_parser_t* parser = _parser_create(string, len);
	_value_t* value = NULL;
	if (__parse_value(parser, &value, status)) {
		_parser_destroy(parser);
		_status_set(status, _SUCCESS_STATUS_TYPE, NULL);
		return value;
	} else {
		_parser_destroy(parser);
		return NULL;
	}
}

char const* _json_stringify(_value_t const* value) {
	assert(value != NULL);
	return NULL;
}