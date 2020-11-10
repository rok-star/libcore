```c
#include <libcore/app.h>
#include <libcore/window.h>

void event(_Event const* event, void* param) {
	if (event->type == _CLOSE_EVENT) {
		_App_exit(_App_current());
	}
}

int main(int argc, char const *argv[]) {
	_Window* window = _Window_create();
	_Window_on_event(window, event, NULL);
	_Window_set_closable(window, true);
	_Window_set_visible(window, true);

	_App* app = _App_create();
	_App_run(app);
	_App_destroy(app);

	return 0;
}
```