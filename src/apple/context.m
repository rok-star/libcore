#include <libcore/window.h>
#include <libcore/context.h>
#include <libcore/MACRO.h>

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

typedef struct _Context {
    NSWindow* window;
    _Texture const* texture;
    CAMetalLayer* layer;
    id<CAMetalDrawable> drawable;
    id<MTLCommandQueue> command_queue;
    id<MTLCommandBuffer> command_buffer;
    id<MTLRenderCommandEncoder> command_encoder;
    id<MTLRenderPipelineState> color_pipeline_state;
    id<MTLRenderPipelineState> texture_pipeline_state;
    id<MTLTexture> msaa_texture;
    _CONTEXT_ORIGIN origin;
    _Size size;
    _Rect clip;
    bool painting;
} _Context;

_Context* __Context_create(_Texture const* texture, _Window const* window) {
	_ASSERT(__metal_device != NULL);
    _ASSERT(__metal_library != NULL);

    _Context* context = _NEW(_Context, {});

    if (window != NULL) {
        context->window = (__bridge NSWindow*)_Window_NSWindow(window);
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
    pipeline_descriptor.colorAttachments[0].pixelFormat                 = MTLPixelFormatBGRA8Unorm;
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

_Context* _Context_create_texture(_Texture const* texture) {
    _ASSERT(texture != NULL);
    return __Context_create(texture, NULL);
}

_Context* _Context_create_window(_Window const* window) {
    _ASSERT(window != NULL);
    return __Context_create(NULL, window);
}

void _Context_destroy(_Context* context) {
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

void _Context_begin_paint(_Context* context) {
	_ASSERT(context != NULL);
    _ASSERT(context->command_queue != NULL);
    _ASSERT(context->color_pipeline_state != NULL);
    _ASSERT(context->painting == false);

    context->painting = true;

    MTLRenderPassDescriptor* pass_descriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    pass_descriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 0, 0, 0);
    pass_descriptor.colorAttachments[0].loadAction = MTLLoadActionClear;

    if (context->window != NULL) {
        _ASSERT(context->layer != NULL);

        context->size = (_Size){
            .width = (context->window.contentView.frame.size.width * context->window.backingScaleFactor),
            .height = (context->window.contentView.frame.size.height * context->window.backingScaleFactor),
        };

        // context->size = (_Size){
        //     .width = (context->window.contentView.frame.size.width * 0.25),
        //     .height = (context->window.contentView.frame.size.height * 0.25),
        // };

        _ASSERT(context->size.width > 0);
        _ASSERT(context->size.height > 0);

        [CATransaction begin];
        [CATransaction setValue: (id)kCFBooleanTrue forKey: kCATransactionDisableActions];
        [context->layer setFrame: context->window.contentView.frame];
        [context->layer setDrawableSize: NSMakeSize(context->size.width, context->size.height)];
        [CATransaction commit];

        context->drawable = [context->layer nextDrawable];

        if ((context->msaa_texture == nil)
        || (context->msaa_texture.width != (NSUInteger)context->size.width)
        || (context->msaa_texture.height != (NSUInteger)context->size.height)) {
            MTLTextureDescriptor* desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat: MTLPixelFormatBGRA8Unorm
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
        pass_descriptor.colorAttachments[0].resolveTexture = context->drawable.texture;
        pass_descriptor.colorAttachments[0].storeAction = MTLStoreActionMultisampleResolve;
    } else {
    	context->size = _Texture_size(context->texture);

        _ASSERT(context->size.width > 0);
        _ASSERT(context->size.height > 0);

        pass_descriptor.colorAttachments[0].texture = (__bridge id<MTLTexture>)_Texture_MTLTexture(context->texture);
        pass_descriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    }

    context->command_buffer = [context->command_queue commandBuffer];
    context->command_encoder = [context->command_buffer renderCommandEncoderWithDescriptor: pass_descriptor];
}

void _Context_end_paint(_Context* context) {
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

_Size _Context_size(_Context const* context) {
    _ASSERT(context != NULL);
    if (context->window != NULL) {
        _ASSERT(context->layer != NULL);
        return (_Size){
            .width = (context->window.contentView.frame.size.width * context->window.backingScaleFactor),
            .height = (context->window.contentView.frame.size.height * context->window.backingScaleFactor),
        };
        // return (_Size){
        //     .width = (context->window.contentView.frame.size.width * 0.25),
        //     .height = (context->window.contentView.frame.size.height * 0.25),
        // };
    } else {
        return _Texture_size(context->texture);
    }
}

_Rect _Context_clip(_Context const* context) {
    _ASSERT(context != NULL);
    return context->clip;
}

_CONTEXT_ORIGIN _Context_origin(_Context const* context) {
    _ASSERT(context != NULL);
    return context->origin;
}

void _Context_set_clip(_Context* context, _Rect const* rect) {
    _ASSERT(context != NULL);

    if ((rect == NULL)
    || (rect->size.width <= 0)
   	|| (rect->size.height <= 0)) {
        context->clip = (_Rect){ { 0, 0 }, { 0, 0 } };
   	} else {
        context->clip = *rect;
   	}
}

void _Context_set_origin(_Context* context, _CONTEXT_ORIGIN origin) {
    _ASSERT(context != NULL);
    context->origin = origin;
}

void _Context_draw_vertices(_Context const* context, float const* array, int size, bool strip, _Brush const* brush) {
    _ASSERT(context != NULL);
    _ASSERT(array != NULL);
    _ASSERT(size > 0);
    _ASSERT(brush != NULL);
    _ASSERT(context->command_encoder != NULL);
    _ASSERT((context->size.width > 0) && (context->size.height > 0));
    _ASSERT(context->painting == true);

    _Color const* color = _Brush_color(brush);

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

void _Context_draw_texture(_Context const* context, _Texture const* texture, _RectF const* src, _RectF const* dst, _Color const* tint) {
	_ASSERT(src != NULL);
	_ASSERT(dst != NULL);
	_ASSERT(texture != NULL);
    _ASSERT(context != NULL);
    _ASSERT(context->command_encoder != NULL);
    _ASSERT(context->painting == true);
    _ASSERT(_Texture_MTLTexture(texture) != NULL);

    _Size size = _Texture_size(texture);
    _ASSERT(size.width > 0);
    _ASSERT(size.height > 0);

    if ((dst->origin.x > context->size.width)
    || (dst->origin.y > context->size.height)
    || ((dst->origin.x + dst->size.width) < 0)
    || ((dst->origin.y + dst->size.height) < 0))
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

    float uv_x = (src->origin.x / (float)size.width);
    float uv_y = (src->origin.y / (float)size.height);
    float uv_max_x = ((src->origin.x + src->size.width) / (float)size.width);
    float uv_max_y = ((src->origin.y + src->size.height) / (float)size.height);

    if (context->origin == 1.0f) {
        uv_y = (1 - uv_y);
        uv_max_y = (1 - uv_max_y);
    }

    float array[16] = {
        dst->origin.x,
        dst->origin.y,
        uv_x,
        uv_y,

        (dst->origin.x + dst->size.width),
        dst->origin.y,
        uv_max_x,
        uv_y,

        dst->origin.x,
        (dst->origin.y + dst->size.height),
        uv_x,
        uv_max_y,

        (dst->origin.x + dst->size.width),
        (dst->origin.y + dst->size.height),
        uv_max_x,
        uv_max_y

    };

    [context->command_encoder setRenderPipelineState: context->texture_pipeline_state];
    [context->command_encoder setVertexBytes: &uniforms length: (sizeof(float) * 8) atIndex: 0];
    [context->command_encoder setVertexBytes: &array length: (sizeof(float) * 16) atIndex: 1];
    [context->command_encoder setFragmentTexture: (__bridge id<MTLTexture>)_Texture_MTLTexture(texture) atIndex: 0];

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

void _Context_stroke_line(_Context const* context, _PointF const* from, _PointF const* to, double width, _LINE_CAP cap, _Brush const* brush, _Transform const* transform) {
    _ASSERT(context != NULL);
    _ASSERT(from != NULL);
    _ASSERT(to != NULL);
    _ASSERT(brush != NULL);
}

void _Context_stroke_rect(_Context const* context, _RectF const* rect, double width, _Brush const* brush, _Transform const* transform) {
    _ASSERT(context != NULL);
    _ASSERT(rect != NULL);
    _ASSERT(brush != NULL);

    /* NOTE: Borders are being generated clock-wise for TopLeft origin and opposite for BottomLeft */

    _RectF border1 = {
        .origin = {
            .x = (rect->origin.x - (width * 0.5)),
            .y = (rect->origin.y - (width * 0.5))
        },
        .size = {
            .width = (rect->size.width + width),
            .height = width
        }
    };

    _RectF border2 = {
        .origin = {
            .x = (_RECT_MAX_X(*rect) - (width * 0.5)),
            .y = rect->origin.y + (width * 0.5)
        },
        .size = {
            .width = width,
            .height = (rect->size.height - width)
        }
    };
    _RectF border3 = {
        .origin = {
            .x = (rect->origin.x - (width * 0.5)),
            .y = (_RECT_MAX_Y(*rect) - (width * 0.5))
        },
        .size = {
            .width = (rect->size.width + width),
            .height = width
        }
    };
    _RectF border4 = {
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

    _Context_draw_vertices(context, vertices, (12 * 4), false, brush);
}

void _Context_stroke_path(_Context const* context, _BezierPath const* path, double width, _Brush const* brush, _Transform const* transform) {

}

void _Context_stroke_ellipse(_Context const* context, _RectF const* rect, double width, _Brush const* brush, _Transform const* transform) {

}

void _Context_fill_rect(_Context const* context, _RectF const* rect, _Brush const* brush, _Transform const* transform) {
    _ASSERT(context != NULL);
    _ASSERT(rect != NULL);
    _ASSERT(brush != NULL);

    _RectF rect_ = *rect;

    if (transform != NULL)
        _RECT_TRANSFORM(rect_, *transform);

    float vertices[8];
    _RECT_TRIANGLES_STRIP(rect_, vertices);

    _Context_draw_vertices(context, vertices, 8, true, brush);
}

void _Context_fill_path(_Context const* context, _BezierPath const* path, _Brush const* brush, _Transform const* transform) {

}

void _Context_fill_ellipse(_Context const* context, _RectF const* rect, _Brush const* brush, _Transform const* transform) {

}
