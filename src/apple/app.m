#include <Cocoa/Cocoa.h>
#include <libcore/MACRO.h>
#include <libcore/sleep.h>
#include <libcore/time.h>
#include <libcore/app.h>

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

typedef struct _app_t {
    _dispatch_queue_t* queue;
    void (*proc)(_app_event_t const*, void*);
    void* param;
} _app_t;

typedef struct __dispatch_event_t {
    _app_t* app;
    _app_event_t event;
} __dispatch_event_t;

static void __dispatch_event(__dispatch_event_t* event) {
    _ASSERT(event != NULL);
    _ASSERT(event->app != NULL);
    if (event->app->proc != NULL) {
        event->app->proc(
            &event->event,
            event->app->param
        );
    }
    _FREE(event);
}

_app_t* _app_create(void) {
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

    return _NEW(_app_t, {});
}

void _app_destroy(_app_t* app) {
    _ASSERT(app != NULL);

    [[NSApplication sharedApplication] setDelegate: nil];

    _FREE(app);
}

void _app_process(_app_t* app) {
    _ASSERT(app != NULL);

    for (;;) {
        NSEvent* event = [NSApp nextEventMatchingMask: NSEventMaskAny
                                            untilDate: nil
                                               inMode: NSDefaultRunLoopMode
                                              dequeue: YES];
        if (event == nil)
            break;

        [NSApp sendEvent: event];
        [NSApp updateWindows];
    }

    if (app->proc != NULL) {
        if (app->queue != NULL) {
            _dispatch_queue_post(
                app->queue,
                __dispatch_event,
                _NEW(__dispatch_event_t, {
                    .app = app,
                    .event = {
                        .type = _SPIN_APP_EVENT
                    }
                });
            );
        } else {
            app->proc(
                &(_app_event_t){
                    .type = _SPIN_APP_EVENT
                },
                app->param
            );
        }
    }
}

void _app_on_event(_app_t* app, _dispatch_queue_t* queue, void(*proc)(_app_event_t const*,void*), void* param) {
    _ASSERT(app != NULL);
    app->queue = queue;
    app->proc = proc;
    app->param = param;
}