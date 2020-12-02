#ifndef _LIBCORE_DATE_H
#define _LIBCORE_DATE_H

typedef struct _Date _Date;

#ifdef __cplusplus
extern "C" {
#endif

_Date* _Date_create(double);
void _Date_destroy(_Date*);
double _Date_time(_Date const*);
int _Date_year(_Date const*);
int _Date_month(_Date const*);
int _Date_date(_Date const*);
int _Date_hour(_Date const*);
int _Date_minute(_Date const*);
int _Date_second(_Date const*);
int _Date_millisecond(_Date const*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_DATE_H */
