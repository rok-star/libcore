#include <Cocoa/Cocoa.h>
#include <libcore/MACRO.h>
#include <libcore/sleep.h>
#include <libcore/time.h>
#include <libcore/app.h>

static void (*__event_proc)(_app_event_t const*,void*) = NULL;
static void* __event_param = NULL;
static bool __running = false;
static bool __exiting = false;

@interface __Delegate : NSObject<NSApplicationDelegate>

- (id)initWithOnRun:(void(^)(void))onRun onExit:(void(^)(void))onExit;

@end

@implementation __Delegate {
    void (^_onRun)(void);
    void (^_onExit)(void);
}

- (id)initWithOnRun:(void(^)(void))onRun onExit:(void(^)(void))onExit {
    self = [super init];
    if (self) {
        _onRun = onRun;
        _onExit = onExit;
    }
    return self;
}

- (void)applicationDidFinishLaunching:(NSNotification*)notification {
    _onRun();
}

- (void)applicationWillTerminate:(NSNotification*)notification {
    _onExit();
}

- (void)stop {
    [NSApp stop: self];
    __exiting = true;
}

@end

void _app_run(void) {
    _ASSERT(__running == false);

    __running = true;
    __exiting = false;

    [[NSApplication sharedApplication]
        setDelegate: [[__Delegate alloc]
            initWithOnRun: ^{
                id mainMenu = [[NSMenu alloc] init];
                id appMenu = [[NSMenu alloc] init];
                id appItem = [mainMenu addItemWithTitle: [NSString stringWithCString: "123" encoding: NSUTF8StringEncoding] action: nil keyEquivalent: @""];
                id quitItem = [appMenu addItemWithTitle: @"Quit" action: nil keyEquivalent: @"q"];
                [quitItem setTarget: [NSApp delegate]];
                [quitItem setAction: @selector(stop)];
                [appItem setSubmenu: appMenu];
                [NSApp setMainMenu: mainMenu];
                [NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];
            }
            onExit: ^{}
        ]
    ];

    [NSApp finishLaunching];

    if (__event_proc != NULL)
        __event_proc(&(_app_event_t){ .type = _RUN_APP_EVENT }, __event_param);

    CFRunLoopAddObserver(
        CFRunLoopGetCurrent(),
        CFRunLoopObserverCreateWithHandler(
            kCFAllocatorDefault,
            kCFRunLoopBeforeWaiting,
            YES,
            0,
            ^(CFRunLoopObserverRef a, CFRunLoopActivity b) {
                if (__event_proc != NULL)
                    __event_proc(&(_app_event_t){ .type = _SPIN_APP_EVENT }, __event_param);
            }
        ),
        kCFRunLoopDefaultMode
    );

    [NSApp run];

    if (__event_proc != NULL)
        __event_proc(&(_app_event_t){ .type = _EXIT_APP_EVENT }, __event_param);

    __running = false;
}

void _app_run_FPS(int FPS) {
    _ASSERT(__running == false);

    __running = true;
    __exiting = false;

    [[NSApplication sharedApplication]
        setDelegate: [[__Delegate alloc]
            initWithOnRun: ^{
                id mainMenu = [[NSMenu alloc] init];
                id appMenu = [[NSMenu alloc] init];
                id appItem = [mainMenu addItemWithTitle: [NSString stringWithCString: "123" encoding: NSUTF8StringEncoding] action: nil keyEquivalent: @""];
                id quitItem = [appMenu addItemWithTitle: @"Quit" action: nil keyEquivalent: @"q"];
                [quitItem setTarget: [NSApp delegate]];
                [quitItem setAction: @selector(stop)];
                [appItem setSubmenu: appMenu];
                [NSApp setMainMenu: mainMenu];
                [NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];
            }
            onExit: ^{}
        ]
    ];

    [NSApp finishLaunching];

    if (__event_proc != NULL)
        __event_proc(&(_app_event_t){ .type = _RUN_APP_EVENT }, __event_param);

    _ASSERT(FPS > 0);

    for (;;) {
        double time = _time();

        for (;;) {
            NSEvent* event = [NSApp nextEventMatchingMask: NSEventMaskAny
                                                untilDate: nil
                                                   inMode: NSDefaultRunLoopMode
                                                  dequeue: YES];
            if (event == nil)
                break;

            [NSApp sendEvent: event];
            [NSApp updateWindows];

            if (__exiting)
                break;
        }

        if (__exiting)
            break;

        if (__event_proc != NULL)
            __event_proc(&(_app_event_t){ .type = _SPIN_APP_EVENT }, __event_param);

        if (__exiting)
            break;

        _sleep((1000.0L / FPS) - (_time() - time));
    }

    if (__event_proc != NULL)
        __event_proc(&(_app_event_t){ .type = _EXIT_APP_EVENT }, __event_param);

    __running = false;
}

void _app_run_sleep(double sleep) {
    _ASSERT(__running == false);

    __running = true;
    __exiting = false;

    [[NSApplication sharedApplication]
        setDelegate: [[__Delegate alloc]
            initWithOnRun: ^{
                id mainMenu = [[NSMenu alloc] init];
                id appMenu = [[NSMenu alloc] init];
                id appItem = [mainMenu addItemWithTitle: [NSString stringWithCString: "123" encoding: NSUTF8StringEncoding] action: nil keyEquivalent: @""];
                id quitItem = [appMenu addItemWithTitle: @"Quit" action: nil keyEquivalent: @"q"];
                [quitItem setTarget: [NSApp delegate]];
                [quitItem setAction: @selector(stop)];
                [appItem setSubmenu: appMenu];
                [NSApp setMainMenu: mainMenu];
                [NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];
            }
            onExit: ^{}
        ]
    ];

    [NSApp finishLaunching];

    if (__event_proc != NULL)
        __event_proc(&(_app_event_t){ .type = _RUN_APP_EVENT }, __event_param);

    _ASSERT(sleep >= 0);

    for (;;) {
        for (;;) {
            NSEvent* event = [NSApp nextEventMatchingMask: NSEventMaskAny
                                                untilDate: nil
                                                   inMode: NSDefaultRunLoopMode
                                                  dequeue: YES];
            if (event == nil)
                break;

            [NSApp sendEvent: event];
            [NSApp updateWindows];

            if (__exiting)
                break;
        }
        if (__exiting)
            break;

        if (__event_proc != NULL)
            __event_proc(&(_app_event_t){ .type = _SPIN_APP_EVENT }, __event_param);

        if (__exiting)
            break;

        _sleep(sleep);
    }

    if (__event_proc != NULL)
        __event_proc(&(_app_event_t){ .type = _EXIT_APP_EVENT }, __event_param);

    __running = false;
}

#define _POST_EVENT { \
    NSEvent* e = [NSEvent otherEventWithType: NSEventTypeApplicationDefined \
                                    location: NSMakePoint(0, 0) \
                               modifierFlags: 0 \
                                   timestamp: 0.0 \
                                windowNumber: 0 \
                                     context: nil \
                                     subtype: 0 \
                                       data1: 0 \
                                       data2: 0]; \
    [NSApp postEvent: e atStart: YES]; \
}

void _app_exit(void) {
    _ASSERT(__running == true);

    __exiting = true;

    if ([NSThread isMainThread]) {
        [NSApp stop: nil];
        [NSApp discardEventsMatchingMask: NSEventMaskAny
                             beforeEvent: nil];
        _POST_EVENT
    } else {
        dispatch_async(dispatch_get_main_queue(), ^{
            [NSApp stop: nil];
            [NSApp discardEventsMatchingMask: NSEventMaskAny
                                 beforeEvent: nil];
            _POST_EVENT
        });
    }
}

void _app_wakeup(void) {
    _ASSERT(__running == true);

    if ([NSThread isMainThread]) {
        _POST_EVENT
    } else {
        dispatch_async(dispatch_get_main_queue(), ^{
            _POST_EVENT
        });
    }
}

bool _app_running(void) {
    return __running;
}

void _app_on_event(void (*proc)(_app_event_t const*,void*), void* param) {
    _ASSERT(proc != NULL);
    __event_proc = proc;
    __event_param = param;
}
