#include "sgr.h"

HDC glob_hdc = NULL;
RECT glob_client_rect;
RECT glob_src_rect;

void clear_screen(OffscreenBuffer buffer, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t *row = static_cast<uint8_t*>(buffer.memory);
    for (int y = 0; y < buffer.height; ++y) {
        uint8_t *pixel = row;
        for (int x = 0; x < buffer.width; ++x) {
            *pixel = b;
            ++pixel;
            *pixel = g;
            ++pixel;
            *pixel = r;
            ++pixel;
            ++pixel;
        }
        row += buffer.pitch;
    }
}

void DEBUG_draw_test_box(OffscreenBuffer buffer, HDC hdc, RECT client_rect, RECT src_rect, int w, int h, int size)
{
    glob_hdc = hdc;
    glob_client_rect = client_rect;
    glob_src_rect = src_rect;
    DEBUG_draw_test_rect(buffer, w, h, size);
    DEBUG_draw_test_rhombus(buffer, w, h, size);
    DEBUG_draw_test_xs(buffer, w, h, size);
}

void DEBUG_draw_test_rect(OffscreenBuffer buffer, int w, int h, int size)
{
    /* NOTE(daniel): Center square to test straight lines and drawing order
     * --------------->
     * ^              |
     * |              |
     * |              |
     * |              |
     * |              ∨
     * <---------------
     */
    // x, h are in the center of the box
    size /= 2;
    line_raster_dda(buffer, 255, 255, 255,
        w - size, h - size, w + size, h - size);
    line_raster_dda(buffer, 255, 255, 255,
        w + size, h - size, w + size, h + size);
    line_raster_dda(buffer, 255, 255, 255,
        w + size, h + size, w - size, h + size);
    line_raster_dda(buffer, 255, 255, 255,
        w - size, h + size, w - size, h - size);
}

void DEBUG_draw_test_rhombus(OffscreenBuffer buffer, int w, int h, int size)
{
    // w, h are in the center of the bow
    size /= 2;
    line_raster_dda(buffer, 255, 255, 255,
        w, h - size, w + size, h);
    line_raster_dda(buffer, 255, 255, 255,
        w + size, h, w, h + size);
    line_raster_dda(buffer, 255, 255, 255,
        w, h + size, w - size, h);
    line_raster_dda(buffer, 255, 255, 255,
        w - size, h, w, h - size);
}

void DEBUG_draw_test_xs(OffscreenBuffer buffer, int w, int h, int size)
{
    size /= 2;
    line_raster_dda(buffer, 255, 255, 255,
        w - size, h - size, w + size, h);
    line_raster_dda(buffer, 255, 255, 255,
        w - size, h, w + size, h - size);
    line_raster_dda(buffer, 255, 255, 255,
        w + size, h + size, w - size, h);
    line_raster_dda(buffer, 255, 255, 255,
        w + size, h, w - size, h + size);
}

void line_raster_dda(OffscreenBuffer buffer, uint8_t r, uint8_t g, uint8_t b, int x0, int y0, int x1, int y1,
    double width)
{
    // Raster a line with a Digital Diferential Analyzer algorithm
    double dx = x1 - x0;   // Distance x
    double dy = y1 - y0;   // Distance y
    double step = 0;

    if (fabs(dx) >= abs(dy)) { step = fabs(dx); }
    else { step = fabs(dy); }
    dx /= step;
    dy /= step;
    
    double x = x0;
    double y = y0;
    uint8_t *pixel;

    for (double i = 0; i < step; ++i) {
        // NOTE(daniel): Truncating is supposed to be faster than rounding, should be benchmarked
        pixel = static_cast<uint8_t*>(buffer.memory) + (static_cast<int>(y) * buffer.pitch + (static_cast<int>(x)
            * buffer.bytes_per_pixel));
        *pixel = r;
        *(pixel+1) = g;
        *(pixel+2) = b;
        x += dx;
        y += dy;
        r++;
        g++;
        win32_display_buffer(glob_hdc, glob_client_rect, glob_src_rect);
    }
}

void line_raster_bresenham(OffscreenBuffer buffer, uint8_t r, uint8_t g, uint8_t b, int x0, int y0, int x1, int y1)
{
    // Raster a line with Bresenham's algorithm
    int dx = x1 - x0;   // Distance x
    int dy = y1 - y0;   // Distance y
    double slope = dy / dy;
    double d = 2 * dy - dx;

    uint8_t *row = static_cast<uint8_t*>(buffer.memory);
    uint8_t *pixel = row + (y0 * buffer.pitch + (x0 * buffer.bytes_per_pixel));

    if (true) {
        int y = y0;
        for (int x = x0; x < x1; ++x) {
            pixel = row + (y * buffer.pitch + (x * buffer.bytes_per_pixel));
            *(pixel) = r;
            *(pixel+1) = g;
            *(pixel+2) = b;
            if (d > 0) {
                ++y;
                d = d - 2 * dx;
            }
            d = d + 2 * dy;
        }
    }
    else {

    }
}

