#ifndef _LIBCORE_TEXTURE_H
#define _LIBCORE_TEXTURE_H

typedef struct _Texture _Texture;

_Texture* _Texture_create(void*, int, int, int);
void _Texture_destroy(_Texture*);

#endif /* _LIBCORE_TEXTURE_H */
