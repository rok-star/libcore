#include <stdio.h>
#include <libcore/MACRO.h>
#include <libcore/runloop.h>

// #include <libcore/window.h>

// void event(_Event const* event, void* param) {
// 	if (event->type == _CLOSE_EVENT) {
// 		_Runloop_exit(_Runloop_current());
// 	}
// }

int i = 0;

void runloop_event(_RunloopEvent const* event, void* param) {
	_ASSERT(event != NULL);
	if (event->type == _RUN_RUNLOOP_EVENT) {
		puts("run event");
	} else if (event->type == _EXIT_RUNLOOP_EVENT) {
		puts("exit event");
	} else if (event->type == _SPIN_RUNLOOP_EVENT) {
		puts("spin event");
		i += 1;

		if (i >= 100)
			_Runloop_exit();
	}
}

int main(int argc, char const *argv[]) {
	// _Window* window = _Window_create();
	// _Window_on_event(window, event, NULL);
	// _Window_set_closable(window, true);
	// _Window_set_visible(window, true);

	_Runloop_run(runloop_event, NULL);

	return 0;
}