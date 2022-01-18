#pragma once
#include "sgr.h"
#include "win32_sgr.h"

#include <stdint.h>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>
#include <windows.h>


struct OffscreenBuffer {
    void *memory;
    int size;
    int width;
    int height;
    int pitch;
    int bytes_per_pixel;
};

struct Pointi { int x; int y; };

void clear_screen(OffscreenBuffer buffer, uint8_t r, uint8_t g, uint8_t b);
void line_raster_dda(OffscreenBuffer buffer, uint8_t r, uint8_t g, uint8_t b, int x0, int y0, int x1, int y1, double width=1);
void line_raster_bresenham(OffscreenBuffer buffer, uint8_t r, uint8_t g, uint8_t b, int x0, int y0, int x1, int y1);

void DEBUG_draw_test_box(OffscreenBuffer buffer, HDC hdc, RECT client_rect, RECT src_rect, int w, int h, int size);
void DEBUG_draw_test_rect(OffscreenBuffer buffer, int w, int h, int size);
void DEBUG_draw_test_rhombus(OffscreenBuffer buffer, int w, int h, int size);
void DEBUG_draw_test_xs(OffscreenBuffer buffer, int w, int h, int size);

