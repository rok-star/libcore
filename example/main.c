#include <stdio.h>
#include <libcore/app.h>
#include <libcore/window.h>
#include <libcore/MACRO.h>

void window_event(_WindowEvent const* event, void* param) {
	_ASSERT(event != NULL);
	//printf("%s\n", _WINDOW_EVENT_NAME[event->type]);

	if (event->type == _CLOSE_WINDOW_EVENT) {
		_App_exit();
	}
}

void app_event(_AppEvent const* event, void* param) {
	_ASSERT(event != NULL);
	//printf("%s\n", _APP_EVENT_NAME[event->type]);
}

int main(int argc, char const *argv[]) {
	_Window* window = _Window_create();
	_Window_on_event(window, window_event, NULL);
	_Window_set_closable(window, true);
	_Window_set_size(window, &(_Size){ 640, 480 });
	_Window_set_visible(window, true);
	
	_Size size = _Window_size(window);
	printf("window size is: width = %d, height = %d\n", size.width, size.height);

	_App_on_event(app_event, NULL);
	_App_run();

	_Window_destroy(window);

	return 0;
}