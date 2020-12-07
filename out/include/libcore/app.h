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

typedef struct _app_event_t {
	_APP_EVENT type;
} _app_event_t;

#ifdef __cplusplus
extern "C" {
#endif

void _app_run(void);
void _app_exit(void);
void _app_wakeup(void);
bool _app_running(void);
void _app_on_event(void (*)(_app_event_t const*,void*), void*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_APP_H */
