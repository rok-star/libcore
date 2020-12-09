#ifndef _LIBCORE_APP_H
#define _LIBCORE_APP_H

#include <stdbool.h>

typedef enum {
	_RUN_APP_EVENT  = 0,
	_EXIT_APP_EVENT = 1,
	_SPIN_APP_EVENT = 2
} _APP_EVENT;

typedef enum {
	_DEFAULT_APP_MODE  = 0,
	_SLEEP_APP_MODE    = 1,
	_FPS_APP_MODE 	   = 2
} _APP_MODE;

static char const* _APP_EVENT_NAME[3] = {
	"RUN", "EXIT", "SPIN"
};

typedef struct _app_event_t {
	_APP_EVENT type;
} _app_event_t;

#ifdef __cplusplus
extern "C" {
#endif

void _app_run(_APP_MODE,double);
void _app_exit(void);
void _app_wakeup(void);
bool _app_running(void);
void _app_on_event(void(*)(_app_event_t const*,void*),void*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_APP_H */
