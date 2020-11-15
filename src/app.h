#ifndef _LIBCORE_APP_H
#define _LIBCORE_APP_H

#include <stdbool.h>

typedef enum {
	_RUN_APP_EVENT,
	_EXIT_APP_EVENT,
	_SPIN_APP_EVENT
} _APP_EVENT;

static char const* _APP_EVENT_NAME[3] = {
	"RUN", "EXIT", "SPIN"
};

typedef struct _AppEvent {
	_APP_EVENT type;
} _AppEvent;

#ifdef __cplusplus
extern "C" {
#endif

void _App_run();
void _App_exit(void);
void _App_wakeup(void);
bool _App_running(void);
void _App_on_event(void (*)(_AppEvent const*,void*), void*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_APP_H */
