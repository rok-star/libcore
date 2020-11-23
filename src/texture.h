#ifndef _LIBCORE_TEXTURE_H
#define _LIBCORE_TEXTURE_H

#include <libcore/size.h>

typedef struct _Texture _Texture;

#ifdef __cplusplus
extern "C" {
#endif

_Texture* _Texture_create(void*,int,int,int);
void _Texture_destroy(_Texture*);
_Size _Texture_size(_Texture*);

#if _WIN32

#elif __APPLE__
    void* _Texture_MTLTexture(_Texture*);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_TEXTURE_H */
