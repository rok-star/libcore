#ifndef _LIBEXT_ARGS_H
#define _LIBEXT_ARGS_H

typedef enum {
	_SPACE_PARAM_VALUE_SEPARATOR  = 0,
	_EQUALS_PARAM_VALUE_SEPARATOR = 1,
	_BOTH_PARAM_VALUE_SEPARATOR   = 2
} _PARAM_VALUE_SEPARATOR;

#ifdef __cplusplus
extern "C" {
#endif

int _args_has(char const*[],int,char const*,int64_t);
int _args_value(char const*[],int,char const*,int64_t,_PARAM_VALUE_SEPARATOR,char const**,int64_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBEXT_ARGS_H */
