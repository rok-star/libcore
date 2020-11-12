#ifndef _LIBCORE_BASE_H
#define _LIBCORE_BASE_H

#define _ABORT(...) fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1);

#ifdef NDEBUG
	#define _ASSERT(a) {}
	#define _ASSERT_M(a, b) {}
#else
	#define _ASSERT(a) if (!(a)) _ABORT("libcore: Assertion failed: (%s) in file \"%s\" at line %d\n", #a, __FILE__, __LINE__)
	#define _ASSERT_M(a, b) if (!(a)) _ABORT("libcore: Assertion failed: %s, (%s) in file \"%s\" at line %d\n", #b, #a, __FILE__, __LINE__)
#endif

#endif /* _LIBCORE_BASE_H */
