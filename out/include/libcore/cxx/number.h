#pragma once

#include <libcore/cxx/base.h>

namespace ext {

class number {
private:
    double _data;
public:
    number(double number)
        : _data(number) {}
    number(ext::number const& number)
        : _data(number._data) {}
    number(char const* pchar) {
        ASSERT(pchar != nullptr);
        ASSERT(is_number(pchar));
        ;
    }
    number(ext::string const& string) {
        ASSERT(is_number(string));
        ;
    }
    double const& data() const {
        return _data;
    }
    double& data() {
        return _data;
    }
    double double_() { return _data; }
    float float_() { return _data; }
    int8_t int8() { return _data; }
    int16_t int16() { return _data; }
    int32_t int32() { return _data; }
    int64_t int64() { return _data; }
    uint8_t uint8() { return _data; }
    uint16_t uint16() { return _data; }
    uint32_t uint32() { return _data; }
    uint64_t uint64() { return _data; }
    static bool is_number(char const* pchar) {
        ASSERT(pchar != nullptr);
        return false;
    }
    static bool is_number(ext::string const& string) {
        return is_number(string.c_string());
    }
};

} /* namespace ext */