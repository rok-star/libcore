#ifndef _LIBCORE_RUNLOOP_H
#define _LIBCORE_RUNLOOP_H

#include <stdbool.h>

typedef enum {
	_RUN_RUNLOOP_EVENT,
	_EXIT_RUNLOOP_EVENT,
	_SPIN_RUNLOOP_EVENT
} _RUNLOOP_EVENT;

typedef struct _RunloopEvent {
	_RUNLOOP_EVENT type;
} _RunloopEvent;

#ifdef __cplusplus
extern "C" {
#endif

void _Runloop_run(void (*)(_RunloopEvent const*,void*), void*);
void _Runloop_exit(void);
void _Runloop_wakeup(void);
bool _Runloop_running(void);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_RUNLOOP_H */
