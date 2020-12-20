#include <libcore/texture.h>
#include <libcore/MACRO.h>

#include "metal.h"

typedef struct _texture_t {
    id<MTLTexture> texture;
    struct {
        void const* data;
        int size;
    } data;
    _size_t size;
} _texture_t;

_texture_t* _texture_create(void const* data, int size, int width, int height) {
    _ASSERT(__metal_device != NULL);
    _ASSERT(__metal_library != NULL);

    _ASSERT_M((width > 0) && (height > 0), "texture \"width\" and \"height\" must be greater than 0");
    _ASSERT_M(((data == NULL) && (size == 0)) || ((data != NULL) && (size > 0)), "texture \"data\" and \"size\" must be specified along with each other");
    if (size > 0)
        _ASSERT_M(size == ((width * height) * 4), "texture \"width\" and \"height\" must be correlated with \"size\"");

    MTLTextureDescriptor* descriptor = [[MTLTextureDescriptor alloc] init];
    descriptor.storageMode = MTLStorageModePrivate;
    descriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
    descriptor.width = width;
    descriptor.height = height;

    id<MTLTexture> texture = [__metal_device newTextureWithDescriptor: descriptor];
    _ASSERT(texture != NULL);

    if ((data != NULL) && (size > 0)) {
        id<MTLCommandQueue> command_queue = [__metal_device newCommandQueue];
        id<MTLCommandBuffer> command_buffer = [command_queue commandBuffer];
        id<MTLBlitCommandEncoder> command_encoder = [command_buffer blitCommandEncoder];
        id<MTLBuffer> buffer = [__metal_device newBufferWithBytes: data
                                                           length: size
                                                          options: MTLResourceStorageModeShared];
        [command_encoder copyFromBuffer: buffer
                           sourceOffset: 0
                      sourceBytesPerRow: (width * 4)
                    sourceBytesPerImage: ((width * height) * 4)
                             sourceSize: (MTLSize){ width, height, 1 }
                              toTexture: texture
                       destinationSlice: 0
                       destinationLevel: 0
                      destinationOrigin: (MTLOrigin){ 0, 0, 0 }];

        [command_encoder endEncoding];
        [command_buffer commit];
        [command_buffer waitUntilCompleted];
    }

    return _NEW(_texture_t, {
        .texture = texture,
        .data = { data, size },
        .size = { width, height }
    });
}

void _texture_destroy(_texture_t* texture) {
    _ASSERT(texture != NULL);
    texture->texture = NULL;
}

_size_t const* _texture_size(_texture_t const* texture) {
    _ASSERT(texture != NULL);
    return &texture->size;
}

void* _texture_MTLTexture(_texture_t const* texture) {
    _ASSERT(texture != NULL);
    return (__bridge void*)texture->texture;
}

void _texture_set_msaa(_texture_t* texture, bool msaa) {
    _ASSERT(texture != NULL);
    ;
}