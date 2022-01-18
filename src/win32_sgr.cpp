#include <stdio.h>
#include <stdint.h>

#include <exception>

#include "win32_sgr.h"

#include "sgr.h"


static BITMAPINFO glob_bmi;
static OffscreenBuffer glob_bkbuf;
static void *glob_dd_bkbuf;
static int glob_bkbuf_w;
static int glob_bkbuf_h;
static BOOL running = true;


static uint8_t *glob_ttf_buf;
static char glob_ttf_path_buf[1<<25];

#pragma warning(disable : 4100)
INT WINAPI WinMain(HINSTANCE hinstance, HINSTANCE h_prev_instance, PSTR lp_cmd_line, INT n_cmd_show)
{
#pragma warning(default : 4100)
    // NOTE(daniel): Window Class Extended
    WNDCLASSEX win_cls_ex;

    win_cls_ex.cbSize        = sizeof(WNDCLASSEX);
    win_cls_ex.style         = CS_HREDRAW | CS_VREDRAW;
    win_cls_ex.lpfnWndProc   = wnd_proc;
    win_cls_ex.cbClsExtra    = 0;
    win_cls_ex.cbWndExtra    = 0;
    win_cls_ex.hInstance     = hinstance;
    win_cls_ex.hIcon         = LoadIcon(win_cls_ex.hInstance, IDI_APPLICATION);
    win_cls_ex.hCursor       = LoadCursor(NULL, IDC_ARROW);
    win_cls_ex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    win_cls_ex.lpszMenuName  = NULL;
    win_cls_ex.lpszClassName = "Text Editor Window";
    win_cls_ex.hIconSm       = LoadIcon(win_cls_ex.hInstance, IDI_APPLICATION);


    // NOTE(daniel): Register Window
    if (!RegisterClassEx(&win_cls_ex)) {
        // TODO(daniel): LOG
        return 1;
    }


    int wndw = 500;
    int wndh = 500;
    HWND hwindow = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,         // NOTE(daniel): An optional extended window style, I don't know what this mean
        win_cls_ex.lpszClassName,              // The name of the application
        "Simple Graphics Render",                          // The text that appears in the title bar
        WS_OVERLAPPEDWINDOW,            // the type of window to create
        CW_USEDEFAULT, CW_USEDEFAULT,   // Initial position (x, y)
        wndw, wndh,                     // Initial size (width, length)
        NULL,                           // Parent window
        NULL,                           // Menu bar
        hinstance,
        NULL                            // Not used in this application
    );

    if (!hwindow) { running = false; }
    else { running = true; }


    ShowWindow(hwindow, n_cmd_show);
    //UpdateWindow(hwindow);

    MSG msg = { };
    HDC hdc = NULL;
    while (running) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) { break; }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        RECT client_rect;
        GetClientRect(hwindow, &client_rect);
        RECT src_rect = client_rect;
        resize_dib_section(src_rect.right, src_rect.bottom);
        if (!hdc) {
            hdc = GetDC(hwindow);
        }

        DEBUG_draw_test_box(glob_bkbuf, hdc, client_rect, src_rect, glob_bkbuf.width/2, glob_bkbuf.height/2, 300);
    }
    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    char hello_msg[] = "Hello, Window desktop!";

    switch (msg) {
    case WM_CREATE: {
    } break;
    case WM_SIZE: {
        RECT client_rect;
        GetClientRect(hwnd, &client_rect);

        int width = client_rect.right - client_rect.left;
        int height = client_rect.bottom - client_rect.top;

        resize_dib_section(width, height);
    } break;
    case WM_CLOSE: {
        // NOTE(doc): The WM_CLOSE message fiver you an opportunity to prompt
        // the user before closing the window.
        // TODO(daniel): Prompt the user before closging the window.
        DestroyWindow(hwnd);
    } break;
    case WM_DESTROY: {
        PostQuitMessage(0);
        running = false;
    } break;
    //case WM_KEYDOWN: case WM_KEYUP:
    //case WM_SYSKEYDOWN: case WM_SYSKEYUP: {
    //    // NOTE(daniel): return the low order byte from wparam
    //    WORD vkcode = LOWORD(wparam);

    //    BYTE scan_code = LOBYTE(HIWORD(lparam));
    //    BOOL scan_code_e0 = (HIWORD(lparam) & KF_EXTENDED) == KF_EXTENDED;

    //    BOOL up_flag = (HIWORD(lparam) & KF_UP) == KF_UP;
    //    BOOL repeat_flag = (HIWORD(lparam) & KF_REPEAT) == KF_REPEAT;
    //    WORD repeat_count = LOWORD(lparam);

    //    BOOL alt_down_flag = (HIWORD(lparam) & KF_ALTDOWN) == KF_ALTDOWN;

    //    BOOL dlg_mode_flag = (HIWORD(lparam) & KF_DLGMODE) == KF_DLGMODE;
    //    BOOL menu_mode_flag = (HIWORD(lparam) & KF_MENUMODE) == KF_MENUMODE;

    //    char* vkcodestr = static_cast<char*>(malloc(sizeof(char)*128));
    //    sprintf(vkcodestr, "vkcode: %d - repeat_count: %d\n", vkcode, repeat_count);
    //    OutputDebugStringA(vkcodestr);
    //} break;
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
        break;
    }

    return 0;
}

void resize_dib_section(int w, int h)
{
    if (glob_bkbuf.memory) {
        free(glob_bkbuf.memory);
    }

    glob_bkbuf.width = w;
    glob_bkbuf.height = h;
    glob_bkbuf.bytes_per_pixel = 4;
    glob_bkbuf.pitch = glob_bkbuf.bytes_per_pixel * glob_bkbuf.width;
    glob_bkbuf.size = glob_bkbuf.height*glob_bkbuf.pitch;
    glob_bkbuf.memory = static_cast<void*>(malloc(glob_bkbuf.size));

    glob_bmi.bmiHeader.biSize          = sizeof(glob_bmi.bmiHeader);
    glob_bmi.bmiHeader.biWidth         = glob_bkbuf.width;
    glob_bmi.bmiHeader.biHeight        = -glob_bkbuf.height;
    glob_bmi.bmiHeader.biPlanes        = 1;
    glob_bmi.bmiHeader.biBitCount      = 32;
    glob_bmi.bmiHeader.biCompression   = BI_RGB;

    if (!glob_bkbuf.memory) OutputDebugString("bitmap buffer allocation fail\n");

    clear_screen(glob_bkbuf, 150, 3, 234);
}


void win32_display_buffer(HDC hdc, RECT client_rect, RECT src_rect)
{
    int destw = client_rect.right - client_rect.left;
    int desth = client_rect.bottom - client_rect.top;
    int srcw = src_rect.right - src_rect.left;
    int srch = src_rect.bottom - src_rect.top;

    StretchDIBits(
        hdc,
        //0, 0, wsrc, hsrc,
        0, 0, srcw, srch,
        0, 0, destw, desth,
        glob_bkbuf.memory,
        &glob_bmi,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}

void call_save_file(HWND hwnd)
{
    LPTSTR fn = (LPTSTR)malloc(255);
    OPENFILENAME ofn = { };
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = fn;
    ofn.nMaxFile = 255;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = NULL;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = NULL;
    ofn.Flags = OFN_EXPLORER;
    ofn.lpstrDefExt = NULL;

    BOOL err = GetSaveFileName(&ofn);
    DWORD exterr = CommDlgExtendedError();

    char szFile[MAX_PATH];
    ZeroMemory(&ofn,sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = TEXT('\0');
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = TEXT("ALL\0*.*\0Text\0*.txt\0C++\0*.cpp\0mp3\0*.mp3\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.hwndOwner = hwnd;
    ofn.Flags = OFN_EXPLORER;

    //err = GetOpenFileName(&ofn);
    exterr = CommDlgExtendedError();
}
