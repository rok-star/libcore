#include <libcore/texture.h>
#include <libcore/MACRO.h>
#include "metal.h"

typedef struct _Texture {
    id<MTLTexture> texture;
    void* data;
    int size;
    int width;
    int height;
} _Texture;

_Texture* _Texture_create(void* data, int size, int width, int height) {
	_ASSERT(__metal_device != NULL);
    _ASSERT(__metal_library != NULL);

	_ASSERT_M((width > 0) && (height > 0), "texture \"width\" and \"height\" must be greater than 0");
	_ASSERT_M(((data == NULL) && (size == 0)) || ((data != NULL) && (size > 0)), "texture \"data\" and \"size\" must be specified along with each other");
	if (size > 0)
		_ASSERT_M(size == ((width * height) * 4), "texture \"width\" and \"height\" must be correlated with \"size\"");

	MTLTextureDescriptor* descriptor = [[MTLTextureDescriptor alloc] init];
    descriptor.storageMode = MTLStorageModeManaged;
    descriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
    descriptor.width = width;
    descriptor.height = height;

    id<MTLTexture> texture = [__metal_device newTextureWithDescriptor: descriptor];
    _ASSERT(texture != NULL);

    if ((data != NULL) && (size > 0))
        [texture replaceRegion: (MTLRegion){ { 0, 0, 0 }, { width, height, 1 } }
                   mipmapLevel: 0
                     withBytes: data
                   bytesPerRow: (width * 4)];

    return _NEW(_Texture, {
        .texture = texture,
        .data = data,
        .size = size,
        .width = width,
        .height = height
    });
}

void _Texture_destroy(_Texture* texture) {
	_ASSERT(texture != NULL);

    texture->texture = NULL;
}

_Size _Texture_size(_Texture* texture) {
	_ASSERT(texture != NULL);

	return (_Size){
		.width = texture->width,
		.height = texture->height
	};
}

void* _Texture_MTLTexture(_Texture* texture) {
	_ASSERT(texture != NULL);

	return (__bridge void*)texture->texture;
}
