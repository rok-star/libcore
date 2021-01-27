#pragma once

#include <libext/cxx/base.h>

namespace ext {

template<typename T>
class ref final {
	struct payload {
		T* data;
		int refs;
	};
private:
	payload* _payload;
public:
	~ref() {
		if (_payload != nullptr) {
			ASSERT(_payload->data != nullptr);
			ASSERT(_payload->refs >= 1);
			if (_payload->refs <= 1) {
				SAFE_DELETE(_payload->data);
				SAFE_DELETE(_payload);
			}
		}
	}
	ref() : _payload(nullptr) {}
	ref(T* data) {
		_payload = new payload{
			.data = data,
			.refs = 1
		};
	}
	ref(ext::ref<T> const& ref) {

	}
	ref(ext::ref<T> && ref) {

	}
	ext::ref<T>& operator=(ext::ref<T> const& ref) {
		if ((_payload == nullptr) && (ref._payload != nullptr)) {
			ASSERT(ref._payload->data != nullptr);
			ASSERT(ref._payload->refs >= 1);
			_payload = ref._payload;
			_payload->refs += 1;
		} else if ((_payload != nullptr) && (ref._payload == nullptr)) {
			if (_payload != nullptr) {
				ASSERT(_payload->data != nullptr);
				ASSERT(_payload->refs >= 1);
				if (_payload->refs <= 1) {
					SAFE_DELETE(_payload->data);
					SAFE_DELETE(_payload);
					_payload = nullptr;
				} else {
					_payload->refs -= 1;
					_payload = nullptr;
				}
			}
		} else if ((_payload != nullptr) && (ref._payload != nullptr)) {
			ASSERT(_payload->data != nullptr);
			ASSERT(_payload->refs >= 1);
			ASSERT(ref._payload->data != nullptr);
			ASSERT(ref._payload->refs >= 1);
			if (_payload != ref._payload) {
				ASSERT(_payload->data != nullptr);
				ASSERT(_payload->refs >= 1);
				if (_payload->refs <= 1) {
					SAFE_DELETE(_payload->data);
					SAFE_DELETE(_payload);
					_payload = nullptr;
				} else {
					_payload->refs -= 1;
					_payload = nullptr;
				}
				_payload = ref._payload;
				_payload->refs += 1;
			} else {
				ASSERT(_payload->data == ref._payload->data);
			}
		}
		return *this;
	}
	T* operator->() {
		ASSERT(_payload != nullptr);
		return _payload->data;
	}
	T const* operator->() const {
		ASSERT(_payload != nullptr);
		return _payload->data;
	}
	T& value() {
		ASSERT(_payload != nullptr);
		return *(_payload->data);
	}
	T const& value() const {
		ASSERT(_payload != nullptr);
		return *(_payload->data);
	}
	bool has_value() const {
		return _payload != nullptr;
	}
};

template<typename T, typename... A>
inline ext::ref<T> make(A... args) {
	return ext::ref<T>(new T(std::forward<A>(args)...));
}

} /* namespace ext */