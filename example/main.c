#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libcore/app.h>
#include <libcore/window.h>
#include <libcore/context.h>
#include <libcore/MACRO.h>

_Color RED_COLOR = { 255, 0, 0, 255 };
_Color GREEN_COLOR = { 0, 255, 0, 255 };
_Color BLUE_COLOR = { 0, 0, 255, 255 };
_Color WHITE_COLOR = { 255, 255, 255, 255 };

_Window* window = NULL;
_Context* context = NULL;
_Brush* red_brush = NULL;
_Brush* green_brush = NULL;
_Brush* blue_brush = NULL;
_Brush* white_brush = NULL;

#define POINT_TO_RECT(a, b) ((_RectF){ { (a).x - (b), (a.y) - (b) }, { ((b) * 2), ((b) * 2) } })

void window_render(_Size const* size, float ratio) {
	_RectF rect1 = {
		.origin = { 0, 0 },
		.size = _SIZE_MULT(_SIZE_F(*size), ratio)
	};

	_RectF rect2 = {
		.origin = { 10, 10 },
		.size = { 10, 10 }
	};

	// rect2.origin.x += 0.5;
	// rect2.origin.y += 0.5;
	// rect2.size.width -= 1;
	// rect2.size.height -= 1;

	_Transform transform = {
		.scale = ratio
	};

	_Context_begin_paint(context);
	_Context_fill_rect(context, &rect1, white_brush, &transform);
	_Context_stroke_rect(context, &rect2, 2, red_brush, &transform);
	_Context_end_paint(context);
}

void window_event(_WindowEvent const* event, void* param) {
	_ASSERT(event != NULL);
	if (event->type == _SIZE_WINDOW_EVENT) {
		_Size size = _Window_size(window);
		float ratio = _Window_pixelratio(window);
		window_render(&size, ratio);
	} else if (event->type == _CLOSE_WINDOW_EVENT) {
		_App_exit();
	}
}

void app_event(_AppEvent const* event, void* param) {
	_ASSERT(event != NULL);
	if (event->type == _RUN_APP_EVENT) {
		red_brush = _Brush_create_color(&RED_COLOR);
		green_brush = _Brush_create_color(&GREEN_COLOR);
		blue_brush = _Brush_create_color(&BLUE_COLOR);
		white_brush = _Brush_create_color(&WHITE_COLOR);
		window = _Window_create();
		context = _Context_create_window(window);
		_Context_set_origin(context, _LEFTTOP_CONTEXT_ORIGIN);
		_Window_on_event(window, window_event, NULL);
		_Window_set_text(window, "Lorem ipsum привет рулет");
		_Window_set_size(window, &(_Size){ 640, 480 });
		_Window_set_closable(window, false);
		_Window_set_sizable(window, true);
		_Window_set_minimizable(window, true);
		_Window_set_maximizable(window, true);
		_Window_set_visible(window, true);
	} else if (event->type == _EXIT_APP_EVENT) {
		_Brush_destroy(red_brush);
		_Brush_destroy(green_brush);
		_Brush_destroy(blue_brush);
		_Brush_destroy(white_brush);
		_Context_destroy(context);
		_Window_destroy(window);
	}
}

int main(int argc, char const *argv[]) {
	_App_on_event(app_event, NULL);
	_App_run();
	return 0;
}