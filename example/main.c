#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libcore/path.h>
#include <libcore/app.h>
#include <libcore/window.h>
#include <libcore/context.h>
#include <libcore/MACRO.h>

_window_t* window = NULL;
_context_t* context = NULL;
_brush_t* red_brush = NULL;
_brush_t* green_brush = NULL;
_brush_t* blue_brush = NULL;
_brush_t* white_brush = NULL;

#define POINT_TO_RECT(a, b) ((_rect_t){ { (a).x - (b), (a.y) - (b) }, { ((b) * 2), ((b) * 2) } })

void window_render(_size_t const* size, float ratio) {
	_rect_t rect1 = {
		.origin = { 0, 0 },
		.size = _SIZE_MULT(
			*size,
			ratio
		)
	};

	_rect_t rect2 = {
		.origin = { 10, 10 },
		.size = { 10, 10 }
	};

	_context_begin_paint(context);
	_context_fill_rect(context, &rect2, white_brush);
	_context_stroke_rect(context, &rect2, 1, red_brush);
	_context_end_paint(context);
}

void window_event(_window_event_t const* event, void* param) {
	_ASSERT(event != NULL);
	if (event->type == _SIZE_WINDOW_EVENT) {
		window_render(
			_window_size(window),
			_window_pixelratio(window)
		);
	} else if (event->type == _CLOSE_WINDOW_EVENT) {
		_app_exit();
	}
}

void app_event(_app_event_t const* event, void* param) {
	_ASSERT(event != NULL);
	if (event->type == _RUN_APP_EVENT) {
		red_brush = _brush_create_color(&_RED_COLOR);
		green_brush = _brush_create_color(&_GREEN_COLOR);
		blue_brush = _brush_create_color(&_BLUE_COLOR);
		white_brush = _brush_create_color(&_WHITE_COLOR);
		window = _window_create();
		context = _context_create_window(window);
		_context_set_origin(context, _LEFTTOP_CONTEXT_ORIGIN);
		_window_on_event(window, window_event, NULL);
		_window_set_text(window, "Lorem ipsum привет рулет");
		_window_set_size(window, &(_size_t){ 640, 480 });
		_window_set_closable(window, false);
		_window_set_sizable(window, true);
		_window_set_minimizable(window, true);
		_window_set_maximizable(window, true);
		_window_set_visible(window, true);
	} else if (event->type == _EXIT_APP_EVENT) {
		_brush_destroy(red_brush);
		_brush_destroy(green_brush);
		_brush_destroy(blue_brush);
		_brush_destroy(white_brush);
		_context_destroy(context);
		_window_destroy(window);
	}
}

int main(int argc, char const *argv[]) {
	_app_on_event(app_event, NULL);
	_app_run();
	return 0;
}