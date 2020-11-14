#include <stdio.h>
#include <libcore/app.h>
#include <libcore/MACRO.h>

// #include <libcore/window.h>

// void event(_Event const* event, void* param) {
// 	if (event->type == _CLOSE_EVENT) {
// 		_App_exit(_App_current());
// 	}
// }

int i = 0;

void event(_AppEvent const* event, void* param) {
	_ASSERT(event != NULL);
	if (event->type == _RUN_APP_EVENT) {
		puts("run event");
	} else if (event->type == _EXIT_APP_EVENT) {
		puts("exit event");
	} else if (event->type == _SPIN_APP_EVENT) {
		puts("spin event");
		i += 1;

		if (i >= 100)
			_App_exit();
	}
}

int main(int argc, char const *argv[]) {
	// _Window* window = _Window_create();
	// _Window_on_event(window, event, NULL);
	// _Window_set_closable(window, true);
	// _Window_set_visible(window, true);

	_App_on_event(event, NULL);
	_App_run();

	return 0;
}