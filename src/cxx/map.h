#pragma once

namespace ext {

template<typename K, typename V>
class pair {
private:
    K _key;
    V _value;
public:
    pair()
        : _key()
        , _value() {}
    pair(K const& key, V const& value)
        : _key(key)
        , _value(value) {}
    pair(K const& key, V && value)
        : _key(key)
        , _value(std::move(value)) {}
    pair(K && key, V const& value)
        : _key(std::move(key))
        , _value(value) {}
    pair(K && key, V && value)
        : _key(std::move(key))
        , _value(std::move(value)) {}
    pair(ext::pair<K, V> const& pair)
        : _key(pair._key)
        , _value(pair._value) {}
    pair(ext::pair<K, V> && pair)
        : _key(std::move(pair._key))
        , _value(std::move(pair._value)) {}
    ext::pair<K, V>& operator=(ext::pair<K, V> const& pair) {
        _key = pair._key;
        _value = pair._value;
        return *this;
    }
    ext::pair<K, V>& operator=(ext::pair<K, V> && pair) {
        _key = std::move(pair._key);
        _value = std::move(pair._value);
        return *this;
    }
    bool operator==(ext::pair<K, V> const& pair) const {
        return (_key == pair._key)
            && (_value == pair._value);
    }
    bool operator!=(ext::pair<K, V> const& pair) const {
        return !operator==(pair);
    }
    K const& key() const {
        return _key;
    }
    V const& value() const {
        return _value;
    }
    K& key() {
        return _key;
    }
    V& value() {
        return _value;
    }
};

template<typename K, typename V>
class map {
private:
    ext::array<ext::pair<K, V>> _data;
    int _find_index(K const& key) const {
        for (int i = 0; i < _data.size(); i++)
            if (_data[i].key() == key)
                return i;
        return -1;
    }
public:
    map()
        : _data() {}
    map(ext::map<K, V> const& map)
        : _data(map._data) {}
    map(ext::map<K, V> && map)
        : _data(std::move(map._data)) {}
    ext::map<K, V>& operator=(ext::map<K, V> const& map) {
        if (this != &map)
            _data = map._data;
        return *this;
    }
    ext::map<K, V>& operator=(ext::map<K, V> && map) {
        if (this != &map)
            _data = std::move(map._data);
        return *this;
    }
    V const& operator[](K const& key) const {
        return get(key);
    }
    V& operator[](K const& key) {
        return get(key);
    }
    bool operator==(ext::map<K, V> const& map) const {
        return (_data == map._data);
    }
    bool operator!=(ext::map<K, V> const& map) const {
        return !operator==(map);
    }
    ext::array<ext::pair<K, V>> const& data() const {
        return _data;
    }
    ext::array<ext::pair<K, V>>& data() {
        return _data;
    }
    bool has(K const& key) const {
        for (ext::pair<K, V> const& pair : _data)
            if (pair.key() == key)
                return true;
        return false;
    }
    V const& get(K const& key) const {
        int index = _find_index(key);
        ASSERT(index > -1);
        return _data[index].value();
    }
    V& get(K const& key) {
        int index = _find_index(key);
        ASSERT(index > -1);
        return _data[index].value();
    }
    ext::map<K, V>& set(K const& key, V const& value) {
        int index = _find_index(key);
        if (index > -1) {
            _data[index].value() = value;
        } else {
            _data.push({ key, value });
        }
        return *this;
    }
    ext::map<K, V>& set(K const& key, V && value) {
        int index = _find_index(key);
        if (index > -1) {
            _data[index].value() = std::move(value);
        } else {
            _data.push({ key, std::move(value) });
        }
        return *this;
    }
};

} /* namespace ext */