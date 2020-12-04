#include <libcore/context.h>

typedef struct _Context {
    HWND hwnd;
    _Texture* texture;
    ;
    int width;
    int height;
    float origin;
    bool clip_rect;
    int clip_rect_x;
    int clip_rect_y;
    int clip_rect_width;
    int clip_rect_height;
} _Context;

_Context* _Context_create(void) {
	_ASSERT();
}

void _Context_destroy(_Context* context) {
	_ASSERT();
}

void _Context_begin_paint(_Context* context) {
	_ASSERT();
}

void _Context_end_paint(_Context* context) {
	_ASSERT();
}