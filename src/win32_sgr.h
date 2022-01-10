#include <windows.h>
#include <stdlib.h>
#include <string.h>



LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
void resize_dib_section(int w, int h);
void update_window(HDC hdc, RECT wnd_rect, RECT src_rect);
