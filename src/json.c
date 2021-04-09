#include <libcore/json.h>
#include <libcore/parser.h>
#include <libcore/MACRO.h>

typedef struct __string_t {
	char const* data;
	int64_t size;
} __string_t;

typedef struct __keyvalue_t {
	__string_t key;
	_value_t* value;
} __keyvalue_t;

static bool __parse_keyvalue(_parser_t* parser, __keyvalue_t* keyvalue, _status_t* status) {
	_ASSERT(parser != NULL);
	_ASSERT(keyvalue != NULL);
	_ASSERT(status != NULL);
	if (_parser_read_exact(parser, "\"", 1)) {
		if (_parser_read_quoted(parser, "\"", 1, &keyvalue->key.data, &keyvalue->key.size)) {
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
	_ASSERT(parser != NULL);
	_ASSERT(value != NULL);
	_ASSERT(status != NULL);
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
	_ASSERT(parser != NULL);
	_ASSERT(value != NULL);
	_ASSERT(status != NULL);
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

static bool __parse_value(_parser_t* parser, _value_t** value, _status_t* status) {
	_ASSERT(parser != NULL);
	_ASSERT(value != NULL);
	_ASSERT(status != NULL);
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
			__string_t string = {};
			if (_parser_read_quoted(parser, "\"", 1, &string.data, &string.size)) {
				(*value) = _value_create_string(string.data, string.size);
				return true;
			} else {
				_status_set(status, _FAILURE_STATUS_TYPE, "unexpected end of data");
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
	_ASSERT(string != NULL);
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
	_ASSERT(value != NULL);
	_ASSERT(status != NULL);
}