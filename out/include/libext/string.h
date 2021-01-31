#ifndef _LIBEXT_STRING_H
#define _LIBEXT_STRING_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

char* _string_clone(char const*,int64_t);
char* _string_uppercase(char const*,int64_t);
char* _string_lowercase(char const*,int64_t);
bool _string_compare(char const*,char const*,int64_t);
void _string_tokenize(char const*,int64_t,char***,int64_t*);
void _string_split(char const*,int64_t,char const*,char const***,int64_t*);
bool _string_whitespace(char const*,int64_t);
bool _string_digital(char const*,int64_t);
int64_t _string_read_int8(char const*,int64_t,bool,int8_t*);
int64_t _string_read_int16(char const*,int64_t,bool,int16_t*);
int64_t _string_read_int32(char const*,int64_t,bool,int32_t*);
int64_t _string_read_int64(char const*,int64_t,bool,int64_t*);
int64_t _string_read_uint8(char const*,int64_t,bool,uint8_t*);
int64_t _string_read_uint16(char const*,int64_t,bool,uint16_t*);
int64_t _string_read_uint32(char const*,int64_t,bool,uint32_t*);
int64_t _string_read_uint64(char const*,int64_t,bool,uint64_t*);
int64_t _string_read_double(char const*,int64_t,bool,double*);
int64_t _string_read_line(char const*,int64_t);
int64_t _string_read_until(char const*,int64_t,char const*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBEXT_STRING_H */
