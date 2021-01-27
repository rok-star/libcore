#pragma once

#include <memory>
#include <libext/unicode.h>
#include <libext/cxx/base.h>
#include <libext/cxx/array.h>

namespace ext {

class string final {
private:
    ext::array<char> _data;
public:
    string()
        : _data() {}
    string(ext::string const& string)
        : _data(string._data) {}
    string(ext::string && string)
        : _data(std::move(string._data)) {}
    string(ext::array<char> const& array)
        : _data(array) {}
    string(ext::array<char> && array)
        : _data(std::move(array)) {}
    string(char const* pchar)
        : _data(pchar, strlen(pchar)) {}
    string(char const* pchar, int len)
        : _data(pchar, len) {}
    ext::string& operator=(ext::string const& string) {
        _data = string._data;
        return *this;
    }
    ext::string& operator=(ext::string && string) {
        _data = std::move(string._data);
        return *this;
    }
    ext::string& operator+=(ext::string const& string) {
        _data.push(string._data);
        return *this;
    }
    ext::string& operator+=(char const* pchar) {
        _data.push(ext::string(pchar)._data);
        return *this;
    }
    ext::string operator+(ext::string const& string) const {
        ext::string ret;
        ret.data().reserve(_data.size() + string._data.size());
        ret.data().push(_data);
        ret.data().push(string._data);
        return ret;
    }
    ext::string operator+(char const* pchar) const {
        ext::string ret;
        ext::string string(pchar);
        ret.data().reserve(_data.size() + string._data.size());
        ret.data().push(_data);
        ret.data().push(string._data);
        return ret;
    }
    bool operator==(ext::string const& string) const {
        return (_data == string._data);
    }
    bool operator==(char const* pchar) const {
        int size = strlen(pchar);
        if (_data.size() == size) {
            for (int i = 0; i < _data.size(); i++)
                if (_data[i] != pchar[i])
                    return false;
            return true;
        } else return false;
    }
    bool operator!=(ext::string const& string) const {
        return !operator==(string);
    }
    bool operator!=(char const* pchar) const {
        return !operator==(pchar);
    }
    ext::array<char> const& data() const {
        return _data;
    }
    ext::array<char>& data() {
        return _data;
    }
    ext::string substr(int index, int size) const {
        index = (index < 0) ? 0 : index;
        size = (size < 0) ? 0 : size;
        int index_ = -1;
        int size_ = -1;
        int from_ = 0;
        int to_ = 0;
        for (int i = 0; i < _data.size(); i++) {
            if ((_data[i] & 0xc0) != 0x80)
                index_ += 1;
            if (index_ == index) {
                from_ = i;
                for (; i <= _data.size(); i++) {
                    if (i == _data.size()) {
                        to_ = i;
                        break;
                    } else if ((_data[i] & 0xc0) != 0x80) {
                        size_ += 1;
                        if (size_ == size) {
                            to_ = i;
                            break;
                        }
                    }
                }
                break;
            }
        }
        return ((to_ - from_) > 0)
            ? ext::string(_data.data() + from_, (to_ - from_))
            : ext::string();
    }
    ext::string slice(int start, int end) const {
    	return ext::string();
    }
    ext::string first() const {
        ASSERT(size() > 0);
        return substr(0, 1);
    }
    ext::string last() const {
        ASSERT(size() > 0);
        return substr(size() - 1, 1);
    }
    ext::string first_or(ext::string const& string) const {
        return (size() > 0) ? substr(0, 1) : string;
    }
    ext::string last_or(ext::string const& string) const {
        int size_ = size();
        return (size_ > 0) ? substr(size_ - 1, 1) : string;
    }
    int size() const {
        int ret = 0;
        for (int i = 0; i < _data.size(); i++)
            if ((_data[i] & 0xc0) != 0x80)
                ret += 1;
        return ret;
    }
    ext::string& clear() {
        _data.clear();
        return *this;
    }
    ext::string& push(char char_) {
        _data.push(char_);
        return *this;
    }
    ext::string& push(ext::string const& string) {
        _data.push(string._data);
        return *this;
    }
    ext::string& unshift(char char_) {
        _data.unshift(char_);
        return *this;
    }
    ext::string& unshift(ext::string const& string) {
        _data.unshift(string._data);
        return *this;
    }
    bool empty() const {
        return _data.empty();
    }
    bool whitespace() const {
        if (_data.size() > 0) {
            int read = 0;
            int index = 0;
            uint32_t codepoint = 0;
            while ((read = _utf8_read_codepoint(_data.data(), (_data.size() - index), &codepoint)) > 0) {
                if (_codepoint_whitespace(codepoint) == false)
                    return false;
                index += read;
            }
            return true;
        } else {
            return false;
        }
    }
    bool digital()  const{
        if (_data.size() > 0) {
            int read = 0;
            int index = 0;
            uint32_t codepoint = 0;
            while ((read = _utf8_read_codepoint(_data.data(), (_data.size() - index), &codepoint)) > 0) {
                if (_codepoint_digital(codepoint) == false)
                    return false;
                index += read;
            }
            return true;
        } else {
            return false;
        }
    }
    ext::string upper() const {
        return ext::string();
    }
    ext::string lower() const {
        return ext::string();
    }
    ext::array<ext::string> split() const {
        return {};
    }
    ext::array<ext::string> split(char const* separator) const {
        return {};
    }
    ext::array<ext::string> split(ext::string const& separator) const {
        return {};
    }
    char const* c_string() const {
        static char const* _empty = "";
        if (_data.empty()) {
            return _empty;
        } else {
            const_cast<ext::string*>(this)->_data.push(0x0);
            const_cast<ext::string*>(this)->_data.pop();
            return _data.data();
        }
    }
    // ext::array<uint8_t> encode(ext::encoding encoding) {
    //     return {};
    // }
    ext::array<uint32_t> codepoints() const {
        ext::array<uint32_t> ret;
        ret.reserve(size());
        int read = 0;
        int index = 0;
        uint32_t codepoint = 0;
        while ((read = _utf8_read_codepoint(_data.data(), (_data.size() - index), &codepoint)) > 0) {
            ret.push(codepoint);
        }
        return ret;
    }
    static ext::string from_codepoints(ext::array<uint32_t> const& codepoints) {
        int pos = 0;
        int len = 0;
        for (uint32_t const& codepoint : codepoints)
            len += _codepoint_write_utf8(codepoint, NULL, 0);
        char* ret = ext::alloc<char>(len + 1);
        for (uint32_t const& codepoint : codepoints)
            pos += _codepoint_write_utf8(codepoint, (ret + pos), (len - pos));
        return ext::string::with_pchar(ret, len, (len + 1));
    }
    static ext::string with_pchar(char* pchar, int size) {
        return ext::string(ext::array<char>::with_data(pchar, size, size));
    }
    static ext::string with_pchar(char* pchar, int size, int capacity) {
        return ext::string(ext::array<char>::with_data(pchar, size, capacity));
    }
    static ext::string with_capacity(int capacity) {
        ext::string ret;
        ret.data().reserve(capacity);
        return ret;
    }
    template<typename... A>
    static ext::string format(A const&... args) {
        int len = snprintf(nullptr, 0, args...);
        char* pchar = ext::alloc<char>(len + 1);
        sprintf(pchar, args...);
        return ext::string::with_pchar(pchar, len, (len + 1));
    }
    // static ext::string decode(void const* data, ext::encoding encoding) {
    //     return "";
    // }
};

template<typename T>
inline ext::string ext::array<T>::join() const {
    return {};
}

template<typename T>
inline ext::string ext::array<T>::join(char const* separator) const {
    return {};
}

template<typename T>
inline ext::string ext::array<T>::join(ext::string const& separator) const {
    return {};
}

} /* namespace ext */