#ifndef _LIBCORE_MACRO_H
#define _LIBCORE_MACRO_H

#define _ABORT(...) { \
	fprintf(stderr, __VA_ARGS__); \
	fflush(stderr); \
	exit(1); \
}

#ifndef NDEBUG
	#define _ASSERT(a)
	#define _ASSERT_M(a, b)
#else
	#define _ASSERT(a) if (!(a)) _ABORT("libcore: Assertion failed: (%s) in file \"%s\" at line %d\n", #a, __FILE__, __LINE__)
	#define _ASSERT_M(a, b) if (!(a)) _ABORT("libcore: Assertion failed: %s, (%s) in file \"%s\" at line %d\n", #b, #a, __FILE__, __LINE__)
#endif

#define _FREE(a) { \
	if (a != NULL) { \
		free(a); \
		a = NULL; \
	} \
}

#define _ALLOC(TYPE, N) ({ \
	TYPE* __ret = (TYPE*)calloc(N, sizeof(TYPE)); \
	_ASSERT(__ret != NULL); \
	__ret; \
})

#define _NEW(TYPE, ...) ({ \
	TYPE* __ret = _ALLOC(TYPE, 1); \
	*__ret = (TYPE)__VA_ARGS__; \
	__ret; \
})

#define _FORMAT(...) ({ \
	size_t __len = snprintf(NULL, 0, __VA_ARGS__); \
	char* __ret = _ALLOC(char, (__len + 1)); \
	snprintf(__ret, (__len + 1), __VA_ARGS__); \
	__ret; \
})

#define _MAX(a, b) ({ \
	__typeof__(a) __aa = (a); \
	__typeof__(b) __bb = (b); \
	((__aa > __bb) ? __aa : __bb); \
})

#define _MIN(a, b) ({ \
	__typeof__(a) __aa = (a); \
	__typeof__(b) __bb = (b); \
	((__aa < __bb) ? __aa : __bb); \
})

#define _PUSH(data, size, capacity, ...) { \
	if (size == capacity) { \
		__typeof__(data) __nd = _ALLOC(__typeof__(*data), ((size * 2) + 1)); \
		if (data != NULL) { \
			memcpy(__nd, data, size * sizeof(*data)); \
			_FREE(data); \
		} \
		data = __nd; \
		capacity = ((size * 2) + 1); \
	} \
	data[size++] = (__VA_ARGS__); \
}

#endif /* _LIBCORE_MACRO_H */
