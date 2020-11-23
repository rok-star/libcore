#ifndef _LIBCORE_UNICODE_H
#define _LIBCORE_UNICODE_H

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t _codepoint_upper(uint32_t);
uint32_t _codepoint_lower(uint32_t);
bool _codepoint_whitespace(uint32_t);
bool _codepoint_digital(uint32_t);
int _codepoint_write_utf8(uint32_t, char*, int);
int _codepoint_write_utf16(uint32_t, uint16_t*, int);
int _utf8_read_codepoint(char const*, int, uint32_t*);
int _utf16_read_codepoint(uint16_t const*, int, uint32_t*);
int _utf8_to_utf16(char const*, int, uint16_t*, int);
int _utf8_to_utf32(char const*, int, uint32_t*, int);
int _utf8_to_ucs2(char const*, int, wchar_t*, int);
int _utf16_to_utf8(uint16_t const*, int, char*, int);
int _utf16_to_utf32(uint16_t const*, int, uint32_t*, int);
int _utf32_to_utf8(uint32_t const*, int, char*, int);
int _utf32_to_utf16(uint32_t const*, int, uint16_t*, int);
int _utf32_to_ucs2(uint32_t const*, int, wchar_t*, int);
int _ucs2_to_utf8(wchar_t const*, int, char*, int);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_UNICODE_H */
