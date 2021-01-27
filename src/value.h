#ifndef _LIBEXT_VALUE_H
#define _LIBEXT_VALUE_H

typedef enum {
	_NULL_VALUE,
	_STRING_VALUE,
	_NUMBER_VALUE,
	_DATE_VALUE,
	_ARRAY_VALUE,
	_MAP_VALUE
} _VALUE;

typedef struct _value_t _value_t;

#ifdef __cplusplus
extern "C" {
#endif

_value_t* _value_clone(_value_t const*);
_value_t* _value_create_null(void);
_value_t* _value_create_string(char const*);
_value_t* _value_create_number(double);
_value_t* _value_create_date(double);
_value_t* _value_create_array(void);
_value_t* _value_create_map(void);
_VALUE _value_type(_value_t const*);
void _value_set_null(_value_t*);
void _value_set_string(_value_t*,char const*);
void _value_set_number(_value_t*,double);
void _value_set_date(_value_t*,double);
void _value_set_array(_value_t*);
void _value_set_map(_value_t*);
char const* _value_string(_value_t const*);
double _value_number(_value_t const*);
double _value_date(_value_t const*);
int64_t _value_array_count(_value_t const*);
_value_t* _value_array_item(_value_t const*,int64_t);
void _value_array_push(_value_t*,_value_t const*);
void _value_array_unshift(_value_t*,_value_t const*);
_value_t* _value_array_pop(_value_t*);
_value_t* _value_array_shift(_value_t*);
void _value_array_clear(_value_t*);
int64_t _value_map_keys_count(_value_t const*);
char const* _value_map_key_item(_value_t const*,int64_t);
void _value_map_set(_value_t*,char const*,_value_t*);
bool _value_map_has(_value_t*,char const*);
_value_t* _value_map_get(_value_t*,char const*);
void _value_map_clear(_value_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBEXT_VALUE_H */
