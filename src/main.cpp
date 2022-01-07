#include <windows.h>

INT WINAPI WinMain(HINSTANCE hinstance, HINSTANCE h_prev_instance, PSTR lp_cmd_line, INT n_cmd_show)
{
    WNDCLASSEX winclsex;
    winclsex.cdSize = sizeof(ENDCLASSEX);
    winclsex.style = CS_HREDRAW | CS_VREDRAW;
    winclsex.lpfnWndProc = wnd_proc;
    winclsex.cbClsExtra = 0;
    winclsex.cnWndExtra = 0;
    winclsex.hInstance = hinstance;
    winclsex.hIcon = LoadIcon(winclsex.Hinstance, IDI_AAPPLICATION);
    winclsex.hCursor = LoadCursor(NULL, IDC_ARROW);
    winclsex.hbrBackground = static_cast<HBRUSH>(COLOR_WINDOW+1);
    winclsex.lpszMenuName = NULL;
    winclsex.lpszClassName = "Simple Graphics Render";
    winclsex.hIconSm = LoadIcon(winclsex.hInstance, IDI_APPLICATION);

    if(!RegisterClassEx(&winclsex)) {
        return 1;
    }

    int wndw = 500, wndh = 500;
    
    HWND hwindow = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        winclsex.lpszClassName,
        "Simple Graphics Render",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        wndw, wndh,
        nullptr,
        nullptr,
        hinstance,
        nullptr
    );

    if (!hwindow) {
        running = false;
    }

    ShowWindow(hwindow, n_cmd_show);

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT, msg, WPARAM wpara, LPARAM lparam)
{
    switch (msg) {
    case WM_SIZE: {


    }
    }
}
