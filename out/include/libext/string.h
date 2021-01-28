#ifndef _LIBEXT_STRING_H
#define _LIBEXT_STRING_H

#include <stdbool.h>

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
int64_t _string_readline(char const*,int64_t);

#ifdef __cplusplus
}
#endif

#endif /* _LIBEXT_STRING_H */
