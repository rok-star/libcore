#include <libcore/string.h>
#include <libcore/args.h>
#include <libcore/MACRO.h>

typedef struct _args_t {
	int action;
	struct {
		struct {
			int option;
			char const* value;
		} *data;
		int size;
		int capacity;
	} root_options;
} _args_t;

_args_t* _args_create(_args_schema_t const* schema, char const** argv, int argc, _status_t* status) {
	_ASSERT(schema != NULL);
	_ASSERT(argv != NULL);
	_ASSERT(status != NULL);

	_args_action_t const* action = NULL;

	struct {
		_args_option_t const** data;
		int size;
		int capacity;
	} action_options;

	struct {
		_args_option_t const** data;
		int size;
		int capacity;
	} root_options;

	for (int i = 1; i < argc; i++) {
		bool found = false;

		if (action != NULL) {
			for (int j = 0; j < action->options.size; j++) {
				if (strcmp(action->options[j].name, argv[i]) == 0) {
					_PUSH(action_options, &action->options[j]);
					found = true;
					break;
				}
			}
		} else {
			for (int j = 0; j < schema->actions.size; j++) {
				if (strcmp(schema->actions[j].name, argv[i]) == 0) {
					action = &schema->actions[j];
					found = true;
					break;
				}
			}
			if (!found) {
				for (int j = 0; j < schema->options.size; j++) {
					if (strcmp(schema->options[j].name, argv[i]) == 0) {
						_PUSH(root_options, &schema->options[j]);
						found = true;
						break;
					}
				}
			}
		}

		if (!found) {
			_STATUS_SET_FORMAT(status, _FAILURE_STATUS_TYPE, "unexpected argument \"%s\"", argv[i]);
			return NULL;
		}
	}



	return _NEW(_args_t, {});
}

void _args_destroy(_args_t* args) {
	_ASSERT(args != NULL);
	_FREE(args);
}

int _args_action(_args_t* args) {
	_ASSERT(args != NULL);
	return args->action;
}

double _args_option_number(_args_t* args, int option) {
	_ASSERT(args != NULL);
	return 0;
}

char const* _args_option_string(_args_t* args, int option) {
	_ASSERT(args != NULL);
	return NULL;
}