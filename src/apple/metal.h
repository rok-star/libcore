#ifndef _LIBCORE_METAL_H
#define _LIBCORE_METAL_H

#include <Cocoa/Cocoa.h>
#include <MetalKit/MetalKit.h>
#include <QuartzCore/CoreAnimation.h>

extern char const* __metal_source;
extern id<MTLDevice> __metal_device;
extern id<MTLLibrary> __metal_library;

#endif /* _LIBCORE_METAL_H */
