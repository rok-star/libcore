#ifdef __linux__
    #define _POSIX_C_SOURCE 200809L
#endif

#include <time.h>
#include <stdint.h>
#include <pthread.h>
#include <libext/lock.h>
#include <libext/cond.h>
#include <libext/MACRO.h>

#define pthread_cond_init_E(a, b) { \
    int __err = pthread_cond_init(a, b); \
    if (__err != 0) \
        _ABORT("pthread_cond_init() failed: %s", strerror(__err)); \
}

#define pthread_cond_signal_E(a) { \
    int __err = pthread_cond_signal(a); \
    if (__err != 0) \
        _ABORT("pthread_cond_signal() failed: %s", strerror(__err)); \
}

#define pthread_cond_broadcast_E(a) { \
    int __err = pthread_cond_broadcast(a); \
    if (__err != 0) \
        _ABORT("pthread_cond_broadcast() failed: %s", strerror(__err)); \
}

#define pthread_cond_wait_E(a, b) { \
    int __err = pthread_cond_wait(a, b); \
    if (__err != 0) \
        _ABORT("pthread_cond_wait() failed: %s", strerror(__err)); \
}

#define pthread_cond_timedwait_E(a, b, c) { \
    int __err = pthread_cond_timedwait(a, b, c); \
    if (__err != 0) \
        _ABORT("pthread_cond_timedwait() failed: %s", strerror(__err)); \
}

#define clock_gettime_E(a, b) { \
    int __err = clock_gettime(a, b); \
    if (__err != 0) \
        _ABORT("clock_gettime() failed: %s\n", strerror(__err)); \
}

extern pthread_mutex_t* _lock_mutex(_lock_t*);

typedef struct _cond_t {
    pthread_cond_t cond;
} _cond_t;

_cond_t* _cond_create(void) {
    _cond_t* cond = _NEW(_cond_t, {});
    pthread_cond_init_E(&cond->cond, NULL);
    return cond;
}

void _cond_destroy(_cond_t* cond) {
    _ASSERT(cond != NULL);
    pthread_cond_destroy(&cond->cond);
    _FREE(cond);
}

void _cond_notify(_cond_t* cond) {
    _ASSERT(cond != NULL);
    pthread_cond_signal_E(&cond->cond);
}

void _cond_notify_all(_cond_t* cond) {
    _ASSERT(cond != NULL);
    pthread_cond_broadcast_E(&cond->cond);
}

void _cond_wait(_cond_t* cond, _lock_t* lock) {
    _ASSERT(cond != NULL);
    _ASSERT(lock != NULL);
    pthread_cond_wait_E(&cond->cond, _lock_mutex(lock));
}

void _cond_wait_timeout(_cond_t* cond, _lock_t* lock, double ms) {
    _ASSERT(cond != NULL);
    _ASSERT(lock != NULL);
    uint32_t ns = (((ms < 0) ? 0 : ms) * 1000000.0);
    struct timespec until = {};
    clock_gettime_E(CLOCK_REALTIME, &until);
    ns += until.tv_nsec;
    until.tv_sec += (ns / 1000000000);
    until.tv_nsec = (ns % 1000000000);
    pthread_cond_timedwait_E(&cond->cond, _lock_mutex(lock), &until);
}
