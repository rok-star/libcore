#ifndef _LIBEXT_PARSER_H
#define _LIBEXT_PARSER_H

#include <stdint.h>
#include <stdbool.h>
#include <libext/status.h>

typedef struct _parser_t _parser_t;

#ifdef __cplusplus
extern "C" {
#endif

_parser_t* _parser_create(char const*,int64_t);
void _parser_destroy(_parser_t*);
int64_t _parser_position(_parser_t*);
bool _parser_end(_parser_t*);
bool _parser_seek(_parser_t*,int64_t);
bool _parser_read_line(_parser_t*,char const**,int64_t*);
bool _parser_read_until(_parser_t*,char const*,int64_t,char const**,int64_t*);
bool _parser_read_exact(_parser_t*,char const*,int64_t);
bool _parser_read_string(_parser_t*,char const**,int64_t*);
bool _parser_read_whitespace(_parser_t*,int64_t*);
bool _parser_read_int8(_parser_t*,bool,int8_t*);
bool _parser_read_int16(_parser_t*,bool,int16_t*);
bool _parser_read_int32(_parser_t*,bool,int32_t*);
bool _parser_read_int64(_parser_t*,bool,int64_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBEXT_PARSER_H */
