#ifndef _LIBCORE_WINDOW_H
#define _LIBCORE_WINDOW_H

#include <libcore/type.h>
#include <libcore/keyboard.h>

typedef struct _Window _Window;

typedef enum {
    _SHOW_EVENT,
    _HIDE_EVENT,
    _SIZE_EVENT,
    _CLOSE_EVENT,
    _MAXIMIZE_EVENT,
    _MINIMIZE_EVENT,
    _MOUSEMOVE_EVENT,
    _LBUTTONDOWN_EVENT,
    _LBUTTONUP_EVENT,
    _RBUTTONDOWN_EVENT,
    _RBUTTONUP_EVENT,
    _KEYDOWN_EVENT,
    _KEYUP_EVENT,
} _EVENT;

typedef struct _Event {
    _EVENT type;
    _Window* window;
    struct {
        _KEY key;
        bool alt;
        bool shift;
        bool control;
        bool super;
    } key_info;
    struct {
        _Point position;
        int wheel;
    } mouse_info;
} _Event;

_Window* _Window_create(void);
void _Window_destroy(_Window*);
void _Window_set_visible(_Window*, bool);
void _Window_set_closable(_Window*, bool);
void _Window_set_sizable(_Window*, bool);
void _Window_set_maximizable(_Window*, bool);
void _Window_set_minimizable(_Window*, bool);
void _Window_set_maximized(_Window*, bool);
void _Window_set_minimized(_Window*, bool);
void _Window_set_topmost(_Window*, bool);
void _Window_set_size(_Window*, _Size*);
void _Window_set_text(_Window*, char const*);
bool _Window_visible(_Window*);
bool _Window_closable(_Window*);
bool _Window_sizable(_Window*);
bool _Window_maximizable(_Window*);
bool _Window_minimizable(_Window*);
bool _Window_maximized(_Window*);
bool _Window_minimized(_Window*);
bool _Window_topmost(_Window*);
_Size _Window_size(_Window*);
char* _Window_text(_Window*);
void _Window_on_event(_Window*, void (*)(_Event const*,void*), void*);

#if _WIN32
    void* _Window_HWND(_Window*);
#elif __APPLE__
    void* _Window_NSWindow(_Window*);
#endif

#endif /* _LIBCORE_WINDOW_H */
