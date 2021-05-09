#ifndef _LIBCORE_VALUE_H
#define _LIBCORE_VALUE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	_NULL_VALUE_ENTRY_TYPE,
	_STRING_VALUE_ENTRY_TYPE,
	_NUMBER_VALUE_ENTRY_TYPE,
	_DATE_VALUE_ENTRY_TYPE
} _VALUE_ENTRY_TYPE;

typedef struct _value_entry_t _value_entry_t;

typedef enum {
	_NULL_VALUE_TYPE,
	_STRING_VALUE_TYPE,
	_NUMBER_VALUE_TYPE,
	_DATE_VALUE_TYPE,
	_BOOL_VALUE_TYPE,
	_ARRAY_VALUE_TYPE,
	_MAP_VALUE_TYPE
} _VALUE_TYPE;

typedef struct _value_t _value_t;

#ifdef __cplusplus
extern "C" {
#endif

_value_entry_t* _value_entry_create_null(char const*,int64_t);
_value_entry_t* _value_entry_create_string(char const*,int64_t,char const*,int64_t);
_value_entry_t* _value_entry_create_number(char const*,int64_t,double);
_value_entry_t* _value_entry_create_date(char const*,int64_t,double);
_VALUE_ENTRY_TYPE _value_entry_type(_value_entry_t*);
char const* _value_entry_path(_value_entry_t*);
char const* _value_entry_string(_value_entry_t*);
double _value_entry_number(_value_entry_t*);
double _value_entry_date(_value_entry_t*);
void _value_entry_destroy(_value_entry_t*);

_value_t* _value_clone(_value_t const*);
_value_t* _value_create_null(void);
_value_t* _value_create_string(char const*,int64_t);
_value_t* _value_create_number(double);
_value_t* _value_create_date(double);
_value_t* _value_create_bool(bool);
_value_t* _value_create_array(void);
_value_t* _value_create_map(void);
_value_t* _value_create_entries(_value_entry_t const**,int64_t);
void _value_move_from_parent(_value_t*);
void _value_destroy(_value_t*);
void _value_set_null(_value_t*);
void _value_set_string(_value_t*,char const*,int64_t);
void _value_set_number(_value_t*,double);
void _value_set_date(_value_t*,double);
void _value_set_bool(_value_t*,bool);
void _value_set_array(_value_t*);
void _value_set_map(_value_t*);
_value_t* _value_parent(_value_t const*);
_VALUE_TYPE _value_type(_value_t const*);
char const* _value_string(_value_t const*);
double _value_number(_value_t const*);
double _value_date(_value_t const*);
bool _value_bool(_value_t const*);
int64_t _value_array_count(_value_t const*);
_value_t* _value_array_get(_value_t const*,int64_t);
_value_t* _value_array_move(_value_t const*,int64_t);
void _value_array_remove(_value_t*,int64_t);
void _value_array_set_copy(_value_t*,int64_t,_value_t const*);
void _value_array_set_move(_value_t*,int64_t,_value_t*);
void _value_array_push_copy(_value_t*,_value_t const*);
void _value_array_push_move(_value_t*,_value_t*);
void _value_array_unshift_copy(_value_t*,_value_t const*);
void _value_array_unshift_move(_value_t*,_value_t*);
_value_t* _value_array_pop(_value_t*);
_value_t* _value_array_shift(_value_t*);
void _value_array_clear(_value_t*);
int64_t _value_map_key_count(_value_t const*);
char const* _value_map_key_item(_value_t const*,int64_t);
void _value_map_set_copy(_value_t*,char const*,int64_t,_value_t const*);
void _value_map_set_move(_value_t*,char const*,int64_t,_value_t*);
bool _value_map_has(_value_t*,char const*,int64_t);
_value_t* _value_map_get(_value_t*,char const*,int64_t);
_value_t* _value_map_move(_value_t*,char const*,int64_t);
void _value_map_clear(_value_t*);
void _value_entries(_value_t const*, _value_entry_t const***,int64_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_VALUE_H */
