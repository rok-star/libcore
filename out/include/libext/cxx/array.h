#pragma once

#include <initializer_list>

namespace ext {

class string;

template<typename T>
class array final {
private:
    int64_t _capacity;
    int64_t _size;
    T* _data;
public:
    ~array() {
        for (int64_t i = 0; i < _size; i++)
            _data[i].~T();
        SAFE_FREE(_data);
    }
    array()
        : _capacity(0)
        , _size(0)
        , _data(nullptr) {}
    array(ext::array<T> const& array)
        : _capacity(array._capacity)
        , _size(array._size)
        , _data(ext::alloc<T>(array._capacity)) {
        for (int64_t i = 0; i < array._size; i++)
            new (_data + i) T(array._data[i]);
    }
    array(ext::array<T> && array)
        : _capacity(array._capacity)
        , _size(array._size)
        , _data(array._data) {
        array._capacity = 0;
        array._size = 0;
        array._data = nullptr;
    }
    array(std::initializer_list<T> const& list)
        : _capacity(list.size())
        , _size(list.size())
        , _data(ext::alloc<T>(list.size())) {
        T* dst = _data;
        for (T const& item : list)
            new (dst++) T(item);
    }
    array(T const* data, int64_t size)
        : _capacity(size)
        , _size(size)
        , _data(ext::alloc<T>(size)) {
        ASSERT(data != nullptr);
        for (int64_t i = 0; i < size; i++)
            new (_data + i) T(data[i]);
    }
    ext::array<T>& operator=(ext::array<T> const& array) {
        if (this != &array) {
            clear();
            reserve(array._size);
            _size = array._size;
            for (int64_t i = 0; i < array._size; i++)
                new (_data + i) T(array._data[i]);
        }
        return *this;
    }
    ext::array<T>& operator=(ext::array<T> && array) {
        if (this != &array) {
            T* td = _data;
            int64_t ts = _size;
            int64_t tc = _capacity;
            _data = array._data;
            _size = array._size;
            _capacity = array._capacity;
            array._data = td;
            array._size = ts;
            array._capacity = tc;
        }
        return *this;
    }
    ext::array<T>& operator+=(T const& item) {
        push(item);
        return *this;
    }
    ext::array<T>& operator+=(ext::array<T> const& array) {
        push(array);
        return *this;
    }
    ext::array<T> operator+(T const& item) const {
        ext::array<T> ret;
        ret.push(item);
        return ret;
    }
    ext::array<T> operator+(ext::array<T> const& item) const {
        ext::array<T> ret;
        ret.push(item);
        return ret;
    }
    bool operator==(ext::array<T> const& array) const {
        if (_size == array._size) {
            for (int64_t i = 0; i < _size; i++)
                if (_data[i] != array._data[i])
                    return false;
            return true;
        } else return false;
    }
    bool operator!=(ext::array<T> const& array) const {
        return !operator==(array);
    }
    T const& operator[](int64_t index) const {
        ASSERT((index >= 0) && (index < _size));
        return _data[index];
    }
    T& operator[](int64_t index) {
        ASSERT((index >= 0) && (index < _size));
        return _data[index];
    }
    int64_t size() const {
        return _size;
    }
    int64_t capacity() const {
        return _capacity;
    }
    T const* begin() const {
        return _data;
    }
    T* begin() {
        return _data;
    }
    T const* end() const {
        return (_data + _size);
    }
    T* end() {
        return (_data + _size);
    }
    T const* data() const {
        return _data;
    }
    T* data() {
        return _data;
    }
    T const& first() const {
        ASSERT(_size > 0);
        return _data[0];
    }
    T& first() {
        ASSERT(_size > 0);
        return _data[0];
    }
    T const& last() const {
        ASSERT(_size > 0);
        return _data[_size - 1];
    }
    T& last() {
        ASSERT(_size > 0);
        return _data[_size - 1];
    }
    T const& first_or(T const& item) const {
        return (_size > 0) ? _data[0] : item;
    }
    T const& last_or(T const& item) const {
        return (_size > 0) ? _data[_size - 1] : item;
    }
    ext::array<T> subarr(int64_t index, int64_t size) const {
        index = (index < 0) ? 0 : ((index > _size) ? _size : index);
        size = (size < 0) ? 0 : ((size > (_size - index)) ? (_size - index) : size);
        return ext::array(_data + index, size);
    }
    ext::array<T> slice(int64_t start, int64_t end) const {
        ext::abort("not implemented");
        return ext::array<T>();
    }
    int64_t index_of(T const& item, int64_t start = 0) const {
        start = (start < 0) ? 0 : start;
        for (int64_t i = start; i < _size; i++)
            if (_data[i] == item)
                return i;
        return -1;
    }
    int64_t index_of(ext::array<T> const& array, int64_t start = 0) const {
        if (array._size > 0) {
            for (int64_t i = start; i < _size; i++) {
                if ((_size - i) >= array._size) {
                    for (int64_t j = 0; j < array._size; j++)
                        if (_data[i + j] != array[j])
                            goto __continue;
                    return i;
                } else return -1;
                __continue: ;
            }
            return -1;
        } else return -1;
    }
    bool includes(T const& item) const {
        return (index_of(item) > -1);
    }
    bool includes(ext::array<T> const& array) const {
        return (index_of(array) > -1);
    }
    bool empty() const {
        return (_size == 0);
    }
    void detach() {
        _data = nullptr;
        _size = 0;
        _capacity = 0;
    }
    ext::array<T>& clear() {
        for (int64_t i = 0; i < _size; i++)
            _data[i].~T();
        _size = 0;
        return *this;
    }
    ext::array<T>& splice(int64_t index, int64_t remove, ext::array<T> const& array) {
        index = (index < 0) ? 0 : ((index > _size) ? _size : index);
        remove = (remove < 0) ? 0 : ((remove > (_size - index)) ? (_size - index) : remove);
        ext::array<T> copy(array);
        ;
        _size -= (remove + copy._size);
        return *this;
    }
    ext::array<T>& splice(int64_t index, int64_t remove, T const& item) {
        index = (index < 0) ? 0 : ((index > _size) ? _size : index);
        remove = (remove < 0) ? 0 : ((remove > (_size - index)) ? (_size - index) : remove);
        T copy(item);
        ;
        _size -= (remove + 1);
        return *this;
    }
    ext::array<T>& splice(int64_t index, int64_t remove) {
        index = (index < 0) ? 0 : ((index > _size) ? _size : index);
        remove = (remove < 0) ? 0 : ((remove > (_size - index)) ? (_size - index) : remove);
        for (int64_t i = index; i < (index + remove); i++)
            _data[i].~T();
        for (int64_t i = (index + remove); i < _size; i++)
            new (_data + (i - remove)) T(std::move(_data[i]));
        _size -= remove;
        return *this;
    }
    ext::array<T>& push(T const& item) {
        if (_capacity == _size) {
            T copy(item);
            reserve(_size * 2);
            new (_data + _size) T(std::move(copy));
        } else {
            new (_data + _size) T(item);
        }
        _size += 1;
        return *this;
    }
    ext::array<T>& push(ext::array<T> const& array) {
        if (_capacity < (_size + array._size)) {
            ext::array<T> copy(array);
            reserve((_size + copy._size) * 2);
            for (int64_t i = 0; i < copy._size; i++)
                new (_data + _size + i) T(std::move(copy._data[i]));
        } else {
            for (int64_t i = 0; i < array._size; i++)
                new (_data + _size + i) T(array._data[i]);
        }
        _size += array._size;
        return *this;
    }
    ext::array<T>& unshift(T const& item) {
        T copy(item);
        if (_capacity == _size)
            reserve(_size * 2);
        for (int64_t i = (_size - 1); i >= 0; i--)
            new (_data + i + 1) T(std::move(_data[i]));
        new (_data) T(std::move(copy));
        _size += 1;
        return *this;
    }
    ext::array<T>& unshift(ext::array<T> const& array) {
        ext::array<T> copy(array);
        if (_capacity < (_size + copy._size))
            reserve((_size + copy._size) * 2);
        for (int64_t i = (_size - 1); i >= 0; i--)
            new (_data + i + copy._size) T(std::move(_data[i]));
        for (int64_t i = 0; i < copy._size; i++)
            new (_data + i) T(std::move(copy._data[i]));
        _size += array._size;
        return *this;
    }
    T pop() {
        ASSERT(_size > 0);
        T ret(std::move(_data[_size - 1]));
        _data[_size - 1].~T();
        _size -= 1;
        return ret;
    }
    T shift() {
        ASSERT(_size > 0);
        T ret(std::move(_data[0]));
        _data[0].~T();
        _size -= 1;
        for (int64_t i = 0; i < _size; i++)
            new (_data + i) T(std::move(_data[i + 1]));
        return ret;
    }
    ext::array<T>& reserve(int64_t capacity) {
        if (capacity > _capacity) {
            T* data = ext::alloc<T>(capacity);
            for (int64_t i = 0; i < _size; i++)
                new (data + i) T(std::move(_data[i]));
            SAFE_FREE(_data);
            _data = data;
            _capacity = capacity;
        }
        return *this;
    }
    // include <libext/cxx/string.h> to activate these methods...
    ext::string join() const;
    ext::string join(char const* separator) const;
    ext::string join(ext::string const& separator) const;
    static ext::array<T> with_data(T* data, int64_t size) {
        ext::array<T> ret;
        ret._data = data;
        ret._size =
        ret._capacity = size;
        return ret;
    }
    static ext::array<T> with_data(T* data, int64_t size, int64_t capacity) {
        ext::array<T> ret;
        ret._data = data;
        ret._size = size;
        ret._capacity = capacity;
        return ret;
    }
    static ext::array<T> with_capacity(int64_t capacity) {
        ext::array<T> ret;
        ret.reserve(capacity);
        return ret;
    }
};

} /* namespace ext */