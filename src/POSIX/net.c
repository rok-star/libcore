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

typedef struct sockaddr_in sockaddr_in_t;
typedef struct sockaddr sockaddr_t;
typedef struct timeval timeval_t;
typedef struct epoll_event epoll_event_t;

typedef struct _listener_t {
	sockaddr_in_t addr;
	int socket;
	void (*proc)(_listener_event_t const*,void*);
	void* param;
} _listener_t;

typedef struct _connection_t {
	int socket;
	void* userdata;
	void (*proc)(_connection_event_t const*,void*);
	void* param;
} _connection_t;

typedef struct _connection_poll_t {
	struct {
		_connection_t** data;
		int64_t size;
		int64_t capacity;
	} connections;
#ifdef __linux__
	int pollfd;
#else
#endif
} _connection_poll_t;

_listener_t* _listener_create(_listener_info_t const* info, _status_t* status) {
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
					_status_set(status, _FAILURE_STATUS_TYPE, strerror(errno));
					return NULL;
				}
			} else {
				close(sock);
				_status_set(status, _FAILURE_STATUS_TYPE, strerror(errno));
				return NULL;
			}
		} else {
			close(sock);
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

	close(listener->socket);
	_FREE(listener);
}

void _listener_process(_listener_t* listener) {
	_ASSERT(listener != NULL);

	fd_set fd_in = {};
	FD_SET(listener->socket, &fd_in);

	if (select((listener->socket + 1), &fd_in, NULL, NULL, &(timeval_t){ .tv_usec = 1 }) > 0) {
		if (FD_ISSET(listener->socket, &fd_in)) {
			if (listener->proc != NULL) {
				listener->proc(
					&(_listener_event_t){
						.type = _ACCEPT_LISTENER_EVENT
					},
					listener->param
				);
			}
		}
	}
}

void _listener_on_event(_listener_t* listener, void(*proc)(_listener_event_t const*,void*), void* param) {
	_ASSERT(listener != NULL);

	listener->proc = proc;
	listener->param = param;
}

_connection_t* _connection_accept(_listener_t* listener, _status_t* status) {
	_ASSERT(listener != NULL);
	_ASSERT(status != NULL);

	int sock = accept(listener->socket, (sockaddr_t*)&listener->addr, &(socklen_t){ sizeof(sockaddr_in_t) });
	if (sock >= 0) {
		int flags = fcntl(sock, F_GETFL, 0);
		_ASSERT(flags >= 0);
		_CALL(fcntl, sock, F_SETFL, (flags | O_NONBLOCK));
		_status_set(status, _SUCCESS_STATUS_TYPE, NULL);
		return _NEW(_connection_t, { .socket = sock });
	} else {
		_status_set(status, _FAILURE_STATUS_TYPE, strerror(errno));
		return NULL;
	}
}

void _connection_process(_connection_t* connection) {
	_ASSERT(connection != NULL);

	fd_set fd_in = {};
	FD_SET(connection->socket, &fd_in);

	if (select((connection->socket + 1), &fd_in, NULL, NULL, &(timeval_t){ .tv_usec = 1 }) > 0) {
		if (FD_ISSET(connection->socket, &fd_in)) {
			if (connection->proc != NULL) {
				connection->proc(
					&(_connection_event_t){
						.type = _READ_CONNECTION_EVENT
					},
					connection->param
				);
			}
		}
	}
}

void _connection_destroy(_connection_t* connection) {
	_ASSERT(connection != NULL);

	close(connection->socket);
	_FREE(connection);
}

int _connection_read(_connection_t* connection, uint8_t* data, int64_t size) {
	_ASSERT(connection != NULL);
	_ASSERT(data != NULL);

	int ret = 0;
	for (;;) {
		int read = recv(connection->socket, data, size, 0);
		if (read > 0) {
			data += read;
			size -= read;
			ret += read;
			if (size == 0) {
				break;
			}
		} else {
			break;
		}
	}
	return ret;
}

void _connection_read_all(_connection_t* connection, uint8_t** data, int64_t* size) {
	_ASSERT(connection != NULL);
	_ASSERT(data != NULL);
	_ASSERT(size != NULL);

	(*data) = NULL;
	(*size) = 0;

	#define __CHUNK_SIZE 1024

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
		__chunk_t chunk = {
			.data = _NEW(uint8_t, __CHUNK_SIZE),
			.size = 0
		};

		chunk.size = recv(connection->socket, chunk.data, __CHUNK_SIZE, 0);

		if (chunk.size > 0) {
			_PUSH(chunks, chunk);
			(*size) += chunk.size;
		} else {
			_FREE(chunk.data);
			break;
		}
	}

	(*data) = _NEW(uint8_t, (*size));

	uint8_t* dst = (*data);
	for (int64_t i = 0; i < chunks.size; i++) {
		memcpy(dst, chunks.data[i].data, chunks.data[i].size);
		_FREE(chunks.data[i].data);
		dst += chunks.data[i].size;
	}

	_FREE(chunks.data);

	#undef __CHUNK_SIZE
}

void _connection_write(_connection_t* connection, uint8_t* data, int64_t size) {
	_ASSERT(connection != NULL);
	_ASSERT(data != NULL);

	write(connection->socket, data, size);
}

void _connection_on_event(_connection_t* connection, void(*proc)(_connection_event_t const*,void*), void* param) {
	_ASSERT(connection != NULL);
	connection->proc = proc;
	connection->param = param;
}

void _connection_set_userdata(_connection_t* connection, void* userdata) {
	_ASSERT(connection != NULL);
	connection->userdata = userdata;
}

void* _connection_userdata(_connection_t const* connection) {
	_ASSERT(connection != NULL);
	return connection->userdata;
}

_connection_poll_t* _connection_poll_create(_status_t* status) {
	_ASSERT(status != NULL);

#ifdef __linux__
	int pollfd = epoll_create(1);
	if (pollfd >= 0) {
		_status_set(status, _SUCCESS_STATUS_TYPE, NULL);
		return _NEW(_connection_poll_t, { .pollfd = pollfd });
	} else {
		_status_set(status, _FAILURE_STATUS_TYPE, strerror(errno));
		return NULL;
	}
#else
	return _NEW(_connection_poll_t, {});
#endif
}

void _connection_poll_destroy(_connection_poll_t* poll) {
	_ASSERT(poll != NULL);
#ifdef __linux__
	close(poll->pollfd);
#else
#endif
	_FREE(poll->connections.data);
	_FREE(poll);
}

void _connection_poll_process(_connection_poll_t* poll) {
	_ASSERT(poll != NULL);

#ifdef __linux__
	epoll_event_t ev[255];
	int ret = epoll_wait(poll->pollfd, ev, 255, 1);
	if (ret >= 0) {
		for (int i = 0; i < ret; i++) {
			if (ev[i].events & EPOLLIN) {
	            _connection_t* conn = (_connection_t*)ev[i].data.ptr;
	            _ASSERT(conn != NULL);
	            if (conn->proc != NULL) {
	            	conn->proc(
	            		&(_connection_event_t){
	            			.type = _READ_CONNECTION_EVENT,
	            			.connection = conn
	            		},
	            		conn->param
	            	);
	            }
			}
		}
	} else {
		_ABORT("epoll_wait() failed: %s\n", strerror(ret));
	}
#else
#endif
}

void _connection_poll_append(_connection_poll_t* poll, _connection_t* connection) {
	_ASSERT(poll != NULL);
	_ASSERT(connection != NULL);

	if (_INDEX_OF(poll->connections, connection) > -1) {
		return;
	}

	_PUSH(poll->connections, connection);

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

	if (_INDEX_OF(poll->connections, connection) == -1) {
		return;
	}

	_REMOVE(poll->connections, connection);

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