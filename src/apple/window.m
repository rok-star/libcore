#include <Cocoa/Cocoa.h>
#include <libcore/MACRO.h>
#include <libcore/window.h>

@interface __Window : NSWindow<NSWindowDelegate>

- (id)initWithWindow:(_Window*)window;

@end

typedef struct _Window {
	__Window* pNSWindow;
	void (*on_event)(_WindowEvent const*,void*);
	void* param;
} _Window;

@implementation __Window {
	_Window* _window;
}

- (id)initWithWindow:(_Window*)window {
	self = [super init];
	if (self) {
		_window = window;
		[self setDelegate: self];
	}
	return self;
}

- (void)triggerEventBasic:(_WINDOW_EVENT) type {
	_ASSERT(event != NULL);
	if (_window->on_event != NULL) {
		_window->on_event(
			&(_WindowEvent){
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
			&(_WindowEvent){
				.type = type,
				.window = _window,
				.mouse_info = (_MouseInfo){
					.position = (_Point){
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
			&(_WindowEvent){
				.type = type,
				.window = _window,
				.key_info = (_KeyInfo){
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
	/* */
}

@end

_Window* _Window_create(void) {
	_Window* window = _NEW(_Window, {});
    __Window* pNSWindow = [[__Window alloc] initWithWindow: window];
    [pNSWindow setStyleMask: NSWindowStyleMaskTitled];
	[pNSWindow setReleasedWhenClosed: NO];
	[pNSWindow setAcceptsMouseMovedEvents: YES];
	[pNSWindow center];
	window->pNSWindow = pNSWindow;
	return window;
}

void _Window_destroy(_Window* window) {
	_ASSERT(window != NULL);
	window->on_event = NULL;
	[window->pNSWindow setReleasedWhenClosed: YES];
	[window->pNSWindow close];
	_FREE(window);
}

void _Window_set_visible(_Window* window, bool value) {
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

void _Window_set_sizable(_Window* window, bool value) {
	_ASSERT(window != NULL);
	if (value == (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskResizable))
		return;
	if (value) {
		window->pNSWindow.styleMask |= NSWindowStyleMaskResizable;
	} else {
		window->pNSWindow.styleMask &= ~NSWindowStyleMaskResizable;
	}
}

void _Window_set_closable(_Window* window, bool value) {
	_ASSERT(window != NULL);
	if (value == (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskClosable))
		return;
	if (value) {
		window->pNSWindow.styleMask |= NSWindowStyleMaskClosable;
	} else {
		window->pNSWindow.styleMask &= ~NSWindowStyleMaskClosable;
	}
}

void _Window_set_maximizable(_Window* window, bool value) {
	_ASSERT(window != NULL);
	if (value == (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskResizable))
		return;
	if (value) {
		window->pNSWindow.styleMask |= NSWindowStyleMaskResizable;
	} else {
		window->pNSWindow.styleMask &= ~NSWindowStyleMaskResizable;
	}
}

void _Window_set_minimizable(_Window* window, bool value) {
	_ASSERT(window != NULL);
	if (value == (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskMiniaturizable))
		return;
	if (value) {
		window->pNSWindow.styleMask |= NSWindowStyleMaskMiniaturizable;
	} else {
		window->pNSWindow.styleMask &= ~NSWindowStyleMaskMiniaturizable;
	}
}

void _Window_set_maximized(_Window* window, bool value) {
	_ASSERT(window != NULL);
	if (value == (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskFullScreen))
		return;
	[window->pNSWindow toggleFullScreen: window->pNSWindow];
	puts("YES");
}

void _Window_set_minimized(_Window* window, bool value) {
	_ASSERT(window != NULL);
	if (value == window->pNSWindow.miniaturized)
		return;
	if (value) {
		[window->pNSWindow miniaturize: nil];
	} else {
		[window->pNSWindow deminiaturize: nil];
	}
}

void _Window_set_topmost(_Window* window, bool value) {
	_ASSERT(window != NULL);
	[window->pNSWindow setLevel: value ? NSStatusWindowLevel : NSNormalWindowLevel];
}

void _Window_set_size(_Window* window, _Size const* value) {
	_ASSERT(window != NULL);
	_ASSERT(value != NULL);
	NSRect rect = [window->pNSWindow frame];
	rect.size.width = value->width;
	rect.size.height = value->height + (window->pNSWindow.frame.size.height
									 - [window->pNSWindow contentRectForFrameRect:
											window->pNSWindow.frame].size.height);
	[window->pNSWindow setFrame: rect display: YES];
}

void _Window_set_text(_Window* window, char const* value) {
	_ASSERT(window != NULL);
	[window->pNSWindow setTitle: [NSString stringWithUTF8String: value]];
}

bool _Window_visible(_Window* window) {
	_ASSERT(window != NULL);
	return window->pNSWindow.visible;
}

bool _Window_closable(_Window* window) {
	_ASSERT(window != NULL);
	return (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskClosable);
}

bool _Window_sizable(_Window* window) {
	_ASSERT(window != NULL);
	return (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskResizable);
}

bool _Window_maximizable(_Window* window) {
	_ASSERT(window != NULL);
	return (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskResizable);
}

bool _Window_minimizable(_Window* window) {
	_ASSERT(window != NULL);
	return (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskMiniaturizable);
}

bool _Window_maximized(_Window* window) {
	_ASSERT(window != NULL);
	return (bool)(window->pNSWindow.styleMask & NSWindowStyleMaskFullScreen);
}

bool _Window_minimized(_Window* window) {
	_ASSERT(window != NULL);
	return window->pNSWindow.miniaturized;
}

bool _Window_topmost(_Window* window) {
	_ASSERT(window != NULL);
	return window->pNSWindow.level == NSStatusWindowLevel;
}

_Size _Window_size(_Window* window) {
	_ASSERT(window != NULL);
	return (_Size){
		.width = window->pNSWindow.contentView.frame.size.width,
		.height = window->pNSWindow.contentView.frame.size.height
	};
}

char* _Window_text(_Window* window) {
	_ASSERT(window != NULL);
	return [window->pNSWindow.title cStringUsingEncoding: NSUTF8StringEncoding];
}

void _Window_on_event(_Window* window, void (*on_event)(_WindowEvent const*,void*), void* param) {
	_ASSERT(window != NULL);
	window->on_event = on_event;
	window->param = param;
}

void* _Window_NSWindow(_Window* window) {
	_ASSERT(window != NULL);
	return (__bridge void*)window->pNSWindow;
}
