#include <windows.h>
#include <stdlib.h>
#include <string.h>
//
// GLOBAL VARIABLES
//
// NOTE(daniel): sz prefix on win32 means zero-terminated string data type
static char* window_class_name = "DesktopApp";
static char* title = "Windows Desktop App";
HINSTANCE hinst;

struct StateInfo {

    
};

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
void resize_dib_section(int w, int h);
void update_window(HDC hdc, RECT wnd_rect, RECT src_rect);
