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

void _Context_fill_rect(_Context* context, _RectF* rect, _Color* color) {
	_ASSERT();
}

void _Context_frame_rect(_Context* context, _RectF* rect, _Color* color, double width) {
	_ASSERT();
}

void _Context_draw_texture(_Context* context, _RectF* src, _RectF* dst, _Texture* texture, _Color* tint) {
	_ASSERT();
}

void _Context_set_clip_rect(_Context* context, _RectF* rect) {
	_ASSERT();
}