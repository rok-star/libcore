#ifndef _LIBCORE_DATE_H
#define _LIBCORE_DATE_H

typedef enum {
    _DAY_DATE_PART			= 0,
    _WEEK_DATE_PART			= 1,
    _HOUR_DATE_PART			= 2,
    _MINUTE_DATE_PART		= 3,
    _SECOND_DATE_PART 		= 4,
    _MILLISECOND_DATE_PART 	= 5
} _DATE_PART

typedef struct _Date {
	double time;
	int year;
	int month;
	int date;
	int hour;
	int minute;
	int second;
	int millisecond;
} _Date;

#ifdef __cplusplus
extern "C" {
#endif

void _Date_now(_Date*);
void _Date_rewind(_Date*,_DATE_PART,int);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_DATE_H */
