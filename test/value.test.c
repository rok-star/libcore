#include <libcore/time.h>
#include <libcore/value.h>
#include <libcore/MACRO.h>

void _value_test(void) {
	char const* DUMMY_STRING = "Hello, World";
	double DUMMY_DATE = _time();

	// Creation...
	{
		_value_t* value = _value_create_null();
		_ASSERT(_value_type(value) == _NULL_VALUE_TYPE);
		_value_destroy(value);
	} {
		_value_t* value = _value_create_string(DUMMY_STRING, strlen(DUMMY_STRING));
		_ASSERT(_value_type(value) == _STRING_VALUE_TYPE);
		_ASSERT(strcmp(_value_string(value), DUMMY_STRING) == 0);
		_value_destroy(value);
	} {
		_value_t* value = _value_create_string("", strlen(""));
		_ASSERT(_value_type(value) == _STRING_VALUE_TYPE);
		_ASSERT(strcmp(_value_string(value), "") == 0);
		_value_destroy(value);
	} {
		_value_t* value = _value_create_number(777);
		_ASSERT(_value_type(value) == _NUMBER_VALUE_TYPE);
		_ASSERT(_value_number(value) == 777);
		_value_destroy(value);
	} {
		_value_t* value = _value_create_date(DUMMY_DATE);
		_ASSERT(_value_type(value) == _DATE_VALUE_TYPE);
		_ASSERT(_value_date(value) == DUMMY_DATE);
		_value_destroy(value);
	} {
		_value_t* value = _value_create_array();
		_ASSERT(_value_type(value) == _ARRAY_VALUE_TYPE);
		_ASSERT(_value_array_count(value) == 0);
		_value_destroy(value);
	} {
		_value_t* value = _value_create_map();
		_ASSERT(_value_type(value) == _MAP_VALUE_TYPE);
		_ASSERT(_value_map_has(value, "one") == false);
		_ASSERT(_value_map_has(value, "two") == false);
		_ASSERT(_value_map_has(value, "three") == false);
		_value_destroy(value);
	}

	// Conversion...
	{
		_value_t* value = _value_create_null();
		_ASSERT(_value_type(value) == _NULL_VALUE_TYPE);

		_value_set_string(value, DUMMY_STRING, strlen(DUMMY_STRING));
		_ASSERT(_value_type(value) == _STRING_VALUE_TYPE);
		_ASSERT(strcmp(_value_string(value), DUMMY_STRING) == 0);

		_value_set_number(value, 777);
		_ASSERT(_value_type(value) == _NUMBER_VALUE_TYPE);
		_ASSERT(_value_number(value) == 777);

		_value_set_date(value, DUMMY_DATE);
		_ASSERT(_value_type(value) == _DATE_VALUE_TYPE);
		_ASSERT(_value_date(value) == DUMMY_DATE);

		_value_set_array(value);
		_ASSERT(_value_type(value) == _ARRAY_VALUE_TYPE);
		_ASSERT(_value_array_count(value) == 0);

		_value_set_map(value);
		_ASSERT(_value_type(value) == _MAP_VALUE_TYPE);
		_ASSERT(_value_map_has(value, "one") == false);
		_ASSERT(_value_map_has(value, "two") == false);
		_ASSERT(_value_map_has(value, "three") == false);

		_value_set_string(value, DUMMY_STRING, strlen(DUMMY_STRING));
		_ASSERT(_value_type(value) == _STRING_VALUE_TYPE);
		_ASSERT(strcmp(_value_string(value), DUMMY_STRING) == 0);

		_value_set_date(value, DUMMY_DATE);
		_ASSERT(_value_type(value) == _DATE_VALUE_TYPE);
		_ASSERT(_value_date(value) == DUMMY_DATE);

		_value_set_number(value, 777);
		_ASSERT(_value_type(value) == _NUMBER_VALUE_TYPE);
		_ASSERT(_value_number(value) == 777);

		_value_destroy(value);
	}

	// String...
	{
		_value_t* value = _value_create_string(DUMMY_STRING, strlen(DUMMY_STRING));
		_ASSERT(_value_type(value) == _STRING_VALUE_TYPE);
		_ASSERT(strcmp(_value_string(value), DUMMY_STRING) == 0);

		_value_set_string(value, DUMMY_STRING, 5);
		_ASSERT(_value_type(value) == _STRING_VALUE_TYPE);
		_ASSERT(strcmp(_value_string(value), "Hello") == 0);

		_value_set_string(value, DUMMY_STRING, 0);
		_ASSERT(_value_type(value) == _STRING_VALUE_TYPE);
		_ASSERT(strcmp(_value_string(value), "") == 0);

		_value_destroy(value);
	}

	// Number...
	{
		_value_t* value = _value_create_number(777);
		_ASSERT(_value_number(value) == 777);
		_value_set_number(value, 888);
		_ASSERT(_value_number(value) == 888);
		_value_destroy(value);
	}

	// Date...
	{
		_value_t* value = _value_create_date(DUMMY_DATE);
		_ASSERT(_value_date(value) == DUMMY_DATE);
		_value_set_date(value, DUMMY_DATE + 100);
		_ASSERT(_value_date(value) == (DUMMY_DATE + 100));
		_value_destroy(value);
	}

	// Array...
	{
		_value_t* value = _value_create_array();
		_ASSERT(_value_array_count(value) == 0);

		_value_array_push_move(value, _value_create_null());
		_value_array_push_move(value, _value_create_string(DUMMY_STRING, strlen(DUMMY_STRING)));
		_value_array_push_move(value, _value_create_number(777));
		_value_array_push_move(value, _value_create_date(DUMMY_DATE));
		_value_array_push_move(value, _value_create_array());
		_value_array_push_move(value, _value_create_map());

		_ASSERT(_value_array_count(value) == 6);
		_ASSERT(_value_type(_value_array_item(value, 0)) == _NULL_VALUE_TYPE);
		_ASSERT(_value_type(_value_array_item(value, 1)) == _STRING_VALUE_TYPE);
		_ASSERT(_value_type(_value_array_item(value, 2)) == _NUMBER_VALUE_TYPE);
		_ASSERT(_value_type(_value_array_item(value, 3)) == _DATE_VALUE_TYPE);
		_ASSERT(_value_type(_value_array_item(value, 4)) == _ARRAY_VALUE_TYPE);
		_ASSERT(_value_type(_value_array_item(value, 5)) == _MAP_VALUE_TYPE);

		_value_t* tmp1 = _value_array_pop(value);
		_value_destroy(tmp1);

		_ASSERT(_value_array_count(value) == 5);

		_value_destroy(value);
	}
}