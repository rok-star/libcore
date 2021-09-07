#ifndef _LIBCORE_JSON_H
#define _LIBCORE_JSON_H

#include <libcore/core/status.h>
#include <libcore/core/value.h>

_value_t* _json_parse(char const*,int64_t,_status_t*);
char const* _json_stringify(_value_t const*);

#endif /* _LIBCORE_JSON_H */
