#ifndef _LIBCORE_MACRO_H
#define _LIBCORE_MACRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <execinfo.h>

#define _ABORT(...) { \
	fprintf(stderr, __VA_ARGS__); \
	fflush(stderr); \
	_BACKTRACE(); \
	exit(1); \
}

#define _BACKTRACE() { \
	void* __buffer[10]; \
	int __nentries = backtrace(__buffer, 10); \
	char** __entries = backtrace_symbols(__buffer, __nentries); \
	if (__entries != NULL) { \
		for (int i = 0; i < __nentries; i++) \
			fprintf(stderr, "%s\n", __entries[i]); \
		fflush(stderr); \
		free(__entries); \
	} \
}

#ifdef NDEBUG
	#define _ASSERT(a)
	#define _ASSERT_M(a, b)
#else
	#define _ASSERT(a) if (!(a)) _ABORT("Assertion failed: (%s) in function \"%s\" in file \"%s\" at line %d\n", #a, __FUNCTION__, __FILE__, __LINE__)
	#define _ASSERT_M(a, b) if (!(a)) _ABORT("Assertion failed: %s, (%s) in function \"%s\" in file \"%s\" at line %d\n", #b, #a, __FUNCTION__, __FILE__, __LINE__)
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

#define _RESERVE_V(data, size, capacity, reserve) { \
	_ASSERT(size >= 0); \
	_ASSERT(capacity >= 0); \
	_ASSERT(reserve >= 0); \
	_ASSERT(size <= capacity); \
	if (reserve > capacity) { \
		__typeof__(data) __data = _ALLOC(__typeof__(*data), reserve); \
		if (data != NULL) { \
			memcpy(__data, data, size * sizeof(*data)); \
			_FREE(data); \
		} \
		data = __data; \
		capacity = reserve; \
	} \
}

#define _PUSH_V(data, size, capacity, ...) { \
	_ASSERT(size >= 0); \
	_ASSERT(capacity >= 0); \
	_ASSERT(size <= capacity); \
	if (size == capacity) { \
		__typeof__(capacity) __reserve = ((size * 2) + 1); \
		_RESERVE_V(data, size, capacity, __reserve); \
	} \
	data[size++] = (__VA_ARGS__); \
}

#define _POP_V(data, size, capacity) ({ \
	_ASSERT(data != NULL); \
	_ASSERT(size > 0); \
	_ASSERT(capacity > 0); \
	_ASSERT(size <= capacity); \
	__typeof__(*data) __ret = data[size - 1]; \
	size -= 1; \
	__ret; \
})

#define _UNSHIFT_V(data, size, capacity, ...) { \
	_ASSERT(size >= 0); \
	_ASSERT(capacity >= 0); \
	_ASSERT(size <= capacity); \
	if (size == capacity) { \
		__typeof__(capacity) __reserve = ((size * 2) + 1); \
		__typeof__(data) __data = _ALLOC(__typeof__(*data), __reserve); \
		__data[0] = (__VA_ARGS__); \
		if (data != NULL) { \
			for (int __i = 0; __i < size; __i++) \
				__data[__i + 1] = data[__i]; \
			_FREE(data); \
		} \
		data = __data; \
	} else { \
		for (int __i = (size - 1); __i >= 0; __i--) \
			data[__i + 1] = data[__i]; \
		data[0] = (__VA_ARGS__); \
	} \
	size += 1; \
}

#define _SHIFT_V(data, size, capacity, ...) ({ \
	_ASSERT(data != NULL); \
	_ASSERT(size > 0); \
	_ASSERT(capacity > 0); \
	_ASSERT(size <= capacity); \
	__typeof__(*data) __ret = data[0]; \
	for (int __i = 1; __i < size; __i++) \
		data[__i - 1] = data[__i]; \
	size -= 1; \
	__ret; \
})

#define _REMOVE_V(data, size, capacity, index) { \
	_ASSERT(data != NULL); \
	_ASSERT(size >= 0); \
	_ASSERT(capacity >= 0); \
	_ASSERT(size <= capacity); \
	if ((index >= 0) && (index < size)) { \
		for (int __i = (index + 1); __i < size; __i++) \
			data[__i - 1] = data[__i]; \
		size -= 1; \
	} \
}

#define _INDEX_OF_V(data, size, item) ({ \
	_ASSERT(data != NULL); \
	_ASSERT(size >= 0); \
	int __ret = -1; \
	for (int __i = 0; __i < size; __i++) { \
		if (data[__i] == item) { \
			__ret = __i; \
			break; \
		} \
	} \
	__ret; \
})

#define _FIRST_V(data, size) ({ \
	_ASSERT(data != NULL); \
	_ASSERT(size > 0); \
	__typeof__(*data) __ret = data[0]; \
	__ret; \
})

#define _LAST_v(data, size) ({ \
	_ASSERT(data != NULL); \
	_ASSERT(size > 0); \
	__typeof__(*data) __ret = data[size - 1]; \
	__ret; \
})

#define _RESERVE(array, reserve) _RESERVE_V((array).data, (array).size, (array).capacity, reserve)
#define _PUSH(array, ...) _PUSH_V((array).data, (array).size, (array).capacity, __VA_ARGS__)
#define _POP(array) _POP_V((array).data, (array).size, (array).capacity)
#define _UNSHIFT(array, ...) _UNSHIFT_V((array).data, (array).size, (array).capacity, __VA_ARGS__)
#define _SHIFT(array, ...) _SHIFT_V((array).data, (array).size, (array).capacity)
#define _REMOVE(array, index) _REMOVE_V((array).data, (array).size, (array).capacity, index)
#define _INDEX_OF(array, item) _INDEX_OF_V((array).data, (array).size, item)
#define _FIRST(array) _FIRST_V((array).data, (array).size);
#define _LAST(array) _LAST_v((array).data, (array).size);

#endif /* _LIBCORE_MACRO_H */
