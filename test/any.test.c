#include <libcore/time.h>
#include <libcore/any.h>
#include <libcore/MACRO.h>

void _any_test(void) {
	char const* DUMMY_STRING = "Hello, World";
	double DUMMY_DATE = _time();

	// Creation...
	{
		_any_t* any = _any_create_null();
		_ASSERT(_any_type(any) == _NULL_ANY_TYPE);
		_any_destroy(any);
	} {
		_any_t* any = _any_create_string(DUMMY_STRING, strlen(DUMMY_STRING));
		_ASSERT(_any_type(any) == _STRING_ANY_TYPE);
		_ASSERT(strcmp(_any_string(any), DUMMY_STRING) == 0);
		_any_destroy(any);
	} {
		_any_t* any = _any_create_string("", strlen(""));
		_ASSERT(_any_type(any) == _STRING_ANY_TYPE);
		_ASSERT(strcmp(_any_string(any), "") == 0);
		_any_destroy(any);
	} {
		_any_t* any = _any_create_number(777);
		_ASSERT(_any_type(any) == _NUMBER_ANY_TYPE);
		_ASSERT(_any_number(any) == 777);
		_any_destroy(any);
	} {
		_any_t* any = _any_create_date(DUMMY_DATE);
		_ASSERT(_any_type(any) == _DATE_ANY_TYPE);
		_ASSERT(_any_date(any) == DUMMY_DATE);
		_any_destroy(any);
	} {
		_any_t* any = _any_create_array();
		_ASSERT(_any_type(any) == _ARRAY_ANY_TYPE);
		_ASSERT(_any_array_count(any) == 0);
		_any_destroy(any);
	} {
		_any_t* any = _any_create_map();
		_ASSERT(_any_type(any) == _MAP_ANY_TYPE);
		_ASSERT(_any_map_has(any, "one") == false);
		_ASSERT(_any_map_has(any, "two") == false);
		_ASSERT(_any_map_has(any, "three") == false);
		_any_destroy(any);
	}

	// Conversion...
	{
		_any_t* any = _any_create_null();
		_ASSERT(_any_type(any) == _NULL_ANY_TYPE);

		_any_set_string(any, DUMMY_STRING, strlen(DUMMY_STRING));
		_ASSERT(_any_type(any) == _STRING_ANY_TYPE);
		_ASSERT(strcmp(_any_string(any), DUMMY_STRING) == 0);

		_any_set_number(any, 777);
		_ASSERT(_any_type(any) == _NUMBER_ANY_TYPE);
		_ASSERT(_any_number(any) == 777);

		_any_set_date(any, DUMMY_DATE);
		_ASSERT(_any_type(any) == _DATE_ANY_TYPE);
		_ASSERT(_any_date(any) == DUMMY_DATE);

		_any_set_array(any);
		_ASSERT(_any_type(any) == _ARRAY_ANY_TYPE);
		_ASSERT(_any_array_count(any) == 0);

		_any_set_map(any);
		_ASSERT(_any_type(any) == _MAP_ANY_TYPE);
		_ASSERT(_any_map_has(any, "one") == false);
		_ASSERT(_any_map_has(any, "two") == false);
		_ASSERT(_any_map_has(any, "three") == false);

		_any_set_string(any, DUMMY_STRING, strlen(DUMMY_STRING));
		_ASSERT(_any_type(any) == _STRING_ANY_TYPE);
		_ASSERT(strcmp(_any_string(any), DUMMY_STRING) == 0);

		_any_set_date(any, DUMMY_DATE);
		_ASSERT(_any_type(any) == _DATE_ANY_TYPE);
		_ASSERT(_any_date(any) == DUMMY_DATE);

		_any_set_number(any, 777);
		_ASSERT(_any_type(any) == _NUMBER_ANY_TYPE);
		_ASSERT(_any_number(any) == 777);

		_any_destroy(any);
	}

	// String...
	{
		_any_t* any = _any_create_string(DUMMY_STRING, strlen(DUMMY_STRING));
		_ASSERT(_any_type(any) == _STRING_ANY_TYPE);
		_ASSERT(strcmp(_any_string(any), DUMMY_STRING) == 0);

		_any_set_string(any, DUMMY_STRING, 5);
		_ASSERT(_any_type(any) == _STRING_ANY_TYPE);
		_ASSERT(strcmp(_any_string(any), "Hello") == 0);

		_any_set_string(any, DUMMY_STRING, 0);
		_ASSERT(_any_type(any) == _STRING_ANY_TYPE);
		_ASSERT(strcmp(_any_string(any), "") == 0);

		_any_destroy(any);
	}
}