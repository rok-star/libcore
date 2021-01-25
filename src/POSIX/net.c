#include <sys/select.h>

#include <libcore/net.h>
#include <libcore/MACRO.h>

typedef struct sockaddr_in sockaddr_in_t;
typedef struct sockaddr sockaddr_t;
typedef struct timeval timeval_t;

typedef struct _listener_t {
	sockaddr_in_t addr;
	int fd;
} _listener_t;

_listener_t* _listener_create(_listener_info_t const* info, _status_t* status) {
	_ASSERT(info != NULL);
	_ASSERT(status != NULL);
	_ASSERT(info->port > 0);

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd != 0) {
		if (setsockopt(fd, SOL_SOCKET, (SO_REUSEADDR | SO_REUSEPORT), &(int){ 1 }, sizeof(int)) == 0) {
			sockaddr_in_t addr = {
				.sin_family = AF_INET,
    			.sin_addr.s_addr = INADDR_ANY,
    			.sin_port = htons(info->port)
			};
			if (bind(fd, (sockaddr_t*)&addr, sizeof(sockaddr_in_t)) >= 0) {
				if (listen(fd, 3) >= 0) {
					_status_set(status, _SUCCESS_STATUS_TYPE, NULL);
					return _NEW(_listener_t, {
						.addr = addr,
						.fd = fd
					});
				} else {
					close(fd);
					_status_set(status, _FAILURE_STATUS_TYPE, strerror(errno));
					return NULL;
				}
			} else {
				close(fd);
				_status_set(status, _FAILURE_STATUS_TYPE, strerror(errno));
				return NULL;
			}
		} else {
			close(fd);
			_status_set(status, _FAILURE_STATUS_TYPE, strerror(errno));
			return NULL;
		}
	} else {
		_status_set(status, _FAILURE_STATUS_TYPE, strerror(errno));
		return NULL;
	}
}

void _listener_destroy(_listener_t* listener) {
	_ASSERT(listener != NULL);
	close(listener->fd);
	_FREE(listener);
}

void _listener_process(_listener_t* listener) {
	_ASSERT(listener != NULL);

	fd_set fd_int = {};

	FD_SET(listener->fd, &fd_in);

	if (select((listener->fd + 1), &fd_in, NULL, NULL, &(timeval_t){ .tv_usec = 1 }) > 0) {
		if (FD_ISSET(listener->fd, &fd_in)) {
			int fd = accept(listener->fd, (sockaddr_t*)&listener->addr, (socklen_t*)&{ sizeof(sockaddr_in_t) });
			if (fd >= 0) {
				;
			} else {
				;
			}
		}
	}
}

void _listener_on_event(_listener_t* listener, void(*proc)(_listener_event_t const*,void*), void* param) {
	_ASSERT(listener != NULL);
}