#ifndef _LIBCORE_APP_H
#define _LIBCORE_APP_H

typedef struct _App _App;

_App* _App_create(void);
_App* _App_current(void);
void _App_destroy(_App*);
void _App_run(_App*);
void _App_exit(_App*);

#endif /* _LIBCORE_APP_H */
