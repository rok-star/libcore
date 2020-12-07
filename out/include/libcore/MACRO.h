#ifndef _LIBCORE_MACRO_H
#define _LIBCORE_MACRO_H

#define _ABORT(...) { \
	fprintf(stderr, __VA_ARGS__); \
	fflush(stderr); \
	exit(1); \
}

#ifdef NDEBUG
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

#define _RESERVE(data, size, capacity, reserve) { \
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

#define _PUSH(data, size, capacity, ...) { \
	_ASSERT(size >= 0); \
	_ASSERT(capacity >= 0); \
	_ASSERT(size <= capacity); \
	if (size == capacity) { \
		__typeof__(capacity) __reserve = ((size * 2) + 1); \
		_RESERVE(data, size, capacity, __reserve); \
	} \
	data[size++] = (__VA_ARGS__); \
}

#define _POP(data, size, capacity) ({ \
	_ASSERT(data != NULL); \
	_ASSERT(size > 0); \
	_ASSERT(capacity > 0); \
	_ASSERT(size <= capacity); \
	__typeof__(*data) __ret = data[size - 1]; \
	size -= 1; \
	__ret; \
})

#define _UNSHIFT(data, size, capacity, ...) { \
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

#define _SHIFT(data, size, capacity, ...) ({ \
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

#define _REMOVE(data, size, capacity, index) { \
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

#define _RESERVE_A(array, reserve) _RESERVE((array).data, (array).size, (array).capacity, reserve)
#define _PUSH_A(array, ...) _PUSH((array).data, (array).size, (array).capacity, __VA_ARGS__)
#define _POP_A(array) _POP((array).data, (array).size, (array).capacity)
#define _UNSHIFT_A(array, ...) _UNSHIFT((array).data, (array).size, (array).capacity, __VA_ARGS__)
#define _SHIFT_A(array, ...) _SHIFT((array).data, (array).size, (array).capacity)
#define _REMOVE_A(array, index) _REMOVE((array).data, (array).size, (array).capacity, index)

#endif /* _LIBCORE_MACRO_H */
