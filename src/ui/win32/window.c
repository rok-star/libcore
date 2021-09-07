#include <stdio.h>
#include <stdbool.h>
#include <libcore/ui/win32/WINDOWS.h>
#include <libcore/ui/window.h>
#include <libcore/core/unicode.h>
#include <libcore/core/MACRO.h>

#define WINAPI_CALL(a) { \
    if (a == 0) \
        _ABORT("%s failed: %s", #a, GetLastErrorMessage()); \
}

#define CreateWindowW_E(a, b, c, d, e, f, g, h, i, j, k) ({ \
    HWND __ret = CreateWindowW(a, b, c, d, e, f, g, h, i, j, k); \
    if (__ret == 0) \
        _ABORT("%s", GetLastErrorMessage()); \
    __ret; \
})

typedef struct _window_t {
    HWND hwnd;
    void (*on_event)(_window_event_t const*,void*);
    void* param;
} _window_t;

void _window_trigger_event_basic(_window_t* window, _WINDOW_EVENT type) {
    assert(window != NULL);
    if (window->on_event != NULL) {
        window->on_event(
            &(_window_event_t){
                .type = type,
                .window = window
            },
            window->param
        );
    }
}

void _window_trigger_event_mouse(_window_t* window, _WINDOW_EVENT type, _Point point) {
    assert(window != NULL);
    if (window->on_event != NULL) {
        window->on_event(
            &(_window_event_t){
                .type = type,
                .window = window,
                .mouse_info = (_MouseInfo){
                    .position = point,
                    .wheel = 0
                }
            },
            window->param
        );
    }
}

void _window_trigger_event_key(_window_t* window, _WINDOW_EVENT type, int code, bool shift, bool control, bool option, bool super) {
    assert(window != NULL);
    if (window->on_event != NULL) {
        window->on_event(
            &(_window_event_t){
                .type = type,
                .window = window,
                .key_info = (_KeyInfo){
                    .key = _KEY_FROM_NATIVE[code],
                    .shift = shift,
                    .control = control,
                    .option = option,
                    .super = super
                }
            },
            window->param
        );
    }
}

LRESULT CALLBACK __window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    _window_t* window = (_window_t*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
    if (window != NULL) {
        if (msg == WM_ERASEBKGND) {
            ;
        } else if (msg == WM_PAINT) {
            ;
        } else if (msg == WM_SIZE) {
            _window_trigger_event_basic(window, _SIZE_WINDOW_EVENT);
        } else if (msg == WM_SHOWWINDOW) {
            _window_trigger_event_basic(window, (wparam == true) ? _SHOW_WINDOW_EVENT : _HIDE_WINDOW_EVENT);
        } else if (msg == WM_CLOSE) {
            _window_trigger_event_basic(window, _CLOSE_WINDOW_EVENT);
            return 0;
        }
    }
    return DefWindowProcW(hwnd, msg, wparam, lparam);
}

_window_t* _window_create(void) {
    _window_t* window = _NEW(_window_t, {});
    WNDCLASSEXW wc = {
        .cbSize = sizeof(WNDCLASSEXW),
        .style = (CS_VREDRAW | CS_HREDRAW),
        .lpfnWndProc = __window_proc,
        .hInstance = GetModuleHandle(NULL),
        .hIcon = LoadIcon(NULL, IDI_APPLICATION),
        .hCursor = LoadCursor(NULL, IDC_ARROW),
        .hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH)),
        .lpszClassName = L"_window_t",
        .hIconSm = LoadIcon(NULL, IDI_APPLICATION)
    };
    WINAPI_CALL(RegisterClassExW(&wc));
    window->hwnd = CreateWindowW_E(wc.lpszClassName, NULL, WS_CAPTION | WS_SYSMENU,
        0, 0, 640, 480, NULL, NULL, wc.hInstance, NULL);
    SetWindowLongPtrW(window->hwnd, GWLP_USERDATA, (LONG_PTR)window);
    EnableMenuItem(GetSystemMenu(window->hwnd, false), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
    UpdateWindow(window->hwnd);
    return window;
}

void _window_destroy(_window_t* window) {
    assert(window != NULL);
    window->on_event = NULL;
    DestroyWindow(window->hwnd);
    _FREE(window);
}

void _window_set_visible(_window_t* window, bool value) {
    assert(window != NULL);
    ShowWindow(window->hwnd, (value ? SW_SHOW : SW_HIDE));
}

void _window_set_sizable(_window_t* window, bool value) {
    assert(window != NULL);
    if (value) SetWindowLongPtr(window->hwnd, GWL_STYLE, GetWindowLongPtr(window->hwnd, GWL_STYLE) | WS_SIZEBOX);
    else SetWindowLongPtr(window->hwnd, GWL_STYLE, GetWindowLongPtr(window->hwnd, GWL_STYLE) & ~WS_SIZEBOX);
}

void _window_set_closable(_window_t* window, bool value) {
    assert(window != NULL);
    if (value) EnableMenuItem(GetSystemMenu(window->hwnd, false), SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);
    else EnableMenuItem(GetSystemMenu(window->hwnd, false), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
}

void _window_set_maximizable(_window_t* window, bool value) {
    assert(window != NULL);
    if (value) SetWindowLongPtr(window->hwnd, GWL_STYLE, GetWindowLongPtr(window->hwnd, GWL_STYLE) | WS_MAXIMIZEBOX);
    else SetWindowLongPtr(window->hwnd, GWL_STYLE, GetWindowLongPtr(window->hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);
}

void _window_set_minimizable(_window_t* window, bool value) {
    assert(window != NULL);
    if (value) SetWindowLongPtr(window->hwnd, GWL_STYLE, GetWindowLongPtr(window->hwnd, GWL_STYLE) | WS_MINIMIZEBOX);
    else SetWindowLongPtr(window->hwnd, GWL_STYLE, GetWindowLongPtr(window->hwnd, GWL_STYLE) & ~WS_MINIMIZEBOX);
}

void _window_set_maximized(_window_t* window, bool value) {
    assert(window != NULL);
}

void _window_set_minimized(_window_t* window, bool value) {
    assert(window != NULL);
}

void _window_set_topmost(_window_t* window, bool value) {
    assert(window != NULL);
}

void _window_set_size(_window_t* window, _size_t const* value) {
    assert(window != NULL);
    assert(value != NULL);
}

void _window_set_text(_window_t* window, char const* value) {
    assert(window != NULL);
    int len = strlen(value);
    wchar_t wchar[len + 1];
    _utf8_to_ucs2(value, len, wchar, (len + 1));
    SetWindowTextW(window->hwnd, wchar);
}

bool _window_visible(_window_t const* window) {
    assert(window != NULL);
    return false;
}

bool _window_closable(_window_t const* window) {
    assert(window != NULL);
    return false;
}

bool _window_sizable(_window_t const* window) {
    assert(window != NULL);
    return false;
}

bool _window_maximizable(_window_t const* window) {
    assert(window != NULL);
    return false;
}

bool _window_minimizable(_window_t const* window) {
    assert(window != NULL);
    return false;
}

bool _window_maximized(_window_t const* window) {
    assert(window != NULL);
    return false;
}

bool _window_minimized(_window_t const* window) {
    assert(window != NULL);
    return false;
}

bool _window_topmost(_window_t const* window) {
    assert(window != NULL);
    return false;
}

_size_t _window_size(_window_t const* window) {
    assert(window != NULL);
    RECT rc;
    GetWindowRect(window->hwnd, &rc);
    return (_size_t){
        .width = (rc.right - rc.left),
        .height = (rc.bottom - rc.top)
    };
}

char* _window_text(_window_t const* window) {
    assert(window != NULL);
    return NULL;
}

void _window_on_event(_window_t* window, void (*on_event)(_window_event_t const*,void*), void* param) {
    assert(window != NULL);
    window->on_event = on_event;
    window->param = param;
}

void* _window_HWND(_window_t const* window) {
    assert(window != NULL);
    return 0;
}
