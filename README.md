```c
#include <libcore/runloop.h>
#include <libcore/window.h>

void event(_WindowEvent const* event, void* param) {
    if (event->type == _CLOSE_WINDOW_EVENT) {
        _Runloop_exit();
    }
}

int main(int argc, char const *argv[]) {
    _Window* window = _Window_create();
    _Window_on_event(window, event, NULL);
    _Window_set_closable(window, true);
    _Window_set_visible(window, true);

    _Runloop_run(NULL, NULL);

    return 0;
}
```