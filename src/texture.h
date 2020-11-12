#ifndef _LIBCORE_TEXTURE_H
#define _LIBCORE_TEXTURE_H

typedef struct _Texture _Texture;

#ifdef __cplusplus
extern "C" {
#endif

_Texture* _Texture_create(void*, int, int, int);
void _Texture_destroy(_Texture*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_TEXTURE_H */
