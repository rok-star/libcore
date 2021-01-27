#pragma once

#include <memory>

#ifdef NDEBUG
    #define ASSERT(expr)
    #define ASSERT_M(expr, message)
#else
    #define ASSERT(expr) { \
        if (!(expr)) \
            ext::abort("Assertion failed: (%s) in file \"%s\" at line %d\n", #expr, __FILE__, __LINE__); \
    }
    #define ASSERT_M(expr, message) { \
        if (!(expr)) \
            ext::abort("Assertion failed: %s, (%s) in file \"%s\" at line %d\n", #message, #expr, __FILE__, __LINE__); \
    }
#endif

#define SAFE_FREE(obj) { \
    if (obj != nullptr) { \
        ::free(obj); \
        obj = nullptr; \
    } \
}
#define SAFE_DELETE(obj) { \
    if (obj != nullptr) { \
        delete obj; \
        obj = nullptr; \
    } \
}
#define SAFE_DELETE_A(obj) { \
    if (obj != nullptr) { \
        delete[] obj; \
        obj = nullptr; \
    } \
}

namespace ext {

template<typename T>
inline T* alloc(int size) {
    size = (size < 0 ? 0 : size);
    void* ret = ::malloc(sizeof(T) * (size_t)size);
    ::memset(ret, 0x0, (sizeof(T) * (size_t)size));
    return static_cast<T*>(ret);
}

template<typename T>
inline void free(T* obj) {
    ::free(obj);
}

template<typename... A>
inline void output(char const* a, A const&... args) {
    ::fprintf(stdout, a, args...);
    ::fflush(stdout);
}

template<typename... A>
inline void error(char const* a, A const&... args) {
    ::fprintf(stderr, a, args...);
    ::fflush(stderr);
}

template<typename... A>
inline void abort(char const* a, A const&... args) {
    ext::error(a, args...);
    ::exit(0);
}

template<typename T, typename... A>
inline T max(T const& value, A const&... rest) {
    static_assert(((std::is_same_v<T, std::decay_t<A>>) && ...), "***");
    T max_ = value;
    ((max_ = rest > max_ ? rest : max_), ...);
    return max_;
}

template<typename T, typename... A>
inline T min(T const& value, A const&... rest) {
    static_assert(((std::is_same_v<T, std::decay_t<A>>) && ...), "***");
    T min_ = value;
    ((min_ = rest < min_ ? rest : min_), ...);
    return min_;
}

} /* namespace ext */