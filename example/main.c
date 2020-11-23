#include <stdio.h>
#include <libcore/app.h>
#include <libcore/window.h>
#include <libcore/context.h>
#include <libcore/MACRO.h>

_Window* window = NULL;
_Context* context = NULL;

void window_render(void) {
	_RectF rect = {
		.origin = { 0, 0 },
		.size = _TO_SIZE_F(_Window_size(window))
	};
	_Context_begin_paint(context);
	_Context_fill_rect(context, &rect, &(_Color){ 255, 255, 255, 255 });
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
		window = _Window_create();
		context = _Context_create(_WINDOW_CONTEXT, window);
		_Window_on_event(window, window_event, NULL);
		_Window_set_text(window, "Лорем ипсум");
		_Window_set_size(window, &(_Size){ 640, 480 });
		_Window_set_closable(window, false);
		_Window_set_sizable(window, true);
		_Window_set_minimizable(window, true);
		_Window_set_maximizable(window, true);
		_Window_set_visible(window, true);
	} else if (event->type == _EXIT_APP_EVENT) {
		_Context_destroy(context);
		_Window_destroy(window);
	}
}

int main(int argc, char const *argv[]) {
	_App_on_event(app_event, NULL);
	_App_run();
	return 0;
}