void _json_test_sample02(void) {
	char const json[] = { 34, 83, 105, 109, 112, 108, 101, 32, 115, 116, 114, 105, 110, 103, 34, 0 };
	_status_t status = {};

	_value_t* root = _json_parse(json, sizeof json, &status);
	if (status.type == _FAILURE_STATUS_TYPE) {
		_ABORT("%s\n", status.message);
	}
	_ASSERT(_value_type(root) == _STRING_VALUE_TYPE);
	_ASSERT(_string_compare(_value_string(root), strlen(_value_string(root)), "Simple string", 13) == true);

	_value_destroy(root);
}