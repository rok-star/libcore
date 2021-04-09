void _json_test_sample03(void) {
	char const json[] = { 91, 32, 49, 44, 32, 50, 44, 32, 51, 32, 93, 0 };
	_status_t status = {};

	_value_t* root = _json_parse(json, sizeof json, &status);
	if (status.type == _FAILURE_STATUS_TYPE) {
		_ABORT("%s\n", status.message);
	}
	_ASSERT(_value_type(root) == _ARRAY_VALUE_TYPE);
	_ASSERT(_value_array_count(root) == 3);

	_value_t* tmp1 = _value_array_get(root, 0);
	_ASSERT(_value_type(tmp1) == _NUMBER_VALUE_TYPE);
	_ASSERT(_value_number(tmp1) == 1);

	_value_t* tmp2 = _value_array_get(root, 1);
	_ASSERT(_value_type(tmp2) == _NUMBER_VALUE_TYPE);
	_ASSERT(_value_number(tmp2) == 2);

	_value_t* tmp3 = _value_array_get(root, 2);
	_ASSERT(_value_type(tmp3) == _NUMBER_VALUE_TYPE);
	_ASSERT(_value_number(tmp3) == 3);

	_value_destroy(root);
}