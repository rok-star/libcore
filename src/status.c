#include <assert.h>
#include <libcore/MACRO.h>
#include <libcore/status.h>

void _status_set(_status_t* status, _STATUS_TYPE type, char const* message) {
	assert(status != NULL);
	status->type = type;
	if (message != NULL) {
		strncpy(status->message, message, (_STATUS_MAX_MESSAGE - 1));
	}
}