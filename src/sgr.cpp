#include "sgr.h"


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

void draw_line(OffscreenBuffer buffer, uint8_t r, uint8_t g, uint8_t b, int x0, int y0,
    int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    double slope = dy / dy;
    double d = 2 * dy - dx;

    uint8_t *row = static_cast<uint8_t*>(buffer.memory);
    uint8_t *pixel = row + (y0 * buffer.pitch + (x0 * buffer.bytes_per_pixel));
    *(pixel) = r;
    *(pixel+1) = g;
    *(pixel+2) = b;
    pixel = row + (y1 * buffer.pitch + (x1 * buffer.bytes_per_pixel));
    *(pixel) = r;
    *(pixel+1) = g;
    *(pixel+2) = b;

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

