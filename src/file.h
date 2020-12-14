#ifndef _LIBCORE_FILE_H
#define _LIBCORE_FILE_H

#include <libcore/status.h>

#ifdef __cplusplus
extern "C" {
#endif

void _file_read(char const*,void**,size_t*,_status_t*);
void _file_write(char const*,void*,size_t,_status_t*);
void _file_append(char const*,void*,size_t,_status_t*);
void _file_remove(char const*,_status_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_FILE_H */
