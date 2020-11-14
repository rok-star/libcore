#include <Cocoa/Cocoa.h>
#include <libcore/MACRO.h>
#include <libcore/runloop.h>

bool __running = false;

void _Runloop_run(void (*on_event)(_RunloopEvent const*,void*), void* param) {
	_ASSERT(__running == false);

	[NSApplication sharedApplication];
	[NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];

	if (on_event != NULL)
		on_event(&(_RunloopEvent){ .type = _RUN_RUNLOOP_EVENT }, param);

	CFRunLoopAddObserver(
		CFRunLoopGetCurrent(),
		CFRunLoopObserverCreateWithHandler(
			kCFAllocatorDefault,
			kCFRunLoopBeforeWaiting,
			YES,
			0,
			^(CFRunLoopObserverRef a, CFRunLoopActivity b) {
				if (on_event != NULL)
					on_event(&(_RunloopEvent){ .type = _SPIN_RUNLOOP_EVENT }, param);
			}
		),
		kCFRunLoopDefaultMode
	);

	__running = true;

	[NSApp run];

	__running = false;

	if (on_event != NULL)
		on_event(&(_RunloopEvent){ .type = _EXIT_RUNLOOP_EVENT }, param);
}

void _Runloop_exit(void) {
	_ASSERT(__running == true);

	dispatch_async(dispatch_get_main_queue(), ^{
		[NSApp stop: NULL];
	});
}

void _Runloop_wakeup(void) {
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

bool _Runloop_running(void) {
	return __running;
}
