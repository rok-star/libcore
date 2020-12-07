#include <libcore/context.h>

typedef struct _context_t {
    HWND hwnd;
    _texture_t* texture;
} _context_t;

_context_t* _context_create(void) {
	_ASSERT();
}

void _context_destroy(_context_t* context) {
	_ASSERT();
}

void _context_begin_paint(_context_t* context) {
	_ASSERT();
}

void _context_end_paint(_context_t* context) {
	_ASSERT();
}