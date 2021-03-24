#pragma once

#include <libcore/cxx/base.h>

namespace ext {

struct nullopt_t {};
constexpr nullopt_t nullopt = {};

template<typename T>
class optional {
private:
	T* _value = nullptr;
public:
	~optional() {
		SAFE_DELETE(_value);
	}
	optional()
		: _value(nullptr) {}
	optional(T const& value)
		: _value(new T(value)) {}
	optional(T && value)
		: _value(new T(std::move(value))) {}
	optional(nullopt_t const& value)
		: _value(nullptr) {}
	optional(ext::optional<T> const& optional) {
		if (optional.has_value())
			_value = new T(optional.value());
	}
	optional(ext::optional<T> && optional) {
		_value = optional._value;
		optional._value = nullptr;
	}
	ext::optional<T>& operator=(ext::optional<T> const& optional) {
		if (this != &optional) {
			SAFE_DELETE(_value);
			if (optional.has_value())
				_value = new T(optional.value());
		}
		return *this;
	}
	ext::optional<T>& operator=(ext::optional<T> && optional) {
		if (this != &optional) {
			T* t = _value;
			_value = optional._value;
			optional._value = t;
		}
		return *this;
	}
	ext::optional<T>& operator=(T const& value) {
		SAFE_DELETE(_value);
		_value = new T(value);
		return *this;
	}
	ext::optional<T>& operator=(T && value) {
		SAFE_DELETE(_value);
		_value = new T(std::move(value));
		return *this;
	}
	ext::optional<T>& operator=(nullopt_t const& value) {
		SAFE_DELETE(_value);
		return *this;
	}
	bool operator==(ext::optional<T> const& optional) const {
		if (!this->has_value() && !optional.has_value()) return true;
		if (!this->has_value() && optional.has_value()) return false;
		if (this->has_value() && !optional.has_value()) return false;
		return this->value() == optional.value();
	}
	bool operator!=(ext::optional<T> const& optional) const {
		return !this->operator==(optional);
	}
	bool operator==(T const& value) const {
		if (!this->has_value()) return false;
		return this->value() == value;
	}
	bool operator!=(T const& value) const {
		return !this->operator==(value);
	}
	bool operator==(nullopt_t const& value) const {
		return !this->has_value();
	}
	bool operator!=(nullopt_t const& value) const {
		return !this->operator==(value);
	}
	bool has_value() const {
		return (_value != nullptr);
	}
	T& value() {
		ASSERT(_value != nullptr);
		return *_value;
	}
	T const& value() const {
		ASSERT(_value != nullptr);
		return *_value;
	}
	T const& value_or(T const& a) const {
		return (_value != nullptr) ? *_value : a;
	}
};

} /* namespace ext */