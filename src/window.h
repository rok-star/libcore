#ifndef _LIBCORE_WINDOW_H
#define _LIBCORE_WINDOW_H

#include <stdbool.h>
#include <libcore/size.h>
#include <libcore/point.h>
#include <libcore/keyboard.h>

typedef struct _Window _Window;

typedef enum : int {
    _SHOW_WINDOW_EVENT        = 0,
    _HIDE_WINDOW_EVENT        = 1,
    _SIZE_WINDOW_EVENT        = 2,
    _CLOSE_WINDOW_EVENT       = 3,
    _MAXIMIZE_WINDOW_EVENT    = 4,
    _MINIMIZE_WINDOW_EVENT    = 5,
    _DEMAXIMIZE_WINDOW_EVENT  = 6,
    _DEMINIMIZE_WINDOW_EVENT  = 7,
    _MOUSEMOVE_WINDOW_EVENT   = 8,
    _LBUTTONDOWN_WINDOW_EVENT = 9,
    _LBUTTONUP_WINDOW_EVENT   = 10,
    _RBUTTONDOWN_WINDOW_EVENT = 11,
    _RBUTTONUP_WINDOW_EVENT   = 12,
    _KEYDOWN_WINDOW_EVENT     = 13,
    _KEYUP_WINDOW_EVENT       = 14
} _WINDOW_EVENT;

static char const* _WINDOW_EVENT_NAME[15] = {
    "SHOW", "HIDE", "SIZE", "CLOSE", "MAXIMIZE",
    "MINIMIZE", "DEMAXIMIZE", "DEMINIMIZE",
    "MOUSEMOVE", "LBUTTONDOWN", "LBUTTONUP",
    "RBUTTONDOWN", "RBUTTONUP", "KEYDOWN", "KEYUP"
};

typedef struct _KeyInfo {
    _KEY key;
    bool shift;
    bool control;
    bool option;
    bool super;
} _KeyInfo;

typedef struct _MouseInfo {
    _Point position;
    int wheel;
} _MouseInfo;

typedef struct _WindowEvent {
    _WINDOW_EVENT type;
    _Window* window;
    _KeyInfo key_info;
    _MouseInfo mouse_info;
} _WindowEvent;

#ifdef __cplusplus
extern "C" {
#endif

_Window* _Window_create(void);
void _Window_destroy(_Window*);
void _Window_set_visible(_Window*, bool);
void _Window_set_sizable(_Window*, bool);
void _Window_set_closable(_Window*, bool);
void _Window_set_maximizable(_Window*, bool);
void _Window_set_minimizable(_Window*, bool);
void _Window_set_maximized(_Window*, bool);
void _Window_set_minimized(_Window*, bool);
void _Window_set_topmost(_Window*, bool);
void _Window_set_size(_Window*, _Size const*);
void _Window_set_text(_Window*, char const*);
bool _Window_visible(_Window const*);
bool _Window_closable(_Window const*);
bool _Window_sizable(_Window const*);
bool _Window_maximizable(_Window const*);
bool _Window_minimizable(_Window const*);
bool _Window_maximized(_Window const*);
bool _Window_minimized(_Window const*);
bool _Window_topmost(_Window const*);
_Size _Window_size(_Window const*);
char* _Window_text(_Window const*);
void _Window_on_event(_Window*, void (*)(_WindowEvent const*,void*), void*);

#if _WIN32
    void* _Window_HWND(_Window const*);
#elif __APPLE__
    void* _Window_NSWindow(_Window const*);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_WINDOW_H */
