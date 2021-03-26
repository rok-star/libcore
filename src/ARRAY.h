#ifndef _LIBCORE_ARRAY_H
#define _LIBCORE_ARRAY_H

#define _BASIC_COMPARATOR(a, b) a == b
#define _DUMMY_COMPARATOR(a, b) false
#define _STRING_COMPARATOR(a, b) strcmp(a, b) == 0

#define _ARRAY_TYPE(TYPENAME, TYPE, NAME) \
typedef struct TYPENAME { \
	TYPE* data; \
	int64_t capacity; \
	int64_t size; \
} TYPENAME; \
void NAME##_init(TYPENAME* array); \
void NAME##_done(TYPENAME* array); \
void NAME##_reserve(TYPENAME* array, int64_t capacity); \
void NAME##_resize(TYPENAME* array, int64_t size); \
void NAME##_attach(TYPENAME* array, TYPE* data, int64_t size, int64_t capacity); \
void NAME##_detach(TYPENAME* array); \
bool NAME##_is_empty(TYPENAME const* array); \
void NAME##_splice_data(TYPENAME* array, int64_t index, int64_t remove, TYPE const* data, int64_t data_size); \
void NAME##_splice_array(TYPENAME* array, int64_t index, int64_t remove, TYPENAME const* array2); \
void NAME##_splice(TYPENAME* array, int64_t index, int64_t remove, TYPE item); \
void NAME##_push_data(TYPENAME* array, TYPE const* data, int64_t data_size); \
void NAME##_push_array(TYPENAME* array, TYPENAME const* array2); \
void NAME##_push(TYPENAME* array, TYPE item); \
void NAME##_unshift_data(TYPENAME* array, TYPE const* data, int64_t data_size); \
void NAME##_unshift_array(TYPENAME* array, TYPENAME const* array2); \
void NAME##_unshift(TYPENAME* array, TYPE item); \
TYPE NAME##_pop(TYPENAME* array); \
TYPE NAME##_shift(TYPENAME* array); \
void NAME##_empty(TYPENAME* array); \
bool NAME##_remove_index(TYPENAME* array, int64_t index); \
bool NAME##_remove(TYPENAME* array, const TYPE item); \
int64_t NAME##_index_of(TYPENAME const* array, const TYPE item, int64_t start); \
bool NAME##_contains(TYPENAME const* array, const TYPE item); \
TYPE NAME##_first(TYPENAME const* array); \
TYPE NAME##_last(TYPENAME const* array); \
TYPE NAME##_next(TYPENAME const* array, const TYPE item); \
TYPE NAME##_prev(TYPENAME const* array, const TYPE item);

#define _ARRAY_TYPE_IMPL(TYPENAME, TYPE, NAME, COMPARATOR) \
void NAME##_init(TYPENAME* array) { \
	_ASSERT(array != NULL); \
	_ASSERT(array->data == NULL); \
	_ASSERT(array->capacity == 0); \
	_ASSERT(array->size == 0); \
} \
void NAME##_done(TYPENAME* array) { \
	_ASSERT(array != NULL); \
	if (array->data != NULL) { \
		free(array->data); \
	} \
	array->capacity = 0; \
	array->size = 0; \
	array->data = NULL; \
} \
void NAME##_reserve(TYPENAME* array, int64_t capacity) { \
	_ASSERT(array != NULL); \
	if (array->capacity < capacity) { \
		TYPE* nd = malloc(capacity * sizeof(TYPE)); \
		if (array->data != NULL) { \
			memcpy(nd, array->data, array->size * sizeof(TYPE)); \
			free(array->data); \
		} \
		array->data = nd; \
		array->capacity = capacity; \
	} \
} \
void NAME##_resize(TYPENAME* array, int64_t size) { \
	_ASSERT(array != NULL); \
	if (array->capacity < size) \
		NAME##_reserve(array, size); \
	array->size = size; \
} \
void NAME##_attach(TYPENAME* array, TYPE* data, int64_t size, int64_t capacity) { \
	_ASSERT(array != NULL); \
	_ASSERT(data != NULL); \
	_ASSERT(capacity > 0); \
	_ASSERT(size >= 0); \
	_ASSERT_M((array->data == NULL) && (array->size == 0) && (array->capacity == 0), \
				"array must be in detached mode (.data == NULL, .size == 0, .capacity == 0)"); \
	array->data = data; \
	array->size = size; \
	array->capacity = capacity; \
} \
void NAME##_detach(TYPENAME* array) { \
	_ASSERT(array != NULL); \
	array->data = NULL; \
	array->size = 0; \
	array->capacity = 0; \
} \
bool NAME##_is_empty(TYPENAME const* array) { \
	_ASSERT(array != NULL); \
	return (array->size == 0); \
} \
void NAME##_splice_data(TYPENAME* array, int64_t index, int64_t remove, TYPE const* data, int64_t data_size) { \
	_ASSERT(array != NULL); \
	_ASSERT((data != NULL && data_size > 0) || (data == NULL && data_size == 0)); \
	index = (index < 0 ? 0 : (index > array->size ? array->size : index)); \
	remove = (remove < 0 ? 0 : (remove > (array->size - index) ? (array->size - index) : remove)); \
	int64_t lf = 0; \
	int64_t lt = index > array->size ? array->size : index; \
	int64_t rf = (index + remove) > array->size ? array->size : (index + remove); \
	int64_t rt = array->size; \
	int64_t ns = (lt - lf) + data_size + (rt - rf); \
	int64_t nc = (ns > array->capacity) ? (ns * 2) : array->capacity; \
	TYPE* data_copy = _ALLOC(TYPE, data_size); \
	memcpy(data_copy, data, (data_size * sizeof(TYPE))); \
	if (nc > array->capacity) { \
		TYPE* nd = malloc(nc * sizeof(TYPE)); \
		memcpy(nd, array->data, (lt - lf) * sizeof(TYPE)); \
		memcpy(nd + lt, data_copy, data_size * sizeof(TYPE)); \
		memcpy(nd + lt + data_size, array->data + rf, (rt - rf) * sizeof(TYPE)); \
		if (array->data != NULL) { \
			free(array->data); \
		} \
		array->data = nd; \
		array->size = ns; \
		array->capacity = nc; \
	} else { \
		int64_t dl = data_size - remove; \
		memmove(array->data + rf + dl, array->data + rf, (rt - rf) * sizeof(TYPE)); \
		if (data != NULL) { \
			memcpy(array->data + lt, data_copy, data_size * sizeof(TYPE)); \
		} \
		array->size = ns; \
	} \
	_FREE(data_copy); \
} \
void NAME##_splice_array(TYPENAME* array, int64_t index, int64_t remove, TYPENAME const* array2) { \
	NAME##_splice_data(array, index, remove, array2->data, array2->size); \
} \
void NAME##_splice(TYPENAME* array, int64_t index, int64_t remove, TYPE item) { \
	NAME##_splice_data(array, index, remove, &item, 1); \
} \
void NAME##_push_data(TYPENAME* array, TYPE const* data, int64_t data_size) { \
	NAME##_splice_data(array, array->size, 0, data, data_size); \
} \
void NAME##_push_array(TYPENAME* array, TYPENAME const* array2) { \
	NAME##_splice_data(array, array->size, 0, array2->data, array2->size); \
} \
void NAME##_push(TYPENAME* array, TYPE item) { \
	NAME##_splice_data(array, array->size, 0, &item, 1); \
} \
void NAME##_unshift_data(TYPENAME* array, TYPE const* data, int64_t data_size) { \
	NAME##_splice_data(array, 0, 0, data, data_size); \
} \
void NAME##_unshift_array(TYPENAME* array, TYPENAME const* array2) { \
	NAME##_splice_data(array, 0, 0, array2->data, array2->size); \
} \
void NAME##_unshift(TYPENAME* array, TYPE item) { \
	NAME##_splice_data(array, 0, 0, &item, 1); \
} \
TYPE NAME##_pop(TYPENAME* array) { \
	_ASSERT(array != NULL); \
	_ASSERT(array->size > 0); \
	TYPE value = array->data[0]; \
	NAME##_splice_data(array, 0, 1, NULL, 0); \
	return value; \
} \
TYPE NAME##_shift(TYPENAME* array) { \
	_ASSERT(array != NULL); \
	_ASSERT(array->size > 0); \
	int64_t index = (array->size - 1); \
	TYPE value = array->data[index]; \
	NAME##_splice_data(array, index, 1, NULL, 0); \
	return value; \
} \
void NAME##_empty(TYPENAME* array) { \
	_ASSERT(array != NULL); \
	array->size = 0; \
} \
bool NAME##_remove_index(TYPENAME* array, int64_t index) { \
	if ((index >= 0) && (index < array->size)) { \
		NAME##_splice_data(array, index, 1, NULL, 0); \
		return true; \
	} else return false; \
} \
bool NAME##_remove(TYPENAME* array, const TYPE item) { \
	_ASSERT(array != NULL); \
	for (int64_t i = 0; i < array->size; i++) \
		if (COMPARATOR(array->data[i], item)) { \
			NAME##_splice_data(array, i, 1, NULL, 0); \
			return true; \
		} \
	return false; \
} \
int64_t NAME##_index_of(TYPENAME const* array, const TYPE item, int64_t start) { \
	_ASSERT(array != NULL); \
	start = (start < 0) ? 0 : ((start > array->size) ? array->size : start); \
	for (int64_t i = start; i < array->size; i++) \
		if (COMPARATOR(array->data[i], item)) \
			return i; \
	return -1; \
} \
bool NAME##_contains(TYPENAME const* array, const TYPE item) { \
	_ASSERT(array != NULL); \
	for (int64_t i = 0; i < array->size; i++) \
		if (COMPARATOR(array->data[i], item)) \
			return true; \
	return false; \
} \
TYPE NAME##_first(TYPENAME const* array) { \
	_ASSERT(array != NULL); \
	_ASSERT(array->size > 0); \
	return array->data[0]; \
} \
TYPE NAME##_last(TYPENAME const* array) { \
	_ASSERT(array != NULL); \
	_ASSERT(array->size > 0); \
	return array->data[array->size - 1]; \
} \
TYPE NAME##_next(TYPENAME const* array, const TYPE item) { \
	_ASSERT(array != NULL); \
	_ASSERT(array->size > 0); \
	int64_t index = NAME##_index_of(array, item, 0); \
	_ASSERT((index > -1) && (index < (array->size - 1))); \
	return array->data[index + 1]; \
} \
TYPE NAME##_prev(TYPENAME const* array, const TYPE item) { \
	_ASSERT(array != NULL); \
	_ASSERT(array->size > 0); \
	int64_t index = NAME##_index_of(array, item, 0); \
	_ASSERT(index > 0); \
	return array->data[index - 1]; \
}

#endif /* _LIBCORE_ARRAY_H */