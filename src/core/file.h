#ifndef _LIBCORE_FILE_H
#define _LIBCORE_FILE_H

#include <stdint.h>
#include <libcore/core/status.h>

#ifdef __cplusplus
extern "C" {
#endif

void _file_read(char const*,uint8_t**,int64_t*,_status_t*);
void _file_write(char const*,uint8_t const*,int64_t,_status_t*);
void _file_append(char const*,uint8_t const*,int64_t,_status_t*);
void _file_remove(char const*,_status_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_FILE_H */
