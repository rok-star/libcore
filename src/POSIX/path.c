#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <libcore/MACRO.h>
#include <libcore/path.h>

#define getcwd_E(a, b) { \
    if (getcwd(a, b) == NULL)  \
        _ABORT("getcwd() failed: %s\n", strerror(errno)); \
}

char* _path_cwd(void) {
    char* ret = _ALLOC(char, (PATH_MAX + 1));
    getcwd_E(ret, (PATH_MAX + 1));
    return ret;
}

void _path_stat(char const* path, _path_stat_t* status) {
    _ASSERT(path != NULL);
    ;
}