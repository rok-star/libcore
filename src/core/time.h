#ifndef _LIBCORE_TIME_H
#define _LIBCORE_TIME_H

extern double const _TIME_MILLISECOND;
extern double const _TIME_SECOND;
extern double const _TIME_MINUTE;
extern double const _TIME_HOUR;

#ifdef __cplusplus
extern "C" {
#endif

double _time(void);
double _time_local(void);

#ifdef __cplusplus
}
#endif

#endif /* _LIBCORE_TIME_H */
