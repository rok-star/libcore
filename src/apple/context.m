#include <libcore/window.h>
#include <libcore/context.h>
#include <libcore/MACRO.h>

#include "metal.h"

typedef struct _Context {
    NSWindow* window;
    _Texture* texture;
    CAMetalLayer* layer;
    id<CAMetalDrawable> drawable;
    id<MTLCommandQueue> command_queue;
    id<MTLCommandBuffer> command_buffer;
    id<MTLRenderCommandEncoder> command_encoder;
    id<MTLRenderPipelineState> color_pipeline_state;
    id<MTLRenderPipelineState> texture_pipeline_state;
    int width;
    int height;
    float origin;
    bool clip_rect;
    int clip_rect_x;
    int clip_rect_y;
    int clip_rect_width;
    int clip_rect_height;
    bool painting;
} _Context;

_Context* _Context_create(_CONTEXT_TYPE type, void* target) {
	_ASSERT(__metal_device != NULL);
    _ASSERT(__metal_library != NULL);

    _Context* context = _NEW(_Context, {});

    if (type == _WINDOW_CONTEXT_TYPE) {
        context->window = (__bridge NSWindow*)_Window_NSWindow((_Window*)target);
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
        context->texture = (_Texture*)target;
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

void _Context_destroy(_Context* context) {
    _ASSERT(context != NULL);

    if (context->window != NULL)
        [context->layer removeFromSuperlayer];

    context->layer = NULL;
    context->window = NULL;
    context->texture = NULL;
    context->drawable = NULL;
    context->command_queue = NULL;
    context->command_buffer = NULL;
    context->command_encoder = NULL;
    context->color_pipeline_state = NULL;
    context->texture_pipeline_state = NULL;
}

void _Context_begin_paint(_Context* context) {
	_ASSERT(context != NULL);
    _ASSERT(context->command_queue != NULL);
    _ASSERT(context->color_pipeline_state != NULL);
    _ASSERT(context->painting == false);

    context->painting = true;

    if ((context->origin != -1)
    && (context->origin != 1))
    	context->origin = -1;

    context->clip_rect_x = 0;
    context->clip_rect_y = 0;
    context->clip_rect_width = 0;
    context->clip_rect_height = 0;
    context->clip_rect = false;

    MTLRenderPassDescriptor* pass_descriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    pass_descriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 0, 0, 0);
    pass_descriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    pass_descriptor.colorAttachments[0].storeAction = MTLStoreActionStore;

    if (context->window != NULL) {
        _ASSERT(context->layer != NULL);

        NSSize size = context->window.contentView.frame.size;
		//size.width *= context->window.backingScaleFactor;
		//size.height *= context->window.backingScaleFactor;

        [CATransaction begin];
        [CATransaction setValue: (id)kCFBooleanTrue forKey: kCATransactionDisableActions];
        [context->layer setFrame: context->window.contentView.frame];
        [context->layer setDrawableSize: size];
        [CATransaction commit];

        context->width = size.width;
        context->height = size.height;
        _ASSERT(context->width > 0);
        _ASSERT(context->height > 0);

        context->drawable = [context->layer nextDrawable];
        pass_descriptor.colorAttachments[0].texture = context->drawable.texture;
    } else {
    	_Size size = _Texture_size(context->texture);
        context->width = size.width;
        context->height = size.height;
        _ASSERT(context->width > 0);
        _ASSERT(context->height > 0);

        pass_descriptor.colorAttachments[0].texture = (__bridge id<MTLTexture>)_Texture_MTLTexture(context->texture);
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

void _Context_set_clip_rect(_Context* context, _Rect const* rect) {
    _ASSERT(context != NULL);

    if ((rect == NULL)
    || (rect->size.width <= 0)
   	|| (rect->size.height <= 0)) {
		context->clip_rect_x = 0;
	    context->clip_rect_y = 0;
	    context->clip_rect_width = 0;
	    context->clip_rect_height = 0;
	    context->clip_rect = false;
   	} else {
   		context->clip_rect_x = rect->origin.x;
	    context->clip_rect_y = rect->origin.y;
	    context->clip_rect_width = rect->size.width;
	    context->clip_rect_height = rect->size.height;
	    context->clip_rect = true;
   	}
}

void _Context_set_origin(_Context* context, _CONTEXT_ORIGIN origin) {
    _ASSERT(context != NULL);
    context->origin = ((origin == _LEFTTOP_CONTEXT_ORIGIN) ? -1 : 1);
}

void _Context_draw_texture(_Context* context, _RectF const* src, _RectF const* dst, _Texture const* texture, _Color const* tint) {
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

    if ((dst->origin.x > context->width)
    || (dst->origin.y > context->height)
    || ((dst->origin.x + dst->size.width) < 0)
    || ((dst->origin.y + dst->size.height) < 0))
        return;

    float uniforms[8] = {
        ((tint != NULL) ? tint->red : 0),
        ((tint != NULL) ? tint->green : 0),
        ((tint != NULL) ? tint->blue : 0),
        ((tint != NULL) ? tint->alpha : 0),
        context->width,
        context->height,
        context->origin,
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

    if (context->clip_rect) {
        [context->command_encoder setScissorRect: (MTLScissorRect){
        	context->clip_rect_x,
            context->clip_rect_y,
            context->clip_rect_width,
            context->clip_rect_height
        }];
    }
    [context->command_encoder drawPrimitives: MTLPrimitiveTypeTriangleStrip vertexStart: 0 vertexCount: 4];
}

void _Context_draw_vertices(_Context* context, bool strip, float const* array, int size, _Color const* color) {
	_ASSERT(context != NULL);
	_ASSERT(array != NULL);
	_ASSERT(color != NULL);
	_ASSERT(size > 0);
	_ASSERT(context->command_encoder != NULL);
    _ASSERT((context->width > 0) && (context->height > 0));
    _ASSERT(context->painting == true);

    float uniforms[8] = {
        color->red,
        color->green,
        color->blue,
        color->alpha,
        context->width,
        context->height,
        context->origin,
        0
    };

    [context->command_encoder setRenderPipelineState: context->color_pipeline_state];
    [context->command_encoder setVertexBytes: &uniforms length: (sizeof(float) * 8) atIndex: 0];
    [context->command_encoder setVertexBytes: array length: (sizeof(float) * size) atIndex: 1];
    if (context->clip_rect) {
        [context->command_encoder setScissorRect: (MTLScissorRect){
        	context->clip_rect_x,
            context->clip_rect_y,
            context->clip_rect_width,
            context->clip_rect_height
        }];
    }
    [context->command_encoder drawPrimitives: (strip ? MTLPrimitiveTypeTriangleStrip : MTLPrimitiveTypeTriangle)
                                  vertexStart: 0
                                  vertexCount: (size / 2)];
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

void _Context_stroke_rect(_Context* context, _RectF const* rect, double width, _Brush const* brush) {
    _ASSERT(context != NULL);
    _ASSERT(rect != NULL);
    _ASSERT(color != NULL);

    /* NOTE: Borders are being generated clock-wise for TopLeft origin and opposite for BottomLeft */

    _RectF border1 = {
        .origin = {
            .x = rect->origin.x,
            .y = rect->origin.y
        },
        .size = {
            .width = rect->size.width,
            .height = width
        }
    };
    _RectF border2 = {
        .origin = {
            .x = (_RECT_MAX_X(*rect) - width),
            .y = rect->origin.y + width
        },
        .size = {
            .width = width,
            .height = (rect->size.height - width)
        }
    };
    _RectF border3 = {
        .origin = {
            .x = rect->origin.x,
            .y = (_RECT_MAX_Y(*rect) - width)
        },
        .size = {
            .width = (rect->size.width - width),
            .height = width
        }
    };
    _RectF border4 = {
        .origin = {
            .x = rect->origin.x,
            .y = (rect->origin.y + width)
        },
        .size = {
            .width = width,
            .height = (rect->size.height - (width * 2))
        }
    };

    float vertices[(12 * 4)];
    float* _dst = vertices;
    _RECT_TRIANGLES(border1, _dst); _dst += 12;
    _RECT_TRIANGLES(border2, _dst); _dst += 12;
    _RECT_TRIANGLES(border3, _dst); _dst += 12;
    _RECT_TRIANGLES(border4, _dst);

    if (_Brush_type(brush) == _COLOR_BRUSH_TYPE) {
        _Context_draw_vertices(context, false, vertices, (12 * 4), _Brush_color(brush));
    } else {
        _ABORT("_Context_stroke_rect: Only color brushes supported now");
    }
}

void _Context_fill_rect(_Context* context, _RectF const* rect, _Brush const* brush) {
	_ASSERT(context != NULL);
	_ASSERT(rect != NULL);
	_ASSERT(color != NULL);

	float vertices[8];
	_RECT_TRIANGLES_STRIP(*rect, vertices);

    if (_Brush_type(brush) == _COLOR_BRUSH_TYPE) {
        _Context_draw_vertices(context, true, vertices, 8, _Brush_color(brush));
    } else {
        _ABORT("_Context_fill_rect: Only color brushes supported now");
    }
}
