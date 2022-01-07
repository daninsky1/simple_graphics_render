#include <stdio.h>
#include <stdint.h>

#include <exception>

#include "win32_san.h"

#include "san.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"


static BITMAPINFO glob_bmi;
static void *glob_bkbuf;
static void *glob_dd_bkbuf;
static int glob_bkbuf_w;
static int glob_bkbuf_h;
static BOOL running = true;

static San::State *glob_state;

static uint8_t *glob_ttf_buf;
static char glob_ttf_path_buf[1<<25];


void ttf_init()
{
    stbtt_fontinfo font;

    int w = 0;
    int h = 0;
    int i = 0;
    int j = 0;
    int c = 'a';
    float s = 20.0f;


    fread(glob_ttf_path_buf, 1, 1<<25, fopen("c:/windows/fonts/consola.ttf", "rb"));
    stbtt_InitFont(&font, reinterpret_cast<uint8_t*>(glob_ttf_path_buf),
        stbtt_GetFontOffsetForIndex(reinterpret_cast<uint8_t*>(glob_ttf_path_buf), 0));
    glob_ttf_buf = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, s), c &w, &h, 0, 0, 0);

    stbtt_FreeBitmap(glob_ttf_buf, NULL);
}

INT WINAPI WinMain(HINSTANCE hinstance, HINSTANCE h_prev_instance, PSTR lp_cmd_line, INT n_cmd_show)
{
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

    hinst = hinstance;

    int wndw = 500;
    int wndh = 500;
    HWND hwindow = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,         // NOTE(daniel): An optional extended window style, I don't know what this mean
        win_cls_ex.lpszClassName,              // The name of the application
        title,                          // The text that appears in the title bar
        WS_OVERLAPPEDWINDOW,            // the type of window to create
        CW_USEDEFAULT, CW_USEDEFAULT,   // Initial position (x, y)
        wndw, wndh,                     // Initial size (width, length)
        NULL,                           // Parent window
        NULL,                           // Menu bar
        hinstance,
        NULL                            // Not used in this application
    );

    if (!hwindow) {
        running = false;
        // TODO(daniel): LOG
    }


    ShowWindow(hwindow, n_cmd_show);
    //UpdateWindow(hwindow);

    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    char hello_msg[] = "Hello, Window desktop!";

    switch (msg) {
    case WM_CREATE: {
        // TODO(daniel): Create the init process

        glob_state = static_cast<San::State*>(malloc(sizeof(San::State)));
        glob_state->mode = Mode::normal;

        RECT client_rect;
        GetClientRect(hwnd, &client_rect);

        int width = client_rect.right - client_rect.left;
        int height = client_rect.bottom - client_rect.top;

        resize_dib_section(width, height);

        int n = 2;
    } break;
    case WM_SIZE: {
        RECT client_rect;
        GetClientRect(hwnd, &client_rect);

        int width = client_rect.right - client_rect.left;
        int height = client_rect.bottom - client_rect.top;

        resize_dib_section(width, height);
    } break;
    case WM_PAINT: {
        PAINTSTRUCT paint_struct;
        HDC hdc = BeginPaint(hwnd, &paint_struct);

        RECT client_rect;
        GetClientRect(hwnd, &client_rect);

        update_window(hdc, client_rect, paint_struct.rcPaint);

        EndPaint(hwnd, &paint_struct);
        //ReleaseDC(hwnd, hdc);
    } break;
    case WM_CLOSE: {
        // NOTE(doc): The WM_CLOSE message fiver you an opportunity to prompt
        // the user before closing the window.
        // TODO(daniel): Prompt the user before closging the window.
        DestroyWindow(hwnd);
    } break;
    case WM_DESTROY: {
        PostQuitMessage(0);
    } break;
    case WM_KEYDOWN: case WM_KEYUP:
    case WM_SYSKEYDOWN: case WM_SYSKEYUP: {
        // NOTE(daniel): return the low order byte from wparam
        WORD vkcode = LOWORD(wparam);

        BYTE scan_code = LOBYTE(HIWORD(lparam));
        BOOL scan_code_e0 = (HIWORD(lparam) & KF_EXTENDED) == KF_EXTENDED;

        BOOL up_flag = (HIWORD(lparam) & KF_UP) == KF_UP;
        BOOL repeat_flag = (HIWORD(lparam) & KF_REPEAT) == KF_REPEAT;
        WORD repeat_count = LOWORD(lparam);

        BOOL alt_down_flag = (HIWORD(lparam) & KF_ALTDOWN) == KF_ALTDOWN;

        BOOL dlg_mode_flag = (HIWORD(lparam) & KF_DLGMODE) == KF_DLGMODE;
        BOOL menu_mode_flag = (HIWORD(lparam) & KF_MENUMODE) == KF_MENUMODE;

        char* vkcodestr = static_cast<char*>(malloc(sizeof(char)*128));
        sprintf(vkcodestr, "vkcode: %d - repeat_count: %d\n", vkcode, repeat_count);
        OutputDebugStringA(vkcodestr);

        if (glob_state->mode == Mode::insert) {
            //switch (wparam)
        }
    } break;
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
        break;
    }

    return 0;
}

void clear_screen(void *buffer)
{
    int pitch = w*bytes_per_pixel;
    uint8_t *row = static_cast<uint8_t*>(glob_bkbuf);
    for (int y = 0; y < h; ++y) {
        uint8_t *pixel = row;
        for (int x = 0; x < w; ++x) {
            *pixel = 0;
            ++pixel;
            *pixel = 255;
            ++pixel;
            *pixel = 0;
            ++pixel;
            ++pixel;
        }
        row += pitch;
    }
}

void resize_dib_section(int w, int h)
{
    if (glob_bkbuf) {
        //VirtualFree(glob_bkbuf, 0, MEM_RELEASE);
        free(glob_bkbuf);
    }

    glob_bkbuf_w = w;
    glob_bkbuf_h = h;

    glob_bmi.bmiHeader.biSize          = sizeof(glob_bmi.bmiHeader);
    glob_bmi.bmiHeader.biWidth         = w;
    glob_bmi.bmiHeader.biHeight        = -h;
    glob_bmi.bmiHeader.biPlanes        = 1;
    glob_bmi.bmiHeader.biBitCount      = 32;
    glob_bmi.bmiHeader.biCompression   = BI_RGB;

    int bytes_per_pixel = 4;
    size_t bkbuf_sz = w*h*bytes_per_pixel;
    glob_bkbuf = static_cast<void*>(malloc(bkbuf_sz));

    //glob_bkbuf = VirtualAlloc(0, bkbuf_sz, MEM_COMMIT, PAGE_READWRITE);

    if (!glob_bkbuf) OutputDebugString("bitmap buffer allocation fail\n");

    clear_screen();
}


void update_window(HDC hdc, RECT client_rect, RECT src_rect)
{
    int wdest = client_rect.right - client_rect.left;
    int hdest = client_rect.bottom - client_rect.top;
    int wsrc = src_rect.right - src_rect.left;
    int hsrc = src_rect.bottom - src_rect.top;

    StretchDIBits(
        hdc,
        //0, 0, wsrc, hsrc,
        0, 0, wsrc, hsrc,
        0, 0, wdest, hdest,
        glob_bkbuf,
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
