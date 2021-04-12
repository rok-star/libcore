#ifndef _LIBCORE_ARGS_H
#define _LIBCORE_ARGS_H

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

typedef enum {
	_SPACE_PARAM_VALUE_SEPARATOR  = 0,
	_EQUALS_PARAM_VALUE_SEPARATOR = 1,
	_BOTH_PARAM_VALUE_SEPARATOR   = 2
} _PARAM_VALUE_SEPARATOR;

#ifdef __cplusplus
extern "C" {
#endif

_args_t* _args_create(char const*[],int);
void _args_destroy(_args_t*);
int _args_action(_args_t*);
double _args_option_number(_args_t*,int);
char const* _args_option_string(_args_t*,int);

// int _arg_has(char const**,int,char const*,int64_t,_PARAM_VALUE_SEPARATOR);
// int _arg_value(char const**,int,char const*,int64_t,_PARAM_VALUE_SEPARATOR,char const**,int64_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_ARGS_H */
