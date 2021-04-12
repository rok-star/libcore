#include <libcore/MACRO.h>
#include <libcore/args.h>
#include <libcore/string.h>

// int _arg_has(char const** argv, int argc, char const* name, int64_t len, _PARAM_VALUE_SEPARATOR sep) {
// 	_ASSERT(argv != NULL);
// 	_ASSERT(name != NULL);
// 	if (len > 0) {
// 		for (int i = 0; i < argc; i++) {
// 			char const* arg_str = argv[i];
// 			int64_t arg_len = strlen(argv[i]);
// 			int64_t index = _string_index_of(arg_str, arg_len, "=", 1, 0);
// 			if (sep == _SPACE_PARAM_VALUE_SEPARATOR) {
// 				if (_string_compare(arg_str, arg_len, name, len)) {
// 					return i;
// 				}
// 			} else if (sep == _EQUALS_PARAM_VALUE_SEPARATOR) {
// 				if ((index > -1)
// 				&& (_string_compare(arg_str, index, name, len))) {
// 					return i;
// 				}
// 			} else if (sep == _BOTH_PARAM_VALUE_SEPARATOR) {
// 				if (_string_compare(arg_str, (index > -1 ? index : arg_len), name, len)) {
// 					return i;
// 				}
// 			}
// 		}
// 	}
// 	return -1;
// }

// int _arg_value(char const** argv, int argc, char const* name, int64_t len, _PARAM_VALUE_SEPARATOR sep, char const** value, int64_t* value_len) {
// 	_ASSERT(argv != NULL);
// 	_ASSERT(name != NULL);
// 	_ASSERT(value != NULL);
// 	_ASSERT(value_len != NULL);
// 	int i = _arg_has(argv, argc, name, len, sep);
// 	if (i > -1) {
// 		if (sep == _SPACE_PARAM_VALUE_SEPARATOR) {
// 			int ii = (i + 1);
// 			if (ii < argc) {
// 				(*value) = argv[ii];
// 				(*value_len) = strlen(argv[ii]);
// 				return ii;
// 			} else {
// 				return -1;
// 			}
// 		} else if (sep == _EQUALS_PARAM_VALUE_SEPARATOR) {
// 			int64_t index = _string_index_of(argv[i], strlen(argv[i]), "=", 1, 0);
// 			if (index > -1) {
// 				(*value) = argv[ii];
// 				(*value_len) = index;
// 				return i;
// 			} else {
// 				return -1;
// 			}
// 		} else if (sep == _BOTH_PARAM_VALUE_SEPARATOR) {
// 			int64_t arg_len = strlen(argv[i]);
// 			int64_t index = _string_index_of(argv[i], arg_len, "=", 1, 0);
// 			(*value) = argv[ii];
// 			(*value_len) = (index > -1 ? index : arg_len);
// 			return i;
// 		} else {
// 			return -1;
// 		}
// 	} else {
// 		return -1;
// 	}
// }