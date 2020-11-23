#include <libcore/MACRO.h>
#include "metal.h"

char const* __metal_source = "#include <metal_stdlib> \n\
                            #include <simd/simd.h> \n\
                            using namespace metal; \n\
                            struct uniforms_t { \n\
                                float4 color; \n\
                                float2 viewport; \n\
                                float origin; \n\
                                float tint; \n\
                            }; \n\
                            struct vertex_color_out_t { \n\
                                float4 position [[position]]; \n\
                                float4 color; \n\
                            }; \n\
                            vertex vertex_color_out_t vertex_color_shader( \n\
                                const constant uniforms_t& uniforms [[buffer(0)]], \n\
                                const constant vector_float2 *pvertex_array [[buffer(1)]], \n\
                                unsigned int vid [[vertex_id]] \n\
                            ) { \n\
                                return { \n\
                                    .color = uniforms.color, \n\
                                    .position = float4( \n\
                                        pvertex_array[vid].x / (uniforms.viewport.x / 2) - 1.0, \n\
                                        (pvertex_array[vid].y / (uniforms.viewport.y / 2) - 1.0) * uniforms.origin, \n\
                                        1.0, \n\
                                        1.0 \n\
                                    ) \n\
                                }; \n\
                            } \n\
                            fragment vector_float4 fragment_color_shader( \n\
                                vertex_color_out_t interpolated [[stage_in]] \n\
                            ) { \n\
                                return interpolated.color; \n\
                            } \n\
                            struct vertex_texture_out_t { \n\
                                float4 position [[position]]; \n\
                                float2 texcoords; \n\
                                float4 color; \n\
                                float tint; \n\
                            }; \n\
                            vertex vertex_texture_out_t vertex_texture_shader( \n\
                                const constant uniforms_t& uniforms [[buffer(0)]], \n\
                                const constant vector_float4 *pvertex_array [[buffer(1)]], \n\
                                unsigned int vid [[vertex_id]] \n\
                            ) { \n\
                                return { \n\
                                    .texcoords = float2(pvertex_array[vid].z, pvertex_array[vid].w), \n\
                                    .position = float4( \n\
                                        pvertex_array[vid].x / (uniforms.viewport.x / 2) - 1.0, \n\
                                        (pvertex_array[vid].y / (uniforms.viewport.y / 2) - 1.0) * uniforms.origin, \n\
                                        1.0, \n\
                                        1.0 \n\
                                    ), \n\
                                    .color = uniforms.color, \n\
                                    .tint = uniforms.tint \n\
                                }; \n\
                            } \n\
                            fragment vector_float4 fragment_texture_shader( \n\
                                vertex_texture_out_t interpolated [[stage_in]], \n\
                                texture2d<float> texture [[texture(0)]] \n\
                            ) { \n\
                                constexpr sampler nearestSampler(mag_filter::nearest, min_filter::nearest); \n\
                                vector_float4 color = texture.sample(nearestSampler, interpolated.texcoords); \n\
                                if ((interpolated.tint > 0) && (color[3] > 0)) \n\
                                    color = interpolated.color; \n\
                                return color; \n\
                            }";

id<MTLDevice> __metal_device = NULL;
id<MTLLibrary> __metal_library = NULL;

__attribute__((constructor))
void __metal_init(void) {
    NSError* _err = NULL;
    __metal_device = MTLCreateSystemDefaultDevice();
    __metal_library = [__metal_device newLibraryWithSource: [NSString stringWithUTF8String: __metal_source]
                                                   options: [[MTLCompileOptions alloc] init]
                                                     error: &_err];
    if (_err != NULL)
        _ABORT("%s", [[_err localizedDescription] cStringUsingEncoding: NSUTF8StringEncoding]);
}

__attribute__((destructor))
void __metal_done(void) {
    _ASSERT(__metal_device != NULL);
    _ASSERT(__metal_library != NULL);
    __metal_device = NULL;
    __metal_library = NULL;
}
