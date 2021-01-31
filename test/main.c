#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libext/string.h>
#include <libext/http.h>
#include <libext/MACRO.h>

void test_string_read_number(void) {

    puts(":: string :: numbers :: uint64_t\n");

    {
    struct {
		char const* string;
		uint64_t should;
		bool error;
	} uint64_test_data[] = {
		{ .string = "-123", .error = true },
		{ .string = "123456789", .should = 123456789 },
		{ .string = "2147483647", .should = 2147483647 },
		{ .string = "-2147483647", .error = true },
		{ .string = "2147483648", .should = 2147483648 },
		{ .string = "-2147483648", .error = true },
		{ .string = "214748364711", .should = 214748364711 },
		{ .string = "3147483647", .should = 3147483647 },
		{ .string = "- 1 2 3", .error = true },
		{ .string = " 123456789  ", .should = 123456789 },
		{ .string = " 2  1 47483647", .should = 2147483647 },
		{ .string = "- 2 1 4 7483647", .error = true },
		{ .string = "214 74 83 648", .should = 2147483648 },
		{ .string = "-214 7 4 83648", .error = true },
		{ .string = "2147 4 83 6  4711", .should = 214748364711 },
		{ .string = "31474 8 3 6 47", .should = 3147483647 },
		{ .string = "123a456789", .should = 123 },
		{ .string = "21474b83647", .should = 21474 },
		{ .string = "-2147483647", .error = true },
		{ .string = "214748x3648", .should = 214748 },
		{ .string = "-21474836b48", .error = true },
		{ .string = "21474836c4711", .should = 21474836 },
		{ .string = "31474836z47", .should = 31474836 },
		{ .string = "123456789.123", .should = 123456789 },
		{ .string = "21474836471234567.456", .should = 21474836471234567 },
		{ .string = "-2147483647.789", .error = true },
		{ .string = "2147483648.987", .should = 2147483648 },
		{ .string = "-2147483648.654321", .error = true },
		{ .string = "214748364711.123456", .should = 214748364711 },
		{ .string = "3147483647.987654321", .should = 3147483647 },
		{ .string = "999999999.888888888", .should = 999999999 },
		{ .string = NULL },
	};

    __typeof__(*uint64_test_data) *next = uint64_test_data;

    for (;;) {
        __typeof__(*uint64_test_data) item = *next;

        if (item.string == NULL) {
            break;
        }

        uint64_t value = 0;
        int64_t read = _string_read_uint64(item.string, strlen(item.string), true, &value);
        if (read > 0) {
            if (item.error) {
                printf("%s - FAIL, should be an error, but got %llu\n", item.string, value);
            } else if (value == item.should) {
                printf("%s - OK, equals, %llu\n", item.string, value);
            } else {
                printf("%s - FAIL, value isn't equal to correct (%llu)\n", item.string, value);
            }
        } else {
            if (!item.error) {
                printf("%s - FAIL, should be fine, but there's an error\n", item.string);
            } else {
                printf("%s - OK, error cought\n", item.string);
            }
        }

        next += 1;
    }

    }

    puts("\n:: string :: numbers :: uint32_t ::\n");

    {
    struct {
		char const* string;
		uint32_t should;
		bool error;
	} uint32_test_data[] = {
		{ .string = "-123", .error = true },
		{ .string = "123456789", .should = 123456789 },
		{ .string = "2147483647", .should = 2147483647 },
		{ .string = "-2147483647", .error = true },
		{ .string = "2147483648", .should = 2147483648 },
		{ .string = "-2147483648", .error = true },
		{ .string = "214748364711", .error = true },
		{ .string = "3147483647", .should = 3147483647 },
		{ .string = "- 1 2 3", .error = true },
		{ .string = " 123456789  ", .should = 123456789 },
		{ .string = " 2  1 47483647", .should = 2147483647 },
		{ .string = "- 2 1 4 7483647", .error = true },
		{ .string = "214 74 83 648", .should = 2147483648 },
		{ .string = "-214 7 4 83648", .error = true },
		{ .string = "2147 4 83 6  4711", .error = true },
		{ .string = "31474 8 3 6 47", .should = 3147483647 },
		{ .string = "123a456789", .should = 123 },
		{ .string = "21474b83647", .should = 21474 },
		{ .string = "-2147483647", .error = true },
		{ .string = "214748x3648", .should = 214748 },
		{ .string = "-21474836b48", .error = true },
		{ .string = "21474836c4711", .should = 21474836 },
		{ .string = "31474836z47", .should = 31474836 },
		{ .string = "123456789.123", .should = 123456789 },
		{ .string = "21474836471234567.456", .error = true },
		{ .string = "-2147483647.789", .error = true },
		{ .string = "2147483648.987", .should = 2147483648 },
		{ .string = "-2147483648.654321", .error = true },
		{ .string = "214748364711.123456", .error = true },
		{ .string = "3147483647.987654321", .should = 3147483647 },
		{ .string = "999999999.888888888", .should = 999999999 },
		{ .string = NULL },
	};

    __typeof__(*uint32_test_data) *next = uint32_test_data;

    for (;;) {
        __typeof__(*uint32_test_data) item = *next;

        if (item.string == NULL) {
            break;
        }

        uint32_t value = 0;
        int64_t read = _string_read_uint32(item.string, strlen(item.string), true, &value);
        if (read > 0) {
            if (item.error) {
                printf("%s - FAIL, should be an error, but got %u\n", item.string, value);
            } else if (value == item.should) {
                printf("%s - OK, equals, %u\n", item.string, value);
            } else {
                printf("%s - FAIL, value isn't equal to correct (%u)\n", item.string, value);
            }
        } else {
            if (!item.error) {
                printf("%s - FAIL, should be fine, but there's an error\n", item.string);
            } else {
                printf("%s - OK, error cought\n", item.string);
            }
        }

        next += 1;
    }

    }

    puts("\n:: string :: numbers :: uint16_t ::\n");

    {
    struct {
		char const* string;
		uint16_t should;
		bool error;
	} uint16_test_data[] = {
		{ .string = "-123", .error = true },
		{ .string = "123456789", .error = true },
		{ .string = "2147483647", .error = true },
		{ .string = "-2147483647", .error = true },
		{ .string = "2147483648", .error = true },
		{ .string = "-2147483648", .error = true },
		{ .string = "214748364711", .error = true },
		{ .string = "3147483647", .error = true },
		{ .string = "- 1 2 3", .error = true },
		{ .string = " 123456789  ", .error = true },
		{ .string = " 2  1 47483647", .error = true },
		{ .string = "- 2 1 4 7483647", .error = true },
		{ .string = "214 74 83 648", .error = true },
		{ .string = "-214 7 4 83648", .error = true },
		{ .string = "2147 4 83 6  4711", .error = true },
		{ .string = "31474 8 3 6 47", .error = true },
		{ .string = "123a456789", .should = 123 },
		{ .string = "21474b83647", .should = 21474 },
		{ .string = "-2147483647", .error = true },
		{ .string = "214748x3648", .error = true },
		{ .string = "-21474836b48", .error = true },
		{ .string = "21474836c4711", .error = true },
		{ .string = "31474836z47", .error = true },
		{ .string = "123456789.123", .error = true },
		{ .string = "21474836471234567.456", .error = true },
		{ .string = "-2147483647.789", .error = true },
		{ .string = "2147483648.987", .error = true },
		{ .string = "-2147483648.654321", .error = true },
		{ .string = "214748364711.123456", .error = true },
		{ .string = "3147483647.987654321", .error = true },
		{ .string = "999999999.888888888", .error = true },
		{ .string = NULL },
	};

    __typeof__(*uint16_test_data) *next = uint16_test_data;

    for (;;) {
        __typeof__(*uint16_test_data) item = *next;

        if (item.string == NULL) {
            break;
        }

        uint16_t value = 0;
        int64_t read = _string_read_uint16(item.string, strlen(item.string), true, &value);
        if (read > 0) {
            if (item.error) {
                printf("%s - FAIL, should be an error, but got %u\n", item.string, value);
            } else if (value == item.should) {
                printf("%s - OK, equals, %u\n", item.string, value);
            } else {
                printf("%s - FAIL, value isn't equal to correct (%u)\n", item.string, value);
            }
        } else {
            if (!item.error) {
                printf("%s - FAIL, should be fine, but there's an error\n", item.string);
            } else {
                printf("%s - OK, error cought\n", item.string);
            }
        }

        next += 1;
    }

    }

    puts("\n:: string :: numbers :: uint8_t ::\n");

    {
    struct {
		char const* string;
		uint8_t should;
		bool error;
	} uint8_test_data[] = {
		{ .string = "-123", .error = true },
		{ .string = "123456789", .error = true },
		{ .string = "2147483647", .error = true },
		{ .string = "-2147483647", .error = true },
		{ .string = "2147483648", .error = true },
		{ .string = "-2147483648", .error = true },
		{ .string = "214748364711", .error = true },
		{ .string = "3147483647", .error = true },
		{ .string = "- 1 2 3", .error = true },
		{ .string = " 123456789  ", .error = true },
		{ .string = " 2  1 47483647", .error = true },
		{ .string = "- 2 1 4 7483647", .error = true },
		{ .string = "214 74 83 648", .error = true },
		{ .string = "-214 7 4 83648", .error = true },
		{ .string = "2147 4 83 6  4711", .error = true },
		{ .string = "31474 8 3 6 47", .error = true },
		{ .string = "123a456789", .should = 123 },
		{ .string = "21474b83647", .error = true },
		{ .string = "-2147483647", .error = true },
		{ .string = "214748x3648", .error = true },
		{ .string = "-21474836b48", .error = true },
		{ .string = "21474836c4711", .error = true },
		{ .string = "31474836z47", .error = true },
		{ .string = "123456789.123", .error = true },
		{ .string = "21474836471234567.456", .error = true },
		{ .string = "-2147483647.789", .error = true },
		{ .string = "2147483648.987", .error = true },
		{ .string = "-2147483648.654321", .error = true },
		{ .string = "214748364711.123456", .error = true },
		{ .string = "3147483647.987654321", .error = true },
		{ .string = "999999999.888888888", .error = true },
		{ .string = NULL },
	};

    __typeof__(*uint8_test_data) *next = uint8_test_data;

    for (;;) {
        __typeof__(*uint8_test_data) item = *next;

        if (item.string == NULL) {
            break;
        }

        uint8_t value = 0;
        int64_t read = _string_read_uint8(item.string, strlen(item.string), true, &value);
        if (read > 0) {
            if (item.error) {
                printf("%s - FAIL, should be an error, but got %u\n", item.string, value);
            } else if (value == item.should) {
                printf("%s - OK, equals, %u\n", item.string, value);
            } else {
                printf("%s - FAIL, value isn't equal to correct (%u)\n", item.string, value);
            }
        } else {
            if (!item.error) {
                printf("%s - FAIL, should be fine, but there's an error\n", item.string);
            } else {
                printf("%s - OK, error cought\n", item.string);
            }
        }

        next += 1;
    }

    }

    puts("\n:: string :: numbers :: int64_t ::\n");

    {
    struct {
		char const* string;
		int64_t should;
		bool error;
	} int64_test_data[] = {
		{ .string = "-123", .should = -123 },
		{ .string = "123456789", .should = 123456789 },
		{ .string = "2147483647", .should = 2147483647 },
		{ .string = "-2147483647", .should = -2147483647 },
		{ .string = "2147483648", .should = 2147483648 },
		{ .string = "-2147483648", .should = -2147483648 },
		{ .string = "214748364711", .should = 214748364711 },
		{ .string = "3147483647", .should = 3147483647 },
		{ .string = "- 1 2 3", .should = -123 },
		{ .string = " 123456789  ", .should = 123456789 },
		{ .string = " 2  1 47483647", .should = 2147483647 },
		{ .string = "- 2 1 4 7483647", .should = - 2147483647 },
		{ .string = "214 74 83 648", .should = 2147483648 },
		{ .string = "-214 7 4 83648", .should = -2147483648 },
		{ .string = "2147 4 83 6  4711", .should = 214748364711 },
		{ .string = "31474 8 3 6 47", .should = 3147483647 },
		{ .string = "123a456789", .should = 123 },
		{ .string = "21474b83647", .should = 21474 },
		{ .string = "-2147483647", .should = -2147483647 },
		{ .string = "214748x3648", .should = 214748 },
		{ .string = "-21474836b48", .should = -21474836 },
		{ .string = "21474836c4711", .should = 21474836 },
		{ .string = "31474836z47", .should = 31474836 },
		{ .string = "123456789.123", .should = 123456789 },
		{ .string = "21474836471234567.456", .should = 21474836471234567 },
		{ .string = "-2147483647.789", .should = -2147483647 },
		{ .string = "2147483648.987", .should = 2147483648 },
		{ .string = "-2147483648.654321", .should = -2147483648 },
		{ .string = "214748364711.123456", .should = 214748364711 },
		{ .string = "3147483647.987654321", .should = 3147483647 },
		{ .string = "999999999.888888888", .should = 999999999 },
		{ .string = NULL },
	};

    __typeof__(*int64_test_data) *next = int64_test_data;

    for (;;) {
        __typeof__(*int64_test_data) item = *next;

        if (item.string == NULL) {
            break;
        }

        int64_t value = 0;
        int64_t read = _string_read_int64(item.string, strlen(item.string), true, &value);
        if (read > 0) {
            if (item.error) {
                printf("%s - FAIL, should be an error, but got %lld\n", item.string, value);
            } else if (value == item.should) {
                printf("%s - OK, equals, %lld\n", item.string, value);
            } else {
                printf("%s - FAIL, value isn't equal to correct (%lld)\n", item.string, value);
            }
        } else {
            if (!item.error) {
                printf("%s - FAIL, should be fine, but there's an error\n", item.string);
            } else {
                printf("%s - OK, error cought\n", item.string);
            }
        }

        next += 1;
    }

    }

    puts("\n:: string :: numbers :: int32_t ::\n");

    {
    struct {
		char const* string;
		int32_t should;
		bool error;
	} int32_test_data[] = {
		{ .string = "-123", .should = -123 },
		{ .string = "123456789", .should = 123456789 },
		{ .string = "2147483647", .should = 2147483647 },
		{ .string = "-2147483647", .should = -2147483647 },
		{ .string = "2147483648", .error = true },
		{ .string = "-2147483648", .error = true },
		{ .string = "214748364711", .error = true },
		{ .string = "3147483647", .error = true },
		{ .string = "- 1 2 3", .should = -123 },
		{ .string = " 123456789  ", .should = 123456789 },
		{ .string = " 2  1 47483647", .should = 2147483647 },
		{ .string = "- 2 1 4 7483647", .should = - 2147483647 },
		{ .string = "214 74 83 648", .error = true },
		{ .string = "-214 7 4 83648", .error = true },
		{ .string = "2147 4 83 6  4711", .error = true },
		{ .string = "31474 8 3 6 47", .error = true },
		{ .string = "123a456789", .should = 123 },
		{ .string = "21474b83647", .should = 21474 },
		{ .string = "-2147483647", .should = -2147483647 },
		{ .string = "214748x3648", .should = 214748 },
		{ .string = "-21474836b48", .should = -21474836 },
		{ .string = "21474836c4711", .should = 21474836 },
		{ .string = "31474836z47", .should = 31474836 },
		{ .string = "123456789.123", .should = 123456789 },
		{ .string = "21474836471234567.456", .error = true },
		{ .string = "-2147483647.789", .should = -2147483647 },
		{ .string = "2147483648.987", .error = true },
		{ .string = "-2147483648.654321", .error = true },
		{ .string = "214748364711.123456", .error = true },
		{ .string = "3147483647.987654321", .error = true },
		{ .string = "999999999.888888888", .should = 999999999 },
		{ .string = NULL },
	};

    __typeof__(*int32_test_data) *next = int32_test_data;

    for (;;) {
        __typeof__(*int32_test_data) item = *next;

        if (item.string == NULL) {
            break;
        }

        int32_t value = 0;
        int64_t read = _string_read_int32(item.string, strlen(item.string), true, &value);
        if (read > 0) {
            if (item.error) {
                printf("%s - FAIL, should be an error, but got %d\n", item.string, value);
            } else if (value == item.should) {
                printf("%s - OK, equals, %d\n", item.string, value);
            } else {
                printf("%s - FAIL, value isn't equal to correct (%d)\n", item.string, value);
            }
        } else {
            if (!item.error) {
                printf("%s - FAIL, should be fine, but there's an error\n", item.string);
            } else {
                printf("%s - OK, error cought\n", item.string);
            }
        }

        next += 1;
    }

    }
}

void test_http(void) {
	char const* request = "GET /video.ogv HTTP/1.1\r\nFragment: track=audio\r\nRange: npt=00:10-00:20\r\nAccept-Range-Refer: bytes\r\n\r\n";
	char const* response = "HTTP 200 OK\r\nContent-Length: 600\r\nContent-Range: npt 00:10-00:20/01:20\r\nContent-Fragment: track=audio\r\nVary: Fragment, Accept-Range-Refer\r\nRange-Refer: this bytes 0-600, http://www.example.com/video.ogv#track=audio bytes 20000-30000\r\n\r\n";

	_status_t status = {};
	_http_message_t* req = _http_message_create(request, strlen(request), _REQUEST_HTTP_MESSAGE_TYPE, &status);

	if (status.type == _FAILURE_STATUS_TYPE) {
		_OUTPUT("%s\n", status.message);
	} else {
		_OUTPUT("method: \"%.*s\"\n", (int)req->method.size, req->method.data);
		_OUTPUT("url: \"%.*s\"\n", (int)req->url.size, req->url.data);
		_OUTPUT("version: \"%.*s\"\n", (int)req->version.size, req->version.data);
	}
}

int main(int argc, char const *argv[]) {

    test_http();

    return 0;
}
