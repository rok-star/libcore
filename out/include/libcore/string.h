#ifndef _LIBCORE_STRING_H
#define _LIBCORE_STRING_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

char* _string_clone(char const*,int);
char* _string_uppercase(char const*,int);
char* _string_lowercase(char const*,int);
bool _string_compare(char const*,char const*,int);
void _string_tokenize(char const*,int,char***,int*);
void _string_split(char const*,int,char const*,char const***,int*);
bool _string_whitespace(char const*,int);
bool _string_digital(char const*,int);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_STRING_H */
