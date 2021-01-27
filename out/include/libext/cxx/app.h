#pragma once

#include <libext/app.h>

namespace ext {

class app {
private:
	static ext::app* _current;
	static void _app_event(_app_event_t const* event, void* param) {
		ASSERT(event != nullptr);
		ASSERT(param != nullptr);
		if (event->type == _RUN_APP_EVENT) {
			(static_cast<ext::app*>(param))->_on_run();
		} else if (event->type == _EXIT_APP_EVENT) {
			(static_cast<ext::app*>(param))->_on_exit();
		} else if (event->type == _SPIN_APP_EVENT) {
			(static_cast<ext::app*>(param))->_on_spin();
		}
	}
protected:
	virtual void _on_run() {}
	virtual void _on_exit() {}
	virtual void _on_spin() {}
public:
	ext::event_source<> on_run;
	ext::event_source<> on_exit;
	ext::event_source<> on_spin;
	virtual ~app() {}
	app() {}
	app(ext::app const& app) = delete;
	app(ext::app && app) = delete;
	ext::app& operator=(ext::app const& app) = delete;
	ext::app& operator=(ext::app && app) = delete;
	void run() {
		ASSERT(_current == nullptr);
		_current = this;
		_app_on_event(_app_event, this);
		_app_run();
	}
	void exit() {
		ASSERT(_current == this);
		_app_exit();
		_app_on_event(nullptr, nullptr);
		_current = nullptr;
	}
	void wakeup() {
		ASSERT(_current == this);
		_app_wakeup();
	}
	bool running() {
		ASSERT(_current == this);
		return _app_running();
	}
	static ext::app& current() {
		return *_current;
	}
};

inline ext::app* ext::app::_current = nullptr;

} /* namespace ext */