#include <libcore/MACRO.h>
#include <libcore/window.h>
#include <libcore/context.h>

#include "metal.h"

#define _SAMPLE_COUNT 4

#define _RECT_TRANSFORM(rect, transform) { \
    if ((transform).scale != 0) { \
        (rect).origin.x *= (transform).scale; \
        (rect).origin.y *= (transform).scale; \
        (rect).size.width *= (transform).scale; \
        (rect).size.height *= (transform).scale; \
    } \
    (rect).origin.x += (transform).x; \
    (rect).origin.y += (transform).y; \
}

#define _POINT_TRANSFORM(point, transform) { \
    if ((transform).scale != 0) { \
        (point).x *= (transform).scale; \
        (point).y *= (transform).scale; \
    } \
    (point).x += (transform).x; \
    (point).y += (transform).y; \
}

#define _RECT_TRIANGLES_STRIP(rect, data) { \
    (data)[0] = (rect).origin.x; \
    (data)[1] = (rect).origin.y; \
    (data)[2] = _RECT_MAX_X(rect); \
    (data)[3] = (rect).origin.y; \
    (data)[4] = (rect).origin.x; \
    (data)[5] = _RECT_MAX_Y(rect); \
    (data)[6] = _RECT_MAX_X(rect); \
    (data)[7] = _RECT_MAX_Y(rect); \
}

#define _RECT_TRIANGLES(rect, data) { \
    (data)[0] = (rect).origin.x; \
    (data)[1] = (rect).origin.y; \
    (data)[2] = _RECT_MAX_X(rect); \
    (data)[3] = (rect).origin.y; \
    (data)[4] = (rect).origin.x; \
    (data)[5] = _RECT_MAX_Y(rect); \
    (data)[6] = (rect).origin.x; \
    (data)[7] = _RECT_MAX_Y(rect); \
    (data)[8] = _RECT_MAX_X(rect); \
    (data)[9] = _RECT_MAX_Y(rect); \
    (data)[10] = _RECT_MAX_X(rect); \
    (data)[11] = (rect).origin.y; \
}

extern void* _window_NSWindow(_window_t const*);

typedef struct _context_t {
    NSWindow* window;
    _texture_t const* texture;
    CAMetalLayer* layer;
    id<CAMetalDrawable> drawable;
    id<MTLCommandQueue> command_queue;
    id<MTLCommandBuffer> command_buffer;
    id<MTLRenderCommandEncoder> command_encoder;
    id<MTLRenderPipelineState> color_pipeline_state;
    id<MTLRenderPipelineState> texture_pipeline_state;
    id<MTLTexture> msaa_texture;
    _CONTEXT_ORIGIN origin;
    _size_t size;
    _rect_t clip;
    bool painting;
} _context_t;

static _context_t* __create(_texture_t const* texture, _window_t const* window) {
    _ASSERT(__metal_device != NULL);
    _ASSERT(__metal_library != NULL);

    _context_t* context = _NEW(_context_t, {});

    if (window != NULL) {
        context->window = (__bridge NSWindow*)_window_NSWindow(window);
        context->layer = [[CAMetalLayer alloc] init];
        [context->layer setDevice: __metal_device];
        [context->layer setPixelFormat: MTLPixelFormatBGRA8Unorm];
        [context->layer setMagnificationFilter: kCAFilterNearest];
        [context->layer setMinificationFilter: kCAFilterNearest];
        [context->layer setFramebufferOnly: YES];
        [context->layer setPresentsWithTransaction: YES];
        [context->window.contentView setWantsLayer: YES];
        [context->window.contentView.layer addSublayer: context->layer];
    } else {
        context->texture = texture;
    }

    context->command_queue = [__metal_device newCommandQueue];

    NSError* _err = NULL;
    MTLRenderPipelineDescriptor* pipeline_descriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipeline_descriptor.colorAttachments[0].pixelFormat                 = MTLPixelFormatRGBA8Unorm;
    pipeline_descriptor.colorAttachments[0].blendingEnabled             = YES;
    pipeline_descriptor.colorAttachments[0].rgbBlendOperation           = MTLBlendOperationAdd;
    pipeline_descriptor.colorAttachments[0].alphaBlendOperation         = MTLBlendOperationAdd;
    pipeline_descriptor.colorAttachments[0].sourceRGBBlendFactor        = MTLBlendFactorSourceAlpha;
    pipeline_descriptor.colorAttachments[0].sourceAlphaBlendFactor      = MTLBlendFactorSourceAlpha;
    pipeline_descriptor.colorAttachments[0].destinationRGBBlendFactor   = MTLBlendFactorOneMinusSourceAlpha;
    pipeline_descriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    pipeline_descriptor.sampleCount = _SAMPLE_COUNT;

    pipeline_descriptor.vertexFunction = [__metal_library newFunctionWithName: @"vertex_color_shader"];
    pipeline_descriptor.fragmentFunction = [__metal_library newFunctionWithName: @"fragment_color_shader"];
    context->color_pipeline_state = [__metal_device newRenderPipelineStateWithDescriptor: pipeline_descriptor error: &_err];

    if (_err != NULL)
        _ABORT("%s", [[_err localizedDescription] cStringUsingEncoding: NSUTF8StringEncoding]);

    pipeline_descriptor.vertexFunction = [__metal_library newFunctionWithName: @"vertex_texture_shader"];
    pipeline_descriptor.fragmentFunction = [__metal_library newFunctionWithName: @"fragment_texture_shader"];
    context->texture_pipeline_state = [__metal_device newRenderPipelineStateWithDescriptor: pipeline_descriptor error: &_err];

    if (_err != NULL)
        _ABORT("%s", [[_err localizedDescription] cStringUsingEncoding: NSUTF8StringEncoding]);

    return context;
}

_context_t* _context_create_texture(_texture_t const* texture) {
    _ASSERT(texture != NULL);
    return __create(texture, NULL);
}

_context_t* _context_create_window(_window_t const* window) {
    _ASSERT(window != NULL);
    return __create(NULL, window);
}

void _context_destroy(_context_t* context) {
    _ASSERT(context != NULL);

    if (context->window != NULL)
        [context->layer removeFromSuperlayer];

    context->layer = nil;
    context->window = nil;
    context->texture = nil;
    context->drawable = nil;
    context->command_queue = nil;
    context->command_buffer = nil;
    context->command_encoder = nil;
    context->color_pipeline_state = nil;
    context->texture_pipeline_state = nil;
    context->msaa_texture = nil;
}

void _context_begin_paint(_context_t* context) {
    _ASSERT(context != NULL);
    _ASSERT(context->command_queue != NULL);
    _ASSERT(context->color_pipeline_state != NULL);
    _ASSERT(context->painting == false);

    context->painting = true;

    MTLRenderPassDescriptor* pass_descriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    pass_descriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 0, 0, 0);
    pass_descriptor.colorAttachments[0].loadAction = MTLLoadActionClear;

    id<MTLTexture> resolve_texture = nil;

    if (context->window != NULL) {
        _ASSERT(context->layer != NULL);

        context->size = (_size_t){
            .width = (context->window.contentView.frame.size.width * context->window.backingScaleFactor),
            .height = (context->window.contentView.frame.size.height * context->window.backingScaleFactor),
        };

        _ASSERT(context->size.width > 0);
        _ASSERT(context->size.height > 0);

        [CATransaction begin];
        [CATransaction setValue: (id)kCFBooleanTrue forKey: kCATransactionDisableActions];
        [context->layer setFrame: context->window.contentView.frame];
        [context->layer setDrawableSize: NSMakeSize(context->size.width, context->size.height)];
        [CATransaction commit];

        context->drawable = [context->layer nextDrawable];
        resolve_texture = context->drawable.texture;
    } else {
        context->size = _texture_size(context->texture);

        _ASSERT(context->size.width > 0);
        _ASSERT(context->size.height > 0);

        resolve_texture = (__bridge id<MTLTexture>)_texture_MTLTexture(context->texture);
    }

    if ((context->msaa_texture == nil)
    || (context->msaa_texture.width != (NSUInteger)context->size.width)
    || (context->msaa_texture.height != (NSUInteger)context->size.height)) {
        MTLTextureDescriptor* desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat: MTLPixelFormatRGBA8Unorm
                                                                                        width: context->size.width
                                                                                        height: context->size.height
                                                                                     mipmapped: NO];
        desc.storageMode = MTLStorageModePrivate;
        desc.textureType = MTLTextureType2DMultisample;
        desc.usage = MTLTextureUsageRenderTarget;
        desc.sampleCount = _SAMPLE_COUNT;
        context->msaa_texture = [__metal_device newTextureWithDescriptor: desc];
    }

    pass_descriptor.colorAttachments[0].texture = context->msaa_texture;
    pass_descriptor.colorAttachments[0].resolveTexture = resolve_texture;
    pass_descriptor.colorAttachments[0].storeAction = MTLStoreActionMultisampleResolve;

    context->command_buffer = [context->command_queue commandBuffer];
    context->command_encoder = [context->command_buffer renderCommandEncoderWithDescriptor: pass_descriptor];
}

void _context_end_paint(_context_t* context) {
    _ASSERT(context != NULL);
    _ASSERT(context->command_buffer != NULL);
    _ASSERT(context->painting == true);

    context->painting = false;

    [context->command_encoder endEncoding];

    if (context->window != NULL) {
        _ASSERT(context->drawable != NULL);
        [context->drawable present];
    }

    [context->command_buffer commit];
    context->command_buffer = NULL;
    context->command_encoder = NULL;
    context->drawable = NULL;
}

_size_t _context_size(_context_t const* context) {
    _ASSERT(context != NULL);
    if (context->window != NULL) {
        _ASSERT(context->layer != NULL);
        return (_size_t){
            .width = (context->window.contentView.frame.size.width * context->window.backingScaleFactor),
            .height = (context->window.contentView.frame.size.height * context->window.backingScaleFactor),
        };
    } else {
        return _texture_size(context->texture);
    }
}

_rect_t _context_clip(_context_t const* context) {
    _ASSERT(context != NULL);
    return context->clip;
}

_CONTEXT_ORIGIN _context_origin(_context_t const* context) {
    _ASSERT(context != NULL);
    return context->origin;
}

void _context_set_clip(_context_t* context, _rect_t const* rect) {
    _ASSERT(context != NULL);

    if ((rect == NULL)
    || (rect->size.width <= 0)
    || (rect->size.height <= 0)) {
        context->clip = (_rect_t){ { 0, 0 }, { 0, 0 } };
    } else {
        context->clip = *rect;
    }
}

void _context_set_origin(_context_t* context, _CONTEXT_ORIGIN origin) {
    _ASSERT(context != NULL);
    context->origin = origin;
}

void _context_draw_vertices(_context_t const* context, float const* array, int size, bool strip, _brush_t const* brush) {
    _ASSERT(context != NULL);
    _ASSERT(array != NULL);
    _ASSERT(size > 0);
    _ASSERT(brush != NULL);
    _ASSERT(context->painting == true);
    _ASSERT(context->command_encoder != NULL);
    _ASSERT((context->size.width > 0) && (context->size.height > 0));

    _color_t const* color = _brush_color(brush);

    float uniforms[8] = {
        color->red,
        color->green,
        color->blue,
        color->alpha,
        context->size.width,
        context->size.height,
        ((context->origin == _LEFTTOP_CONTEXT_ORIGIN) ? -1 : 1),
        0
    };

    [context->command_encoder setRenderPipelineState: context->color_pipeline_state];
    [context->command_encoder setVertexBytes: &uniforms length: (sizeof(float) * 8) atIndex: 0];
    [context->command_encoder setVertexBytes: array length: (sizeof(float) * size) atIndex: 1];
    if ((context->clip.size.width > 0)
    && (context->clip.size.height > 0)) {
        [context->command_encoder setScissorRect: (MTLScissorRect){
            context->clip.origin.x,
            context->clip.origin.y,
            context->clip.size.width,
            context->clip.size.height
        }];
    }
    [context->command_encoder drawPrimitives: (strip ? MTLPrimitiveTypeTriangleStrip : MTLPrimitiveTypeTriangle)
                                  vertexStart: 0
                                  vertexCount: (size / 2)];
}

void _context_draw_texture(_context_t const* context, _texture_t const* texture, _rect_t const* src, _rect_t const* dst, _color_t const* tint) {
    _ASSERT(texture != NULL);
    _ASSERT(context != NULL);
    _ASSERT(context->painting == true);
    _ASSERT(context->command_encoder != NULL);
    _ASSERT(_texture_MTLTexture(texture) != NULL);

    _size_t size = _texture_size(texture);
    _ASSERT(size.width > 0);
    _ASSERT(size.height > 0);

    _rect_t src_ = ((src != NULL) ? *src : (_rect_t){ { 0, 0 }, size });
    _rect_t dst_ = ((dst != NULL) ? *dst : (_rect_t){ { 0, 0 }, size });

    if ((dst_.origin.x > context->size.width)
    || (dst_.origin.y > context->size.height)
    || ((dst_.origin.x + dst_.size.width) < 0)
    || ((dst_.origin.y + dst_.size.height) < 0))
        return;

    float uniforms[8] = {
        ((tint != NULL) ? tint->red : 0),
        ((tint != NULL) ? tint->green : 0),
        ((tint != NULL) ? tint->blue : 0),
        ((tint != NULL) ? tint->alpha : 0),
        context->size.width,
        context->size.height,
        ((context->origin == _LEFTTOP_CONTEXT_ORIGIN) ? -1 : 1),
        ((tint != NULL) ? 1.0f : 0.0f)
    };

    float uv_x = (src_.origin.x / (float)size.width);
    float uv_y = (src_.origin.y / (float)size.height);
    float uv_max_x = ((src_.origin.x + src_.size.width) / (float)size.width);
    float uv_max_y = ((src_.origin.y + src_.size.height) / (float)size.height);

    if (context->origin == 1.0f) {
        uv_y = (1 - uv_y);
        uv_max_y = (1 - uv_max_y);
    }

    float array[16] = {
        dst_.origin.x,
        dst_.origin.y,
        uv_x,
        uv_y,

        (dst_.origin.x + dst_.size.width),
        dst_.origin.y,
        uv_max_x,
        uv_y,

        dst_.origin.x,
        (dst_.origin.y + dst_.size.height),
        uv_x,
        uv_max_y,

        (dst_.origin.x + dst_.size.width),
        (dst_.origin.y + dst_.size.height),
        uv_max_x,
        uv_max_y

    };

    [context->command_encoder setRenderPipelineState: context->texture_pipeline_state];
    [context->command_encoder setVertexBytes: &uniforms length: (sizeof(float) * 8) atIndex: 0];
    [context->command_encoder setVertexBytes: &array length: (sizeof(float) * 16) atIndex: 1];
    [context->command_encoder setFragmentTexture: (__bridge id<MTLTexture>)_texture_MTLTexture(texture) atIndex: 0];

    if ((context->clip.size.width > 0)
    && (context->clip.size.height > 0)) {
        [context->command_encoder setScissorRect: (MTLScissorRect){
            context->clip.origin.x,
            context->clip.origin.y,
            context->clip.size.width,
            context->clip.size.height
        }];
    }
    [context->command_encoder drawPrimitives: MTLPrimitiveTypeTriangleStrip vertexStart: 0 vertexCount: 4];
}

void _context_stroke_line(_context_t const* context, _point_t const* from, _point_t const* to, double width, _LINE_CAP cap, _brush_t const* brush, _transform_t const* transform) {
    _ASSERT(context != NULL);
    _ASSERT(from != NULL);
    _ASSERT(to != NULL);
    _ASSERT(brush != NULL);
}

void _context_stroke_rect(_context_t const* context, _rect_t const* rect, double width, _brush_t const* brush, _transform_t const* transform) {
    _ASSERT(context != NULL);
    _ASSERT(rect != NULL);
    _ASSERT(brush != NULL);

    /* NOTE: Borders are being generated clock-wise for TopLeft origin and opposite for BottomLeft */

    _rect_t border1 = {
        .origin = {
            .x = (rect->origin.x - (width * 0.5)),
            .y = (rect->origin.y - (width * 0.5))
        },
        .size = {
            .width = (rect->size.width + width),
            .height = width
        }
    };

    _rect_t border2 = {
        .origin = {
            .x = (_RECT_MAX_X(*rect) - (width * 0.5)),
            .y = rect->origin.y + (width * 0.5)
        },
        .size = {
            .width = width,
            .height = (rect->size.height - width)
        }
    };
    _rect_t border3 = {
        .origin = {
            .x = (rect->origin.x - (width * 0.5)),
            .y = (_RECT_MAX_Y(*rect) - (width * 0.5))
        },
        .size = {
            .width = (rect->size.width + width),
            .height = width
        }
    };
    _rect_t border4 = {
        .origin = {
            .x = rect->origin.x - (width * 0.5),
            .y = (rect->origin.y + (width * 0.5))
        },
        .size = {
            .width = width,
            .height = (rect->size.height - width)
        }
    };

    if (transform != NULL) {
        _RECT_TRANSFORM(border1, *transform);
        _RECT_TRANSFORM(border2, *transform);
        _RECT_TRANSFORM(border3, *transform);
        _RECT_TRANSFORM(border4, *transform);
    }

    float vertices[(12 * 4)];
    float* _dst = vertices;
    _RECT_TRIANGLES(border1, _dst); _dst += 12;
    _RECT_TRIANGLES(border2, _dst); _dst += 12;
    _RECT_TRIANGLES(border3, _dst); _dst += 12;
    _RECT_TRIANGLES(border4, _dst);

    _context_draw_vertices(context, vertices, (12 * 4), false, brush);
}

void _context_stroke_path(_context_t const* context, _bezier_path_t const* path, double width, _brush_t const* brush, _transform_t const* transform) {
    _ABORT("_context_stroke_path: Not implemented");
}

void _context_stroke_ellipse(_context_t const* context, _rect_t const* rect, double width, _brush_t const* brush, _transform_t const* transform) {
    _ABORT("_context_stroke_ellipse: Not implemented");
}

void _context_fill_rect(_context_t const* context, _rect_t const* rect, _brush_t const* brush, _transform_t const* transform) {
    _ASSERT(context != NULL);
    _ASSERT(rect != NULL);
    _ASSERT(brush != NULL);

    _rect_t rect_ = *rect;

    if (transform != NULL)
        _RECT_TRANSFORM(rect_, *transform);

    float vertices[8];
    _RECT_TRIANGLES_STRIP(rect_, vertices);

    _context_draw_vertices(context, vertices, 8, true, brush);
}

void _context_fill_path(_context_t const* context, _bezier_path_t const* path, _brush_t const* brush, _transform_t const* transform) {
    _ABORT("_context_fill_path: Not implemented");
}

void _context_fill_ellipse(_context_t const* context, _rect_t const* rect, _brush_t const* brush, _transform_t const* transform) {
    _ABORT("_context_fill_ellipse: Not implemented");
}
