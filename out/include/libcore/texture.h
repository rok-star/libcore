#ifndef _LIBEXT_TEXTURE_H
#define _LIBEXT_TEXTURE_H

#include <libext/math.h>

typedef struct _texture_t _texture_t;

#ifdef __cplusplus
extern "C" {
#endif

_texture_t* _texture_create(void const*,int,int,int);
void _texture_destroy(_texture_t*);
_size_t const* _texture_size(_texture_t const*);

#if _WIN32

#elif __APPLE__
    void* _texture_MTLTexture(_texture_t const*);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBEXT_TEXTURE_H */
