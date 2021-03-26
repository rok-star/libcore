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

_any_entry_t* _any_entry_create_null(void) {
	return _NEW(_any_t, { .type = _NULL_ANY_TYPE });
}

_any_entry_t* _any_entry_create_string(char const* string, int64_t len) {
	_ASSERT(string != NULL);
	return _NEW(_any_t, {
		.type = _STRING_ANY_TYPE,
		.data = (char*)((len > 0) ? _string_clone(string, len) : _ALLOC(char, 1))
	});
}

_any_entry_t* _any_entry_create_number(double) {

}

_any_entry_t* _any_entry_create_date(double) {

}

_ANY_ENTRY_TYPE _any_entry_type(_any_entry_t*) {

}

char const* _any_entry_path(_any_entry_t*) {

}

char const* _any_entry_string(_any_entry_t*) {

}

double _any_entry_number(_any_entry_t*) {

}

double _any_entry_date(_any_entry_t*) {

}

void _any_entry_destroy(_any_entry_t*) {

}

_any_t* _any_clone(_any_t const*) {

}

_any_t* _any_create_null(void) {

}

_any_t* _any_create_string(char const*) {

}

_any_t* _any_create_number(double) {

}

_any_t* _any_create_date(double) {

}

_any_t* _any_create_array(void) {

}

_any_t* _any_create_map(void) {

}

_any_t* _any_create_entries(_any_entry_t const**,int64_t) {

}

void _any_destroy(_any_t*) {

}

void _any_set_null(_any_t*) {

}

void _any_set_string(_any_t*,char const*) {

}

void _any_set_number(_any_t*,double) {

}

void _any_set_date(_any_t*,double) {

}

void _any_set_array(_any_t*) {

}

void _any_set_map(_any_t*) {

}

_ANY_TYPE _any_type(_any_t const*) {

}

char const* _any_string(_any_t const*) {

}

double _any_number(_any_t const*) {

}

double _any_date(_any_t const*) {

}

int64_t _any_array_count(_any_t const*) {

}

_any_t* _any_array_item(_any_t const*,int64_t) {

}

void _any_array_push(_any_t*,_any_t const*) {

}

void _any_array_unshift(_any_t*,_any_t const*) {

}

_any_t* _any_array_pop(_any_t*) {

}

_any_t* _any_array_shift(_any_t*) {

}

void _any_array_clear(_any_t*) {

}

int64_t _any_map_key_count(_any_t const*) {

}

char const* _any_map_key_item(_any_t const*,int64_t) {

}

void _any_map_set(_any_t*,char const*,_any_t*) {

}

bool _any_map_has(_any_t*,char const*) {

}

_any_t* _any_map_get(_any_t*,char const*) {

}

void _any_map_clear(_any_t*) {

}

void _any_entries(_any_t const*, _any_entry_t const***,int64_t*) {

}