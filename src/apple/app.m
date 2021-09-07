#include <assert.h>
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

- (void)exit {
    _onExit();
}

@end

typedef struct _app_t {
    void (*proc)(_app_event_t const*, void*);
    void* param;
} _app_t;

_app_t* __current = NULL;

_app_t* _app_create(void) {
    assert_M(__current == NULL, "only one instance of \"_app_t\" can be created");
    _app_t* app = _NEW(_app_t, {});
    __current = app;
    [[NSApplication sharedApplication]
        setDelegate: [[__Delegate alloc]
            initWithOnRun: ^{
                id mainMenu = [[NSMenu alloc] init];
                id appMenu = [[NSMenu alloc] init];
                id appItem = [mainMenu addItemWithTitle: [NSString stringWithCString: "123" encoding: NSUTF8StringEncoding] action: nil keyEquivalent: @""];
                id quitItem = [appMenu addItemWithTitle: @"Quit" action: nil keyEquivalent: @"q"];
                [quitItem setTarget: [NSApp delegate]];
                [quitItem setAction: @selector(exit)];
                [appItem setSubmenu: appMenu];
                [NSApp setMainMenu: mainMenu];
                [NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];
            }
            onExit: ^{
                if (app->proc != NULL) {
                    app->proc(
                        &(_app_event_t){
                            .type = _EXIT_APP_EVENT
                        },
                        app->param
                    );
                }
            }
        ]
    ];
    [NSApp finishLaunching];
    return app;
}

void _app_destroy(_app_t* app) {
    assert(app != NULL);
    assert(app == __current);
    [[NSApplication sharedApplication] setDelegate: nil];
    _FREE(app);
    __current = NULL;
}

void _app_process(_app_t* app) {
    assert(app != NULL);
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
}

void _app_process_timeout(_app_t* app, double timeout) {
    assert(app != NULL);
    NSEvent* event = [NSApp nextEventMatchingMask: NSEventMaskAny
                                        untilDate: [NSDate dateWithTimeIntervalSinceNow: (timeout / 1000.0)]
                                           inMode: NSDefaultRunLoopMode
                                          dequeue: YES];
    if (event != nil) {
        [NSApp sendEvent: event];
        [NSApp updateWindows];
        _app_process(app);
    }
}

void _app_on_event(_app_t* app, void(*proc)(_app_event_t const*,void*), void* param) {
    assert(app != NULL);
    app->proc = proc;
    app->param = param;
}