#include <Cocoa/Cocoa.h>
#include <libcore/MACRO.h>
#include <libcore/window.h>

@interface __Window : NSWindow<NSWindowDelegate>

- (id)initWithWindow:(_window_t*)window;

@end

typedef struct _window_t {
    __Window* pNSWindow;
    void (*on_event)(_window_event_t const*,void*);
    void* param;
    _size_t size;
    _point_t origin;
} _window_t;

@implementation __Window {
    _window_t* _window;
}

- (id)initWithWindow:(_window_t*)window {
    self = [super init];
    if (self) {
        _window = window;
        [self setDelegate: self];
    }
    return self;
}

- (void)triggerEventBasic:(_WINDOW_EVENT) type {
    if (_window->on_event != NULL) {
        _window->on_event(
            &(_window_event_t){
                .type = type,
                .window = _window
            },
            _window->param
        );
    }
}

- (void)triggerEventMouse:(_WINDOW_EVENT)type event:(NSEvent*)event {
    _ASSERT(event != NULL);
    if (_window->on_event != NULL) {
        NSPoint point = [event locationInWindow];
        _window->on_event(
            &(_window_event_t){
                .type = type,
                .window = _window,
                .mouse_info = (_mouse_info_t){
                    .position = (_point_t){
                        .x = point.x,
                        .y = (self.contentView.frame.size.height - point.y)
                    },
                    .wheel = 0
                }
            },
            _window->param
        );
    }
}


- (void)triggerEventKey:(_WINDOW_EVENT)type event:(NSEvent*)event {
    _ASSERT(event != NULL);
    if (_window->on_event != NULL) {
        _ASSERT(event.keyCode < 256);
        _window->on_event(
            &(_window_event_t){
                .type = type,
                .window = _window,
                .key_info = (_key_info_t){
                    .key = _KEY_FROM_NATIVE[event.keyCode],
                    .shift = (event.modifierFlags & NSEventModifierFlagShift),
                    .control = (event.modifierFlags & NSEventModifierFlagControl),
                    .option = (event.modifierFlags & NSEventModifierFlagOption),
                    .super = (event.modifierFlags & NSEventModifierFlagCommand)
                }
            },
            _window->param
        );
    }
}

- (void)mouseDown:(NSEvent*)event {
    [self triggerEventMouse: _LBUTTONDOWN_WINDOW_EVENT event: event];
}

- (void)rightMouseDown:(NSEvent*)event {
    [self triggerEventMouse: _RBUTTONDOWN_WINDOW_EVENT event: event];
}

- (void)otherMouseDown:(NSEvent*)event {
    /* */
}

- (void)mouseUp:(NSEvent*)event {
    [self triggerEventMouse: _LBUTTONUP_WINDOW_EVENT event: event];
}

- (void)rightMouseUp:(NSEvent*)event {
    [self triggerEventMouse: _RBUTTONUP_WINDOW_EVENT event: event];
}

- (void)otherMouseUp:(NSEvent*)event {
    /* */
}

- (void)mouseMoved:(NSEvent*)event {
    [self triggerEventMouse: _MOUSEMOVE_WINDOW_EVENT event: event];
}

- (void)mouseDragged:(NSEvent*)event {
    /* */
}

- (void)scrollWheel:(NSEvent*)event {
    /* */
}

- (void)rightMouseDragged:(NSEvent*)event {
    /* */
}

- (void)otherMouseDragged:(NSEvent*)event {
    /* */
}

- (void)mouseEntered:(NSEvent*)event {
    /* */
}

- (void)mouseExited:(NSEvent*)event {
    /* */
}

- (void)keyDown:(NSEvent*)event {
    [self triggerEventKey: _KEYDOWN_WINDOW_EVENT event: event];
}

- (void)keyUp:(NSEvent*)event {
    [self triggerEventKey: _KEYUP_WINDOW_EVENT event: event];
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
    [self triggerEventBasic: _CLOSE_WINDOW_EVENT];
    return NO;
}

- (void)windowWillClose:(NSWindow *)sender {
    if (self.visible)
        [self triggerEventBasic: _HIDE_WINDOW_EVENT];
}

- (void)windowDidResize:(NSNotification*)notification {
    [self triggerEventBasic: _SIZE_WINDOW_EVENT];
}

- (void)windowDidMiniaturize:(NSNotification*)notification {
    [self triggerEventBasic: _MINIMIZE_WINDOW_EVENT];
}

- (void)windowDidDeminiaturize:(NSNotification*)notification {
    [self triggerEventBasic: _DEMINIMIZE_WINDOW_EVENT];
}

- (void)windowDidEnterFullScreen:(NSNotification*)notification {
    [self triggerEventBasic: _MAXIMIZE_WINDOW_EVENT];
}

- (void)windowDidExitFullScreen:(NSNotification*)notification {
    [self triggerEventBasic: _DEMAXIMIZE_WINDOW_EVENT];
}

- (void)windowDidChangeBackingProperties:(NSNotification*)notification {
    [self triggerEventBasic: _PIXELRATIO_WINDOW_EVENT];
    [self triggerEventBasic: _SIZE_WINDOW_EVENT];
}

@end

_window_t* _window_create(void) {
    _window_t* window = _NEW(_window_t, {});
    __Window* pNSWindow = [[__Window alloc] initWithWindow: window];
    [pNSWindow setStyleMask: NSWindowStyleMaskTitled];
    [pNSWindow setReleasedWhenClosed: NO];
    [pNSWindow setAcceptsMouseMovedEvents: YES];
    [pNSWindow center];
    window->pNSWindow = pNSWindow;
    return window;
}

void _window_destroy(_window_t* window) {
    _ASSERT(window != NULL);
    window->on_event = NULL;
    [window->pNSWindow setReleasedWhenClosed: YES];
    [window->pNSWindow close];
    _FREE(window);
}

void _window_set_visible(_window_t* window, bool value) {
    _ASSERT(window != NULL);
    if (value) {
        if (!window->pNSWindow.visible) {
            [window->pNSWindow makeKeyAndOrderFront: nil];
            [window->pNSWindow makeFirstResponder: window->pNSWindow.contentView];
            [window->pNSWindow triggerEventBasic: _SHOW_WINDOW_EVENT];
        }
    } else {
        if (window->pNSWindow.visible) {
            [window->pNSWindow close];
        }
    }
}

void _window_set_sizable(_window_t* window, bool value) {
    _ASSERT(window != NULL);
    if (value == (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskResizable))
        return;
    if (value) {
        window->pNSWindow.styleMask |= NSWindowStyleMaskResizable;
    } else {
        window->pNSWindow.styleMask &= ~NSWindowStyleMaskResizable;
    }
}

void _window_set_closable(_window_t* window, bool value) {
    _ASSERT(window != NULL);
    if (value == (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskClosable))
        return;
    if (value) {
        window->pNSWindow.styleMask |= NSWindowStyleMaskClosable;
    } else {
        window->pNSWindow.styleMask &= ~NSWindowStyleMaskClosable;
    }
}

void _window_set_maximizable(_window_t* window, bool value) {
    _ASSERT(window != NULL);
    if (value == (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskResizable))
        return;
    if (value) {
        window->pNSWindow.styleMask |= NSWindowStyleMaskResizable;
    } else {
        window->pNSWindow.styleMask &= ~NSWindowStyleMaskResizable;
    }
}

void _window_set_minimizable(_window_t* window, bool value) {
    _ASSERT(window != NULL);
    if (value == (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskMiniaturizable))
        return;
    if (value) {
        window->pNSWindow.styleMask |= NSWindowStyleMaskMiniaturizable;
    } else {
        window->pNSWindow.styleMask &= ~NSWindowStyleMaskMiniaturizable;
    }
}

void _window_set_maximized(_window_t* window, bool value) {
    _ASSERT(window != NULL);
    if (value == (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskFullScreen))
        return;
    [window->pNSWindow toggleFullScreen: window->pNSWindow];
}

void _window_set_minimized(_window_t* window, bool value) {
    _ASSERT(window != NULL);
    if (value == window->pNSWindow.miniaturized)
        return;
    if (value) {
        [window->pNSWindow miniaturize: nil];
    } else {
        [window->pNSWindow deminiaturize: nil];
    }
}

void _window_set_topmost(_window_t* window, bool value) {
    _ASSERT(window != NULL);
    [window->pNSWindow setLevel: value ? NSStatusWindowLevel : NSNormalWindowLevel];
}

void _window_set_origin(_window_t* window, _point_t const* value) {
    _ASSERT(window != NULL);
    _ASSERT(value != NULL);
    ;
}

void _window_set_size(_window_t* window, _size_t const* value) {
    _ASSERT(window != NULL);
    _ASSERT(value != NULL);
    NSRect rect = [window->pNSWindow frame];
    rect.size.width = value->width;
    rect.size.height = value->height + (window->pNSWindow.frame.size.height
                                     - [window->pNSWindow contentRectForFrameRect:
                                            window->pNSWindow.frame].size.height);
    [window->pNSWindow setFrame: rect display: YES];
}

void _window_set_text(_window_t* window, char const* value) {
    _ASSERT(window != NULL);
    [window->pNSWindow setTitle: [NSString stringWithUTF8String: value]];
}

bool _window_visible(_window_t const* window) {
    _ASSERT(window != NULL);
    return window->pNSWindow.visible;
}

bool _window_closable(_window_t const* window) {
    _ASSERT(window != NULL);
    return (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskClosable);
}

bool _window_sizable(_window_t const* window) {
    _ASSERT(window != NULL);
    return (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskResizable);
}

bool _window_maximizable(_window_t const* window) {
    _ASSERT(window != NULL);
    return (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskResizable);
}

bool _window_minimizable(_window_t const* window) {
    _ASSERT(window != NULL);
    return (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskMiniaturizable);
}

bool _window_maximized(_window_t const* window) {
    _ASSERT(window != NULL);
    return (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskFullScreen);
}

bool _window_minimized(_window_t const* window) {
    _ASSERT(window != NULL);
    return window->pNSWindow.miniaturized;
}

bool _window_topmost(_window_t const* window) {
    _ASSERT(window != NULL);
    return window->pNSWindow.level == NSStatusWindowLevel;
}

_point_t const* _window_origin(_window_t const* window) {
    _ASSERT(window != NULL);
    ((_window_t*)window)->origin = (_point_t){
        .x = window->pNSWindow.contentView.frame.origin.x,
        .y = window->pNSWindow.contentView.frame.origin.y
    };
    return &window->origin;
}

_size_t const* _window_size(_window_t const* window) {
    _ASSERT(window != NULL);
    ((_window_t*)window)->size = (_size_t){
        .width = window->pNSWindow.contentView.frame.size.width,
        .height = window->pNSWindow.contentView.frame.size.height
    };
    return &window->size;
}

char const* _window_text(_window_t const* window) {
    _ASSERT(window != NULL);
    return [window->pNSWindow.title cStringUsingEncoding: NSUTF8StringEncoding];
}

float _window_pixelratio(_window_t const* window) {
    _ASSERT(window != NULL);
    return window->pNSWindow.backingScaleFactor;
}

void _window_on_event(_window_t* window, void (*on_event)(_window_event_t const*,void*), void* param) {
    _ASSERT(window != NULL);
    window->on_event = on_event;
    window->param = param;
}

void* _window_NSWindow(_window_t const* window) {
    _ASSERT(window != NULL);
    return (__bridge void*)window->pNSWindow;
}
