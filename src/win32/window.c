#include <stdio.h>
#include <stdbool.h>
#include <libcore/WINDOWS.h>
#include <libcore/window.h>
#include <libcore/unicode.h>
#include <libcore/MACRO.h>

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

typedef struct _Window {
	HWND hwnd;
	void (*on_event)(_WindowEvent const*,void*);
	void* param;
} _Window;

void _Window_trigger_event_basic(_Window* window, _WINDOW_EVENT type) {
	_ASSERT(window != NULL);
	if (window->on_event != NULL) {
		window->on_event(
			&(_WindowEvent){
				.type = type,
				.window = window
			},
			window->param
		);
	}
}

void _Window_trigger_event_mouse(_Window* window, _WINDOW_EVENT type, _Point point) {
	_ASSERT(window != NULL);
	if (window->on_event != NULL) {
		window->on_event(
			&(_WindowEvent){
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

void _Window_trigger_event_key(_Window* window, _WINDOW_EVENT type, int code, bool shift, bool control, bool option, bool super) {
	_ASSERT(window != NULL);
	if (window->on_event != NULL) {
		window->on_event(
			&(_WindowEvent){
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

LRESULT CALLBACK __Window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	_Window* window = (_Window*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
	if (window != NULL) {
		if (msg == WM_ERASEBKGND) {
			;
		} else if (msg == WM_PAINT) {
			;
		} else if (msg == WM_SIZE) {
			_Window_trigger_event_basic(window, _SIZE_WINDOW_EVENT);
		} else if (msg == WM_SHOWWINDOW) {
			_Window_trigger_event_basic(window, (wparam == true) ? _SHOW_WINDOW_EVENT : _HIDE_WINDOW_EVENT);
		} else if (msg == WM_CLOSE) {
			_Window_trigger_event_basic(window, _CLOSE_WINDOW_EVENT);
			return 0;
		}
	}
	return DefWindowProcW(hwnd, msg, wparam, lparam);
}

_Window* _Window_create(void) {
	_Window* window = _NEW(_Window, {});
	WNDCLASSEXW wc = {
		.cbSize = sizeof(WNDCLASSEXW),
		.style = (CS_VREDRAW | CS_HREDRAW),
		.lpfnWndProc = __Window_proc,
		.hInstance = GetModuleHandle(NULL),
		.hIcon = LoadIcon(NULL, IDI_APPLICATION),
		.hCursor = LoadCursor(NULL, IDC_ARROW),
		.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH)),
		.lpszClassName = L"_Window",
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

void _Window_destroy(_Window* window) {
	_ASSERT(window != NULL);
	window->on_event = NULL;
	DestroyWindow(window->hwnd);
	_FREE(window);
}

void _Window_set_visible(_Window* window, bool value) {
	_ASSERT(window != NULL);
	ShowWindow(window->hwnd, (value ? SW_SHOW : SW_HIDE));
}

void _Window_set_sizable(_Window* window, bool value) {
	_ASSERT(window != NULL);
	if (value) SetWindowLongPtr(window->hwnd, GWL_STYLE, GetWindowLongPtr(window->hwnd, GWL_STYLE) | WS_SIZEBOX);
	else SetWindowLongPtr(window->hwnd, GWL_STYLE, GetWindowLongPtr(window->hwnd, GWL_STYLE) & ~WS_SIZEBOX);
}

void _Window_set_closable(_Window* window, bool value) {
	_ASSERT(window != NULL);
	if (value) EnableMenuItem(GetSystemMenu(window->hwnd, false), SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);
	else EnableMenuItem(GetSystemMenu(window->hwnd, false), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
}

void _Window_set_maximizable(_Window* window, bool value) {
	_ASSERT(window != NULL);
	if (value) SetWindowLongPtr(window->hwnd, GWL_STYLE, GetWindowLongPtr(window->hwnd, GWL_STYLE) | WS_MAXIMIZEBOX);
	else SetWindowLongPtr(window->hwnd, GWL_STYLE, GetWindowLongPtr(window->hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);
}

void _Window_set_minimizable(_Window* window, bool value) {
	_ASSERT(window != NULL);
	if (value) SetWindowLongPtr(window->hwnd, GWL_STYLE, GetWindowLongPtr(window->hwnd, GWL_STYLE) | WS_MINIMIZEBOX);
	else SetWindowLongPtr(window->hwnd, GWL_STYLE, GetWindowLongPtr(window->hwnd, GWL_STYLE) & ~WS_MINIMIZEBOX);
}

void _Window_set_maximized(_Window* window, bool value) {
	_ASSERT(window != NULL);
}

void _Window_set_minimized(_Window* window, bool value) {
	_ASSERT(window != NULL);
}

void _Window_set_topmost(_Window* window, bool value) {
	_ASSERT(window != NULL);
}

void _Window_set_size(_Window* window, _Size const* value) {
	_ASSERT(window != NULL);
	_ASSERT(value != NULL);
}

void _Window_set_text(_Window* window, char const* value) {
	_ASSERT(window != NULL);
	int len = strlen(value);
	wchar_t wchar[len + 1];
	_utf8_to_ucs2(value, len, wchar, (len + 1));
	SetWindowTextW(window->hwnd, wchar);
}

bool _Window_visible(_Window* window) {
	_ASSERT(window != NULL);
	return false;
}

bool _Window_closable(_Window* window) {
	_ASSERT(window != NULL);
	return false;
}

bool _Window_sizable(_Window* window) {
	_ASSERT(window != NULL);
	return false;
}

bool _Window_maximizable(_Window* window) {
	_ASSERT(window != NULL);
	return false;
}

bool _Window_minimizable(_Window* window) {
	_ASSERT(window != NULL);
	return false;
}

bool _Window_maximized(_Window* window) {
	_ASSERT(window != NULL);
	return false;
}

bool _Window_minimized(_Window* window) {
	_ASSERT(window != NULL);
	return false;
}

bool _Window_topmost(_Window* window) {
	_ASSERT(window != NULL);
	return false;
}

_Size _Window_size(_Window* window) {
	_ASSERT(window != NULL);
	RECT rc;
	GetWindowRect(window->hwnd, &rc);
	return (_Size){
		.width = (rc.right - rc.left),
		.height = (rc.bottom - rc.top)
	};
}

char* _Window_text(_Window* window) {
	_ASSERT(window != NULL);
	return NULL;
}

void _Window_on_event(_Window* window, void (*on_event)(_WindowEvent const*,void*), void* param) {
	_ASSERT(window != NULL);
	window->on_event = on_event;
	window->param = param;
}

void* _Window_HWND(_Window* window) {
	_ASSERT(window != NULL);
	return 0;
}
