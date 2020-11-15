#include <Cocoa/Cocoa.h>
#include <libcore/MACRO.h>
#include <libcore/app.h>

@interface __Delegate : NSObject<NSApplicationDelegate>

@property (nonatomic, strong) void (^onRun)(void);
@property (nonatomic, strong) void (^onExit)(void);

@end

@implementation __Delegate

- (void)applicationDidFinishLaunching:(NSNotification*)notification {
	if (self.onRun != NULL)
		self.onRun();
}

- (void)applicationWillTerminate:(NSNotification*)notification {
	if (self.onExit != NULL)
		self.onExit();
}

@end

void (*__on_event)(_AppEvent const*,void*) = NULL;
void* __param = NULL;
bool __running = false;

void _App_run() {
	_ASSERT(__running == false);

	__Delegate* delegate = [[__Delegate alloc] init];
	delegate.onRun = ^{
		NSMenu* mainMenu = [[NSMenu alloc] init];
		NSMenu* appMenu = [[NSMenu alloc] init];
		NSMenuItem* appMenuItem = [mainMenu addItemWithTitle: [NSString stringWithCString: "123" encoding: NSUTF8StringEncoding] action: nil keyEquivalent: @""];
		[appMenuItem setSubmenu: appMenu];
		[appMenu addItemWithTitle: @"Quit" action: @selector(stop:) keyEquivalent: @"q"];
		[NSApp setMainMenu: mainMenu];
		[NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];

		if (__on_event != NULL)
			__on_event(&(_AppEvent){ .type = _RUN_APP_EVENT }, __param);
	};
	delegate.onExit = ^{
		if (__on_event != NULL)
			__on_event(&(_AppEvent){ .type = _EXIT_APP_EVENT }, __param);
	};

	[[NSApplication sharedApplication] setDelegate: delegate];

	CFRunLoopAddObserver(
		CFRunLoopGetCurrent(),
		CFRunLoopObserverCreateWithHandler(
			kCFAllocatorDefault,
			kCFRunLoopBeforeWaiting,
			YES,
			0,
			^(CFRunLoopObserverRef a, CFRunLoopActivity b) {
				if (__on_event != NULL)
					__on_event(&(_AppEvent){ .type = _SPIN_APP_EVENT }, __param);
			}
		),
		kCFRunLoopDefaultMode
	);

	__running = true;

	[NSApp run];

	__running = false;
}

void _App_exit(void) {
	_ASSERT(__running == true);

	dispatch_async(dispatch_get_main_queue(), ^{
		[NSApp stop: NULL];
	});
}

void _App_wakeup(void) {
	_ASSERT(__running == true);

	dispatch_async(dispatch_get_main_queue(), ^{
	    NSEvent* e = [NSEvent otherEventWithType: NSEventTypeApplicationDefined
										location: NSMakePoint(0, 0)
								   modifierFlags: 0
									   timestamp: 0.0
									windowNumber: 0
										 context: nil
										 subtype: 0
										   data1: 0
										   data2: 0];
		[NSApp postEvent: e atStart: YES];
	});
}

bool _App_running(void) {
	return __running;
}

void _App_on_event(void (*on_event)(_AppEvent const*,void*), void* param) {
	_ASSERT(on_event != NULL);

	__on_event = on_event;
	__param = param;
}
