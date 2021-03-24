#ifndef _LIBCORE_APP_H
#define _LIBCORE_APP_H

#include <stdbool.h>

typedef struct _app_t _app_t;

typedef enum : int {
    _EXIT_APP_EVENT = 0
} _APP_EVENT;

static char const* _APP_EVENT_NAME[1] = { "EXIT" };

typedef struct _app_event_t {
    _APP_EVENT type;
} _app_event_t;

#ifdef __cplusplus
extern "C" {
#endif

_app_t* _app_create(void);
void _app_destroy(_app_t*);
void _app_process(_app_t*);
void _app_on_event(_app_t*, void(*)(_app_event_t const*,void*),void*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_APP_H */
