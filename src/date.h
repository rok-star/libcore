#ifndef _LIBEXT_DATE_H
#define _LIBEXT_DATE_H

typedef struct _date_t _date_t;

#ifdef __cplusplus
extern "C" {
#endif

_date_t* _date_create(double);
void _date_destroy(_date_t*);
double _date_time(_date_t const*);
int _date_year(_date_t const*);
int _date_month(_date_t const*);
int _date_date(_date_t const*);
int _date_hour(_date_t const*);
int _date_minute(_date_t const*);
int _date_second(_date_t const*);
int _date_millisecond(_date_t const*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBEXT_DATE_H */
