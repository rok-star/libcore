#ifdef __linux__
    #define _POSIX_C_SOURCE 200809L
#endif

#include <assert.h>
#include <pthread.h>
#include <libcore/lock.h>
#include <libcore/MACRO.h>

#define pthread_mutex_init_E(a, b) { \
    int __err = pthread_mutex_init(a, b); \
    if (__err != 0) \
        _ABORT("pthread_mutex_init() failed: %s\n", strerror(__err)); \
}

#define pthread_mutex_lock_E(a) { \
    int __err = pthread_mutex_lock(a); \
    if (__err != 0) \
        _ABORT("pthread_mutex_lock() failed: %s\n", strerror(__err)); \
}

#define pthread_mutex_unlock_E(a) { \
    int __err = pthread_mutex_unlock(a); \
    if (__err != 0) \
        _ABORT("pthread_mutex_unlock() failed: %s\n", strerror(__err)); \
}

typedef struct _lock_t {
    pthread_mutex_t mutex;
} _lock_t;

_lock_t* _lock_create(void) {
    _lock_t* lock = _NEW(_lock_t, {});
    pthread_mutex_init_E(&lock->mutex, NULL);
    return lock;
}

void _lock_destroy(_lock_t* lock) {
    assert(lock != NULL);
    pthread_mutex_destroy(&lock->mutex);
    _FREE(lock);
}

void _lock_acquire(_lock_t* lock) {
    assert(lock != NULL);
    pthread_mutex_lock_E(&lock->mutex);
}

void _lock_release(_lock_t* lock) {
    assert(lock != NULL);
    pthread_mutex_unlock_E(&lock->mutex);
}

pthread_mutex_t* _lock_mutex(_lock_t* lock) {
    assert(lock != NULL);
    return &lock->mutex;
}