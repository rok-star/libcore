#pragma once

#include <libext/MACRO.h>
#include <libext/window.h>
#include <libext/cxx/math.h>
#include <libext/cxx/string.h>
#include <libext/cxx/optional.h>
#include <libext/cxx/event.h>

namespace ext {

class window;

class mouse_info {
	friend ext::window;
private:
	ext::point<int> _position;
	mouse_info(ext::point<int> const& position)
		: _position(position) {}
	mouse_info(_window_event_t const* event)
		: _position({ static_cast<int>(event->mouse_info.position.x)
					, static_cast<int>(event->mouse_info.position.y) }) {}
public:
	ext::point<int> const& position() const {
		return _position;
	}
};

class key_info {
	friend ext::window;
private:
	_KEY _key;
	bool _shift;
    bool _control;
    bool _option;
    bool _super;
	key_info(_KEY key, bool shift, bool control, bool option, bool super)
		: _key(key)
		, _shift(shift)
		, _control(control)
		, _option(option)
		, _super(super) {}
	key_info(_window_event_t const* event)
		: _key(event->key_info.key)
		, _shift(event->key_info.shift)
		, _control(event->key_info.control)
		, _option(event->key_info.option)
		, _super(event->key_info.super) {}
public:
	_KEY key() const {
		return _key;
	}
	bool shift() const {
		return _shift;
	}
	bool control() const {
		return _control;
	}
	bool option() const {
		return _option;
	}
	bool super() const {
		return _super;
	}
};

class window {
private:
	_window_t* _window;
	ext::string __text;
	ext::size<int> __size;
	ext::point<int> __origin;
	static void _window_event(_window_event_t const* event, void* param) {
		ASSERT(event != nullptr);
		ASSERT(param != nullptr);
		#define __TRIGGER_SIMPLE(a) { \
			static_cast<ext::window*>(param)->_on_##a(); \
			static_cast<ext::window*>(param)->on_##a.trigger(); \
		}
		#define __TRIGGER_MOUSE(a) { \
			ext::mouse_info info(event); \
			static_cast<ext::window*>(param)->_on_##a(info); \
			static_cast<ext::window*>(param)->on_##a.trigger(info); \
		}
		#define __TRIGGER_KEY(a) { \
			ext::key_info info(event); \
			static_cast<ext::window*>(param)->_on_##a(info); \
			static_cast<ext::window*>(param)->on_##a.trigger(info); \
		}
		if (event->type == _SHOW_WINDOW_EVENT) __TRIGGER_SIMPLE(show)
		else if (event->type == _HIDE_WINDOW_EVENT) __TRIGGER_SIMPLE(hide)
	    else if (event->type == _SIZE_WINDOW_EVENT) __TRIGGER_SIMPLE(size)
	    else if (event->type == _CLOSE_WINDOW_EVENT) __TRIGGER_SIMPLE(close)
	    else if (event->type == _MAXIMIZE_WINDOW_EVENT) __TRIGGER_SIMPLE(maximize)
	    else if (event->type == _MINIMIZE_WINDOW_EVENT) __TRIGGER_SIMPLE(minimize)
	    else if (event->type == _DEMAXIMIZE_WINDOW_EVENT) __TRIGGER_SIMPLE(demaximize)
	    else if (event->type == _DEMINIMIZE_WINDOW_EVENT) __TRIGGER_SIMPLE(deminimize)
	    else if (event->type == _PIXELRATIO_WINDOW_EVENT) __TRIGGER_SIMPLE(pixelratio)
	    else if (event->type == _MOUSEMOVE_WINDOW_EVENT) __TRIGGER_MOUSE(mousemove)
	    else if (event->type == _LBUTTONDOWN_WINDOW_EVENT) __TRIGGER_MOUSE(lbuttondown)
	    else if (event->type == _LBUTTONUP_WINDOW_EVENT) __TRIGGER_MOUSE(lbuttonup)
	    else if (event->type == _RBUTTONDOWN_WINDOW_EVENT) __TRIGGER_MOUSE(rbuttondown)
	    else if (event->type == _RBUTTONUP_WINDOW_EVENT) __TRIGGER_MOUSE(rbuttonup)
	    else if (event->type == _KEYDOWN_WINDOW_EVENT) __TRIGGER_KEY(keydown)
	    else if (event->type == _KEYUP_WINDOW_EVENT) __TRIGGER_KEY(keyup)
	    #undef __TRIGGER_SIMPLE
	    #undef __TRIGGER_MOUSE
	    #undef __TRIGGER_KEY
	}
protected:
	virtual void _on_show() {}
	virtual void _on_hide() {}
	virtual void _on_maximize() {}
	virtual void _on_minimize() {}
	virtual void _on_demaximize() {}
	virtual void _on_deminimize() {}
	virtual void _on_close() {}
	virtual void _on_size() {}
	virtual void _on_pixelratio() {}
	virtual void _on_mousemove(ext::mouse_info const& info) {}
	virtual void _on_lbuttondown(ext::mouse_info const& info) {}
	virtual void _on_lbuttonup(ext::mouse_info const& info) {}
	virtual void _on_rbuttondown(ext::mouse_info const& info) {}
	virtual void _on_rbuttonup(ext::mouse_info const& info) {}
	virtual void _on_keydown(ext::key_info const& info) {}
	virtual void _on_keyup(ext::key_info const& info) {}
public:
	ext::event_source<> on_show;
	ext::event_source<> on_hide;
	ext::event_source<> on_maximize;
	ext::event_source<> on_minimize;
	ext::event_source<> on_demaximize;
	ext::event_source<> on_deminimize;
	ext::event_source<> on_close;
	ext::event_source<> on_size;
	ext::event_source<> on_pixelratio;
	ext::event_source<ext::mouse_info const&> on_mousemove;
	ext::event_source<ext::mouse_info const&> on_lbuttondown;
	ext::event_source<ext::mouse_info const&> on_lbuttonup;
	ext::event_source<ext::mouse_info const&> on_rbuttondown;
	ext::event_source<ext::mouse_info const&> on_rbuttonup;
	ext::event_source<ext::key_info const&> on_keydown;
	ext::event_source<ext::key_info const&> on_keyup;
	window(ext::window const& window) = delete;
	window(ext::window && window) = delete;
	ext::window& operator=(ext::window const& window) = delete;
	ext::window& operator=(ext::window && window) = delete;
	virtual ~window() {
		_window_destroy(_window);
	}
	window() {
		_window = _window_create();
	}
	ext::string const& text() const {
		const_cast<ext::window*>(this)->__text = ext::string(_window_text(_window));
		return __text;
	}
	ext::size<int> const& size() const {
		_size_t const* size_ = _window_size(_window);
		const_cast<ext::window*>(this)->__size = {
			static_cast<int>(size_->width),
			static_cast<int>(size_->height)
		};
		return __size;
	}
	ext::point<int> const& origin() const {
		_point_t const* point_ = _window_origin(_window);
		const_cast<ext::window*>(this)->__origin = {
			static_cast<int>(point_->x),
			static_cast<int>(point_->y)
		};
		return __origin;
	}
	bool visible() const {
		return _window_visible(_window);
	}
	bool sizable() const {
		return _window_sizable(_window);
	}
	bool closable() const {
		return _window_closable(_window);
	}
	bool maximizable() const {
		return _window_maximizable(_window);
	}
	bool minimizable() const {
		return _window_minimizable(_window);
	}
	bool maximized() const {
		return _window_maximized(_window);
	}
	bool minimized() const {
		return _window_minimized(_window);
	}
	ext::window& set_text(ext::string const& text) {
		__text = text;
		_window_set_text(_window, __text.c_string());
		return *this;
	}
	ext::window& set_text(char const* pchar) {
		__text = pchar;
		_window_set_text(_window, __text.c_string());
		return *this;
	}
	ext::window& set_size(ext::size<int> const& size) {
		_size_t size_ = size.size_t();
		_window_set_size(_window, &size_);
		return *this;
	}
	ext::window& set_origin(ext::point<int> const& origin) {
		_point_t origin_ = origin.point_t();
		_window_set_origin(_window, &origin_);
		return *this;
	}
	ext::window& set_visible(bool value) {
		_window_set_visible(_window, value);
		return *this;
	}
	ext::window& set_sizable(bool value) {
		_window_set_sizable(_window, value);
		return *this;
	}
	ext::window& set_closable(bool value) {
		_window_set_closable(_window, value);
		return *this;
	}
	ext::window& set_maximizable(bool value) {
		_window_set_maximizable(_window, value);
		return *this;
	}
	ext::window& set_minimizable(bool value) {
		_window_set_minimizable(_window, value);
		return *this;
	}
	ext::window& set_maximized(bool value) {
		_window_set_maximized(_window, value);
		return *this;
	}
	ext::window& set_minimized(bool value) {
		_window_set_minimized(_window, value);
		return *this;
	}
};

} /* namespace ext */