#include <stdint.h>


struct OffscreenBuffer {
    void *memory;
    int size;
    int width;
    int height;
    int pitch;
    int bytes_per_pixel;
};

void clear_screen(OffscreenBuffer buffer, uint8_t r, uint8_t g, uint8_t b);
void draw_line(OffscreenBuffer buffer, uint8_t r, uint8_t g, uint8_t b, int x0, int y0,
    int x1, int y1);


