#ifndef _LIBCORE_UNICODE_H
#define _LIBCORE_UNICODE_H

#ifdef __cplusplus
extern "C" {
#endif

char32_t _codepoint_upper(char32_t);
char32_t _codepoint_lower(char32_t);
bool _codepoint_whitespace(char32_t);
bool _codepoint_digital(char32_t);
int _codepoint_write_utf8(char32_t, char*, int);
int _codepoint_write_utf16(char32_t, char16_t*, int);
int _utf8_read_codepoint(char const*, int, char32_t*);
int _utf16_read_codepoint(char16_t const*, int, char32_t*);
int _utf8_to_utf16(char const*, int, char16_t*, int);
int _utf8_to_utf32(char const*, int, char32_t*, int);
int _utf8_to_ucs2(char const*, int, wchar_t*, int);
int _utf16_to_utf8(char16_t const*, int, char*, int);
int _utf16_to_utf32(char16_t const*, int, char32_t*, int);
int _utf32_to_utf8(char32_t const*, int, char*, int);
int _utf32_to_utf16(char32_t const*, int, char16_t*, int);
int _utf32_to_ucs2(char32_t const*, int, wchar_t*, int);
int _ucs2_to_utf8(wchar_t const*, int, char*, int);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_UNICODE_H */
