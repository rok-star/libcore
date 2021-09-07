#ifndef _LIBCORE_WINDOW_H
#define _LIBCORE_WINDOW_H

#include <stdbool.h>
#include <libcore/core/math.h>
#include <libcore/ui/keyboard.h>

typedef struct _window_t _window_t;

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
    _KEYUP_WINDOW_EVENT       = 14,
    _PIXELRATIO_WINDOW_EVENT  = 15
} _WINDOW_EVENT;

static char const* _WINDOW_EVENT_NAME[16] = {
    "SHOW", "HIDE", "SIZE", "CLOSE", "MAXIMIZE",
    "MINIMIZE", "DEMAXIMIZE", "DEMINIMIZE",
    "MOUSEMOVE", "LBUTTONDOWN", "LBUTTONUP",
    "RBUTTONDOWN", "RBUTTONUP", "KEYDOWN", "KEYUP",
    "PIXELRATIO"
};

typedef struct _key_info_t {
    _KEY key;
    bool shift;
    bool control;
    bool option;
    bool super;
} _key_info_t;

typedef struct _mouse_info_t {
    _point_t position;
    int wheel;
} _mouse_info_t;

typedef struct _window_event_t {
    _WINDOW_EVENT type;
    _window_t* window;
    _key_info_t key_info;
    _mouse_info_t mouse_info;
} _window_event_t;

#ifdef __cplusplus
extern "C" {
#endif

_window_t* _window_create(void);
void _window_destroy(_window_t*);
void _window_set_visible(_window_t*, bool);
void _window_set_sizable(_window_t*, bool);
void _window_set_closable(_window_t*, bool);
void _window_set_maximizable(_window_t*, bool);
void _window_set_minimizable(_window_t*, bool);
void _window_set_maximized(_window_t*, bool);
void _window_set_minimized(_window_t*, bool);
void _window_set_topmost(_window_t*, bool);
void _window_set_origin(_window_t*, _point_t const*);
void _window_set_size(_window_t*, _size_t const*);
void _window_set_text(_window_t*, char const*);
bool _window_visible(_window_t const*);
bool _window_closable(_window_t const*);
bool _window_sizable(_window_t const*);
bool _window_maximizable(_window_t const*);
bool _window_minimizable(_window_t const*);
bool _window_maximized(_window_t const*);
bool _window_minimized(_window_t const*);
bool _window_topmost(_window_t const*);
_point_t const* _window_origin(_window_t const*);
_size_t const* _window_size(_window_t const*);
char const* _window_text(_window_t const*);
float _window_pixelratio(_window_t const*);
void _window_on_event(_window_t*, void (*)(_window_event_t const*,void*), void*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_WINDOW_H */
