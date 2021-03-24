#ifndef _LIBCORE_DIRECTORY_H
#define _LIBCORE_DIRECTORY_H

#include <libcore/status.h>

#ifdef __cplusplus
extern "C" {
#endif

void _dir_read(char const*,char***,int*,_status_t*);
void _dir_remove(char const*,bool,_status_t*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_DIRECTORY_H */
