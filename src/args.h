#ifndef _LIBCORE_ARGS_H
#define _LIBCORE_ARGS_H

#include <stdbool.h>
#include <libcore/status.h>

typedef enum {
	_STRING_ARGS_SCHEMA_OPTION_TYPE  = 0,
	_NUMBER_ARGS_SCHEMA_OPTION_TYPE  = 1,
	_BOOLEAN_ARGS_SCHEMA_OPTION_TYPE = 2,
	_FILE_ARGS_SCHEMA_OPTION_TYPE    = 3,
	_DIR_ARGS_SCHEMA_OPTION_TYPE     = 4
} _ARGS_SCHEMA_OPTION_TYPE;

typedef struct _args_schema_option_t {
	int id;
	char const* name;
	_ARGS_SCHEMA_OPTION_TYPE type;
	bool array;
} _args_schema_option_t;

typedef struct _args_schema_action_t {
	int id;
	char const* name;
	struct {
		_args_schema_option_t* data;
		int size;
	} options;
} _args_schema_action_t;

typedef struct _args_schema_t {
	struct {
		_args_schema_action_t* data;
		int size;
	} actions;
	struct {
		_args_schema_option_t* data;
		int size;
	} options;
} _args_schema_t;

typedef struct _args_t _args_t;

#ifdef __cplusplus
extern "C" {
#endif

_args_t* _args_create(_args_schema_t const*,char const**,int,_status_t*);
void _args_destroy(_args_t*);
int _args_action(_args_t*);
double _args_option_number(_args_t*,int);
char const* _args_option_string(_args_t*,int);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_ARGS_H */
