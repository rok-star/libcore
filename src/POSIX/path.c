#ifdef __linux__
    #define _POSIX_C_SOURCE 200809L
#endif

#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libext/MACRO.h>
#include <libext/path.h>

#define getcwd_E(a, b) { \
    if (getcwd(a, b) == NULL) \
        _ABORT("getcwd() failed: %s\n", strerror(errno)); \
}

char* _path_cwd(void) {
    char* ret = _ALLOC(char, (PATH_MAX + 1));
    getcwd_E(ret, PATH_MAX);
    return ret;
}

void _path_stat(char const* path, _path_stat_t* stat_) {
    _ASSERT(path != NULL);
    _ASSERT(stat != NULL);
    struct stat st;
    if (stat(path, &st) == 0) {
    	*stat_ = (_path_stat_t){
    		.exists = true,
    		.file = S_ISREG(st.st_mode),
    		.directory = S_ISDIR(st.st_mode),
#if __linux__
            .modified = ((st.st_mtim.tv_sec * 1000.0)
                        + (st.st_mtim.tv_nsec / 1000000.0)),
#else
    		.modified = ((st.st_mtimespec.tv_sec * 1000.0)
                        + (st.st_mtimespec.tv_nsec / 1000000.0)),
#endif
    		.size = st.st_size
    	};
    } else {
    	*stat_ = (_path_stat_t){};
    }
}