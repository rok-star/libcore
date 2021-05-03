#include <libcore/string.h>
#include <libcore/args.h>
#include <libcore/MACRO.h>

typedef struct _args_t {
	_args_schema_action_t const* action;
	struct {
		struct {
			_args_schema_option_t const* option;
			char const* value;
		} *data;
		int size;
		int capacity;
	} action_options;
	struct {
		struct {
			_args_schema_option_t const* option;
			char const* value;
		} *data;
		int size;
		int capacity;
	} root_options;
} _args_t;

static char const* YES = "yes";
static char const* NO = "no";

// static void __key_value(char const* argv, char const** key, int* key_len, char const** val, int* val_len) {
// 	_ASSERT(argv != NULL);
// 	_ASSERT(key != NULL);
// 	_ASSERT(val != NULL);
// 	int len = (int)strlen(argv);
// 	for (int i = 0; i < len; i++) {
// 		if (argv[i] == '=') {
// 			(*key) = argv;
// 			(*key_len) = i;
// 			(*val) = (argv + i + 1);
// 			(*val_len) = (len - (i + 1));
// 			return;
// 		}
// 	}
// 	(*key) = argv;
// 	(*key_len) = len;
// 	(*val) = NULL;
// 	(*val_len) = 0;
// }

// _args_t* _args_create(_args_schema_t const* schema, char const** argv, int argc, _status_t* status) {
// 	_ASSERT(schema != NULL);
// 	_ASSERT(argv != NULL);
// 	_ASSERT(status != NULL);

// 	_args_t* args = _NEW(_args_t, {});

// 	for (int i = 1; i < argc; i++) {
// 		for (int j = 0; j < schema->actions.size; j++) {
// 			if (strcmp(schema->actions[j].name, argv[i]) == 0) {
// 				args->action = &schema->actions[j];
// 				break;
// 			}
// 		}
// 		if (args->action != NULL) {
// 			break;
// 		}
// 	}

// 	for (int i = 1; i < argc; i++) {
// 		_args_scheme_option_t const* option = NULL;

// 		for (int j = 0; j < schema->options.size; j++) {
// 			char const* key = NULL;
// 			char const* value = NULL;
// 			int key_len = 0;
// 			int val_len = 0;
// 			__key_value(argv[i], &key, &key_len, &val, &val_len);
// 			if (key != NULL) {
// 				if (_string_compare(key, key_len, argv[i], strlen(argv[i]))) {
// 					if (val != NULL) {
// 						if ()
// 					} else {

// 					}
// 				}

// 				if (strcmp(schema->options[j].name, argv[i]) == 0) {
// 					option = &schema->options[j];
// 					break;
// 				}
// 			}

// 		}

// 		if (option != NULL) {
// 			_PUSH(args->root_options, option);
// 		} else {
// 			if (args->action != NULL) {
// 				for (int j = 0; j < args->action->options.size; j++) {
// 					if (strcmp(args->action->options[j].name, argv[i]) == 0) {
// 						option = &args->action->options[j];
// 						_PUSH(args->action_options, option);
// 						break;
// 					}
// 				}
// 			}

// 			if (option == NULL) {
// 				_STATUS_SET_FORMAT(status, _FAILURE_STATUS_TYPE, "unexpected argument \"%s\"", argv[i]);
// 				_FREE(args->action_options.data);
// 				_FREE(args->root_options.data);
// 				_FREE(args);
// 				return NULL;
// 			}
// 		}
// 	}

// 	return ret;
// }

// void _args_destroy(_args_t* args) {
// 	_ASSERT(args != NULL);
// 	_FREE(args->action_options.data);
// 	_FREE(args->root_options.data);
// 	_FREE(args);
// }

// int _args_action(_args_t* args) {
// 	_ASSERT(args != NULL);
// 	return (args->action != NULL ? args->action->id : -1);
// }

// char const* _args_option(_args_t* args, int option) {
// 	_ASSERT(args != NULL);
// 	for (int i = 0; i < args->root_options.size; i++) {
// 		if (args->root_options.data[i].id == options) {
// 			;
// 		}
// 	}
// 	return NULL;
// }