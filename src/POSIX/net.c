#ifdef __linux__
    #define _POSIX_C_SOURCE 200809L
	#include <sys/epoll.h>
#endif

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <libext/net.h>
#include <libext/MACRO.h>

#define __POLL_COUNT 255
#define __CHUNK_SIZE 1024
#define __CLEAR_EVENTS(array) { \
	for (int64_t i = 0; i < array.size; i++) { \
		_FREE(array.data[i].message); \
	} \
	array.size = 0; \
}

typedef struct sockaddr_in sockaddr_in_t;
typedef struct sockaddr sockaddr_t;
typedef struct timeval timeval_t;
typedef struct epoll_event epoll_event_t;

typedef struct _listener_t {
	sockaddr_in_t addr;
	int socket;
	struct {
		_listener_event_t* data;
		int64_t size;
		int64_t capacity;
	} events;
} _listener_t;

typedef struct _connection_t {
	int socket;
	struct {
		_connection_event_t* data;
		int64_t size;
		int64_t capacity;
	} events;
} _connection_t;

typedef struct _connection_poll_t {
	struct {
		_connection_event_t* data;
		int64_t size;
		int64_t capacity;
	} events;
#ifdef __linux__
	int pollfd;
#else
#endif
} _connection_poll_t;

_listener_t* _listener_create(_listener_info_t* info, _status_t* status) {
	_ASSERT(info != NULL);
	_ASSERT(status != NULL);
	_ASSERT(info->port > 0);

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock >= 0) {
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) == 0) {
			sockaddr_in_t addr = {
				.sin_family = AF_INET,
    			.sin_addr.s_addr = INADDR_ANY,
    			.sin_port = htons(info->port)
			};
			if (bind(sock, (sockaddr_t*)&addr, sizeof(sockaddr_in_t)) == 0) {
				if (listen(sock, 3) >= 0) {
					_status_set(status, _SUCCESS_STATUS_TYPE, NULL);
					return _NEW(_listener_t, {
						.addr = addr,
						.socket = sock
					});
				} else {
					close(sock);
					_STATUS_SET_FORMAT(status, _FAILURE_STATUS_TYPE, "listen() failed: %s", strerror(errno));
					return NULL;
				}
			} else {
				close(sock);
				_STATUS_SET_FORMAT(status, _FAILURE_STATUS_TYPE, "bind() failed: %s", strerror(errno));
				return NULL;
			}
		} else {
			close(sock);
			_STATUS_SET_FORMAT(status, _FAILURE_STATUS_TYPE, "setsockopt() failed: %s", strerror(errno));
			return NULL;
		}
	} else {
		_STATUS_SET_FORMAT(status, _FAILURE_STATUS_TYPE, "socket() failed: %s", strerror(errno));
		return NULL;
	}
}

void _listener_destroy(_listener_t* listener) {
	_ASSERT(listener != NULL);

	close(listener->socket);
	__CLEAR_EVENTS(listener->events);

	_FREE(listener->events.data);
	_FREE(listener);
}

void _listener_process(_listener_t* listener, _listener_event_t const** events, int64_t* num) {
	_ASSERT(listener != NULL);
	_ASSERT(events != NULL);
	_ASSERT(num != NULL);

	(*events) = NULL;
	(*num) = 0;

	__CLEAR_EVENTS(listener->events);

	fd_set fd_in = {};
	FD_SET(listener->socket, &fd_in);

	int ret = select((listener->socket + 1), &fd_in, NULL, NULL, &(timeval_t){ .tv_usec = 1 });
	if (ret > 0) {
		if (FD_ISSET(listener->socket, &fd_in)) {
			_PUSH(listener->events, (_listener_event_t){
				.type = _ACCEPT_LISTENER_EVENT
			});
		}
	} else if (ret < 0) {
		if (errno == EINTR) {
			_PUSH(listener->events, (_listener_event_t){
				.type = _INTERUPT_LISTENER_EVENT
			});
		} else {
			_PUSH(listener->events, (_listener_event_t){
				.type = _ERROR_LISTENER_EVENT,
				.message = _FORMAT("select() failed: %s", strerror(errno))
			});
		}
	}

	(*events) = listener->events.data;
	(*num) = listener->events.size;
}

_connection_t* _connection_accept(_listener_t* listener, _status_t* status) {
	_ASSERT(listener != NULL);
	_ASSERT(status != NULL);

	int sock = accept(listener->socket, (sockaddr_t*)&listener->addr, &(socklen_t){ sizeof(sockaddr_in_t) });
	if (sock > 0) {
		int flags = fcntl(sock, F_GETFL, 0);
		if (flags >= 0) {
			if (fcntl(sock, F_SETFL, (flags | O_NONBLOCK)) == 0) {
				_status_set(status, _SUCCESS_STATUS_TYPE, NULL);
				return _NEW(_connection_t, { .socket = sock });
			} else {
				close(sock);
				_STATUS_SET_FORMAT(status, _FAILURE_STATUS_TYPE, "fcntl() failed: %s", strerror(errno));
			}
		} else {
			close(sock);
			_STATUS_SET_FORMAT(status, _FAILURE_STATUS_TYPE, "fcntl() failed: %s", strerror(errno));
		}
	} else if (sock < 0) {
		_STATUS_SET_FORMAT(status, _FAILURE_STATUS_TYPE, "accept() failed: %s", strerror(errno));
	}

	return NULL;
}

void _connection_destroy(_connection_t* connection) {
	_ASSERT(connection != NULL);

	close(connection->socket);
	__CLEAR_EVENTS(connection->events);

	_FREE(connection->events.data);
	_FREE(connection);
}

void _connection_process(_connection_t* connection, _connection_event_t const** events, int64_t* num) {
	_ASSERT(connection != NULL);
	_ASSERT(events != NULL);
	_ASSERT(num != NULL);

	(*events) = NULL;
	(*num) = 0;

	__CLEAR_EVENTS(connection->events);

	fd_set fd_in = {};
	FD_SET(connection->socket, &fd_in);

	int ret = select((connection->socket + 1), &fd_in, NULL, NULL, &(timeval_t){ .tv_usec = 1 });
	if (ret > 0) {
		if (FD_ISSET(connection->socket, &fd_in)) {
			_PUSH(connection->events, (_connection_event_t){
				.type = _READ_CONNECTION_EVENT
			});
		}
	} else if (ret < 0) {
		if (errno == EINTR) {
			_PUSH(connection->events, (_connection_event_t){
				.type = _INTERUPT_CONNECTION_EVENT
			});
		} else {
			_PUSH(connection->events, (_connection_event_t){
				.type = _ERROR_CONNECTION_EVENT,
				.message = _FORMAT("select() failed: %s", strerror(errno))
			});
		}
	}

	(*events) = connection->events.data;
	(*num) = connection->events.size;
}

void _connection_write(_connection_t* connection, uint8_t* data, int64_t size) {
	_ASSERT(connection != NULL);
	_ASSERT(data != NULL);

	write(connection->socket, data, size);
}

void _connection_read(_connection_t* connection, uint8_t** data, int64_t* size) {
	_ASSERT(connection != NULL);
	_ASSERT(data != NULL);
	_ASSERT(size != NULL);

	(*data) = NULL;
	(*size) = 0;

	typedef struct __chunk_t {
		uint8_t* data;
		int64_t size;
	} __chunk_t;

	typedef struct __chunk_array_t {
		__chunk_t* data;
		int64_t size;
		int64_t capacity;
	} __chunk_array_t;

	__chunk_array_t chunks = {};

	for (;;) {
		uint8_t tmp[__CHUNK_SIZE];
		int64_t len = recv(connection->socket, tmp, __CHUNK_SIZE, 0);

		if (len > 0) {
			__chunk_t chunk = {
				.data = _ALLOC(uint8_t, len),
				.size = len
			};
			memcpy(chunk.data, tmp, len);
			_PUSH(chunks, chunk);
			(*size) += len;
		} else {
			break;
		}
	}

	if (chunks.size == 1) {
		(*data) = chunks.data[0].data;
	} else if (chunks.size > 1) {
		(*data) = _ALLOC(uint8_t, (*size));
		uint8_t* dst = (*data);
		for (int64_t i = 0; i < chunks.size; i++) {
			memcpy(dst, chunks.data[i].data, chunks.data[i].size);
			_FREE(chunks.data[i].data);
			dst += chunks.data[i].size;
		}
	}

	_FREE(chunks.data);
}

_connection_poll_t* _connection_poll_create(_status_t* status) {
	_ASSERT(status != NULL);

#ifdef __linux__
	int pollfd = epoll_create(1);
	if (pollfd >= 0) {
		_status_set(status, _SUCCESS_STATUS_TYPE, NULL);
		return _NEW(_connection_poll_t, { .pollfd = pollfd });
	} else {
		_STATUS_SET_FORMAT(status, _FAILURE_STATUS_TYPE, "epoll_create() failed: %s", strerror(errno));
		return NULL;
	}
#else
	_status_set(status, _SUCCESS_STATUS_TYPE, NULL);
	return _NEW(_connection_poll_t, {});
#endif
}

void _connection_poll_destroy(_connection_poll_t* poll) {
	_ASSERT(poll != NULL);

#ifdef __linux__
	close(poll->pollfd);
#else
#endif

	__CLEAR_EVENTS(poll->events);

	_FREE(poll->events.data);
	_FREE(poll);
}

void _connection_poll_process(_connection_poll_t* poll, _connection_event_t const** events, int64_t* num) {
	_ASSERT(poll != NULL);
	_ASSERT(events != NULL);
	_ASSERT(num != NULL);

	(*events) = NULL;
	(*num) = 0;

	__CLEAR_EVENTS(poll->events);

#ifdef __linux__
	epoll_event_t ev[__POLL_COUNT];
	int ret = epoll_wait(poll->pollfd, ev, __POLL_COUNT, 1);
	if (ret >= 0) {
		for (int i = 0; i < ret; i++) {
			if (ev[i].events & EPOLLIN) {
				_PUSH(poll->events, (_connection_event_t){
					.type = _READ_CONNECTION_EVENT,
	            	.connection = (_connection_t*)ev[i].data.ptr
				});
			}
		}
	} else {
		if (errno == EINTR) {
			_PUSH(poll->events, (_connection_event_t){
				.type = _INTERUPT_CONNECTION_EVENT
			});
		} else {
			_PUSH(poll->events, (_connection_event_t){
				.type = _ERROR_CONNECTION_EVENT,
				.message = _FORMAT("epoll_wait() failed: %s\n", strerror(errno))
			});
		}
	}
#else
#endif

	(*events) = poll->events.data;
	(*num) = poll->events.size;
}

void _connection_poll_append(_connection_poll_t* poll, _connection_t* connection) {
	_ASSERT(poll != NULL);
	_ASSERT(connection != NULL);

#ifdef __linux__
	_CALL(
		epoll_ctl,
		poll->pollfd,
		EPOLL_CTL_ADD,
		connection->socket,
		&(epoll_event_t){
			.data.ptr = connection,
			.events = (EPOLLIN | EPOLLONESHOT)
		}
	);
#else
#endif
}

void _connection_poll_remove(_connection_poll_t* poll, _connection_t* connection) {
	_ASSERT(poll != NULL);
	_ASSERT(connection != NULL);

#ifdef __linux__
	_CALL(
		epoll_ctl,
		poll->pollfd,
		EPOLL_CTL_DEL,
		connection->socket,
		NULL
	);
#else
#endif
}
