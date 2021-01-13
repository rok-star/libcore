#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libcore/MACRO.h>
#include <libcore/path.h>

#define getcwd_E(a, b) { \
    if (getcwd(a, b) == NULL)  \
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
    		.modified = ((st.st_mtimespec.tv_sec * 1000.0)
                        + (st.st_mtimespec.tv_nsec / 1000000.0)),
    		.size = st.st_size
    	};
    } else {
    	*stat_ = (_path_stat_t){};
    }
}