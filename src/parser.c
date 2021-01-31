#include <ctype.h>
#include <libext/string.h>
#include <libext/parser.h>
#include <libext/MACRO.h>

typedef struct _parser_t {
	char const* data;
	int64_t size;
	int64_t position;
} _parser_t;

_parser_t* _parser_create(char const* data, int64_t size) {
	_ASSERT(data != NULL);
	return _NEW(_parser_t, {
		.data = data,
		.size = size,
		.position = 0
	});
}

void _parser_destroy(_parser_t* parser) {
	_ASSERT(parser != NULL);
	_FREE(parser);
}

int64_t _parser_position(_parser_t* parser) {
	_ASSERT(parser != NULL);
	return parser->position;
}

bool _parser_end(_parser_t* parser)  {
	_ASSERT(parser != NULL);
	return (parser->position >= parser->size);
}

bool _parser_seek(_parser_t* parser, int64_t delta) {
	_ASSERT(parser != NULL);

	if (delta < 0) {
		if ((delta * -1) <= parser->position) {
			parser->position += delta;
			return true;
		} else {
			return false;
		}
	} else if (delta > 0) {
		if (delta <= (parser->size - parser->position)) {
			parser->position += delta;
			return true;
		} else {
			return false;
		}
	} else {
		return true;
	}
}

bool _parser_read_line(_parser_t* parser, char const** out_pchar, int64_t* out_len) {
	_ASSERT(parser != NULL);

	int64_t read = 0;
	for (;;) {
		if ((parser->position + read) >= parser->size) {
			return false;
		} else if (parser->data[(parser->position + read)] == '\n') {
			if (out_pchar != NULL) {
				_ASSERT(out_len != NULL);
				(*out_pchar) = (parser->data + parser->position);
				(*out_len) = (read + 1);
			}
			parser->position += (read + 1);
			return true;
		} else {
			read += 1;
		}
	}
}

bool _parser_read_until(_parser_t* parser, char const* pchar, int64_t len, char const** out_pchar, int64_t* out_len) {
	_ASSERT(parser != NULL);

	int64_t read = 0;
	for (;;) {
		if ((parser->position + read) >= (parser->size - len)) {
			return false;
		} else if (strncmp((parser->data + parser->position + read), pchar, len) == 0) {
			if (out_pchar != NULL) {
				_ASSERT(out_len != NULL);
				(*out_pchar) = (parser->data + parser->position);
				(*out_len) = read;
			}
			parser->position += read;
			return true;
		} else {
			read += 1;
		}
	}
}

bool _parser_read_exact(_parser_t* parser, char const* pchar, int64_t len) {
	_ASSERT(parser != NULL);

	if ((parser->size - parser->position) >= len) {
		if (strncmp((parser->data + parser->position), pchar, len) == 0) {
			parser->position += len;
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool _parser_read_string(_parser_t* parser, char const** out_pchar, int64_t* out_len) {
	_ASSERT(parser != NULL);

	int64_t read = 0;
	for (;;) {
		if ((parser->position + read) >= parser->size) {
			break;
		} else if (isspace(parser->data[parser->position + read])) {
			break;
		} else {
			read += 1;
		}
	}

	if (read > 0) {
		if (out_pchar != NULL) {
			_ASSERT(out_len != NULL);
			(*out_pchar) = (parser->data + parser->position);
			(*out_len) = read;
		}
		parser->position += read;
		return true;
	} else {
		return false;
	}
}

bool _parser_read_whitespace(_parser_t* parser, int64_t* out_read)  {
	_ASSERT(parser != NULL);

	int64_t read = 0;
	for (;;) {
		if ((parser->position + read) >= parser->size) {
			break;
		} else if (!isspace(parser->data[parser->position + read])) {
			break;
		} else {
			read += 1;
		}
	}

	if (read > 0) {
		if (out_read != NULL) {
			(*out_read) = read;
		}
		parser->position += read;
		return true;
	} else {
		return false;
	}
}

bool _parser_read_int8(_parser_t* parser, bool spaces, int8_t* out) {
	_ASSERT(parser != NULL);
	_ASSERT(out != NULL);

	if (!_parser_end(parser)) {
		int64_t read = _string_read_int8(
			(parser->data + parser->position),
			(parser->size - parser->position),
			spaces,
			out
		);
		if (read > 0) {
			parser->position += read;
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool _parser_read_int16(_parser_t* parser, bool spaces, int16_t* out) {
	_ASSERT(parser != NULL);
	_ASSERT(out != NULL);

	if (!_parser_end(parser)) {
		int64_t read = _string_read_int16(
			(parser->data + parser->position),
			(parser->size - parser->position),
			spaces,
			out
		);
		if (read > 0) {
			parser->position += read;
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool _parser_read_int32(_parser_t* parser, bool spaces, int32_t* out) {
	_ASSERT(parser != NULL);
	_ASSERT(out != NULL);

	if (!_parser_end(parser)) {
		int64_t read = _string_read_int32(
			(parser->data + parser->position),
			(parser->size - parser->position),
			spaces,
			out
		);
		if (read > 0) {
			parser->position += read;
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool _parser_read_int64(_parser_t* parser, bool spaces, int64_t* out) {
	_ASSERT(parser != NULL);
	_ASSERT(out != NULL);

	if (!_parser_end(parser)) {
		int64_t read = _string_read_int64(
			(parser->data + parser->position),
			(parser->size - parser->position),
			spaces,
			out
		);
		if (read > 0) {
			parser->position += read;
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}
