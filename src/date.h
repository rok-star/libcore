#ifndef _LIBCORE_DATE_H
#define _LIBCORE_DATE_H

typedef struct _Date _Date;

#ifdef __cplusplus
extern "C" {
#endif

_Date* _Date_create(double);
void _Date_destroy(_Date*);
double _Date_time(_Date*);
int _Date_year(_Date*);
int _Date_month(_Date*);
int _Date_date(_Date*);
int _Date_hour(_Date*);
int _Date_minute(_Date*);
int _Date_second(_Date*);
int _Date_millisecond(_Date*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_DATE_H */
