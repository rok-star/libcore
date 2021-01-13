#ifndef _LIBCORE_PATH_H
#define _LIBCORE_PATH_H

#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
    static char const* const _PATH_SEPARATOR = "\\";
#else
    static char const* const _PATH_SEPARATOR = "/";
#endif

typedef struct _path_stat_t {
    bool exists;
    bool file;
    bool directory;
    double modified;
    int64_t size;
} _path_stat_t;

#ifdef __cplusplus
extern "C" {
#endif

char* _path_cwd(void);
char* _path_resolve(char const*);
char* _path_normalize(char const*);
char* _path_join(char const**);
void _path_stat(char const*,_path_stat_t*);
bool _path_exists(char const*);
bool _path_is_file(char const*);
bool _path_is_diretory(char const*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_PATH_H */
