#ifndef _LIBCORE_ANY_H
#define _LIBCORE_ANY_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	_NULL_ANY_ENTRY_TYPE,
	_STRING_ANY_ENTRY_TYPE,
	_NUMBER_ANY_ENTRY_TYPE,
	_DATE_ANY_ENTRY_TYPE
} _ANY_ENTRY_TYPE;

typedef struct _any_entry_t _any_entry_t;

typedef enum {
	_NULL_ANY_TYPE,
	_STRING_ANY_TYPE,
	_NUMBER_ANY_TYPE,
	_DATE_ANY_TYPE,
	_ARRAY_ANY_TYPE,
	_MAP_ANY_TYPE
} _ANY_TYPE;

typedef struct _any_t _any_t;

#ifdef __cplusplus
extern "C" {
#endif

_any_entry_t* _any_entry_create_null(char const*,int64_t);
_any_entry_t* _any_entry_create_string(char const*,int64_t,char const*,int64_t);
_any_entry_t* _any_entry_create_number(char const*,int64_t,double);
_any_entry_t* _any_entry_create_date(char const*,int64_t,double);
_ANY_ENTRY_TYPE _any_entry_type(_any_entry_t*);
char const* _any_entry_path(_any_entry_t*);
char const* _any_entry_string(_any_entry_t*);
double _any_entry_number(_any_entry_t*);
double _any_entry_date(_any_entry_t*);
void _any_entry_destroy(_any_entry_t*);
_any_t* _any_clone(_any_t const*);
_any_t* _any_create_null(void);
_any_t* _any_create_string(char const*,int64_t);
_any_t* _any_create_number(double);
_any_t* _any_create_date(double);
_any_t* _any_create_array(void);
_any_t* _any_create_map(void);
_any_t* _any_create_entries(_any_entry_t const**,int64_t);
void _any_destroy(_any_t*);
void _any_set_null(_any_t*);
void _any_set_string(_any_t*,char const*,int64_t);
void _any_set_number(_any_t*,double);
void _any_set_date(_any_t*,double);
void _any_set_array(_any_t*);
void _any_set_map(_any_t*);
_ANY_TYPE _any_type(_any_t const*);
char const* _any_string(_any_t const*);
double _any_number(_any_t const*);
double _any_date(_any_t const*);
int64_t _any_array_count(_any_t const*);
_any_t* _any_array_item(_any_t const*,int64_t);
void _any_array_push(_any_t*,_any_t const*);
void _any_array_unshift(_any_t*,_any_t const*);
_any_t* _any_array_pop(_any_t*);
_any_t* _any_array_shift(_any_t*);
void _any_array_clear(_any_t*);
int64_t _any_map_key_count(_any_t const*);
char const* _any_map_key_item(_any_t const*,int64_t);
void _any_map_set(_any_t*,char const*,_any_t const*);
bool _any_map_has(_any_t*,char const*);
_any_t* _any_map_get(_any_t*,char const*);
void _any_map_clear(_any_t*);
void _any_entries(_any_t const*, _any_entry_t const***,int64_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_ANY_H */
