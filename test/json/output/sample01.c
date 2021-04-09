void _json_test_sample01(void) {
	char const json[] = { 55, 55, 55, 0 };
	_status_t status = {};

	_value_t* root = _json_parse(json, sizeof json, &status);
	if (status.type == _FAILURE_STATUS_TYPE) {
		_ABORT("%s\n", status.message);
	}
	_ASSERT(_value_type(root) == _NUMBER_VALUE_TYPE);
	_ASSERT(_value_number(root) == 777);

	_value_destroy(root);
}