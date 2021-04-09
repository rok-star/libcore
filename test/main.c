#include "value.test.c"
#include "json/index.c"

int main(int argc, char const *argv[]) {

	_value_test();
	_json_test();

	puts("Nothing failed.");

	return 0;
}