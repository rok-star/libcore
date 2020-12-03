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

void stroke_bezier(_RectF const* rect) {
	_PointF p1 = { 0.0 + 10.0, 0.0 + 10.0 };
	_PointF p2 = { 0.0 + 10.0, rect->size.height - 10.0 };
	_PointF p3 = { rect->size.width - 10.0, rect->size.height - 10.0 };
	_PointF p4 = { rect->size.width - 10.0, 0.0 + 10.0 };

	_Context_fill_rect(context, &POINT_TO_RECT(p1, 4), green_brush);
	_Context_fill_rect(context, &POINT_TO_RECT(p2, 4), green_brush);
	_Context_fill_rect(context, &POINT_TO_RECT(p3, 4), green_brush);
	_Context_fill_rect(context, &POINT_TO_RECT(p4, 4), green_brush);

	int num = rect->size.width / 10;

	for (int i = 0; i < num; i++) {
		double t = (i / (num - 1.0));
		_PointF pt = _bezier_point(&p1, &p2, &p3, &p4, t);
		_RectF rc = POINT_TO_RECT(pt, 2);
		_Context_fill_rect(context, &rc, red_brush);
	}

	_PointF* res_data = NULL;
	int res_num = 0;
	_bezier_points(&p1, &p2, &p3, &p4, 0.5, &res_data, &res_num);

	printf("res_num: %d\n", res_num);

	for (int i = 0; i < res_num; i++) {
		_RectF rc = POINT_TO_RECT(res_data[i], 2);
		printf("x: %f, y: %f\n", res_data[i].x, res_data[i].y);
		_Context_fill_rect(context, &rc, blue_brush);
	}
}

void window_render(void) {
	_RectF rect = {
		.origin = { 0, 0 },
		.size = _SIZE_F(_Window_size(window))
	};
	_Context_begin_paint(context);
	_Context_fill_rect(context, &rect, white_brush);
	_Context_stroke_rect(context, &rect, 2, red_brush);

	stroke_bezier(&rect);

	_Context_end_paint(context);
}

void window_event(_WindowEvent const* event, void* param) {
	_ASSERT(event != NULL);
	if (event->type == _SIZE_WINDOW_EVENT) {
		window_render();
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
		context = _Context_create(_WINDOW_CONTEXT_TYPE, window);
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