#include <libcore/status.h>
#include <libcore/string.h>
#include <libcore/value.h>
#include <libcore/json.h>
#include <libcore/MACRO.h>

#include "output/sample02.c"
#include "output/sample01.c"
#include "output/sample10.c"
#include "output/sample03.c"

void _json_test(void) {
	_json_test_sample02();
	_json_test_sample01();
	//_json_test_sample10();
	_json_test_sample03();
}