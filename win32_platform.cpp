#include "utils.cpp"
#include <windows.h>

#define IDM_OPEN 1
#define IDM_SAVE 2
#define IDM_EXIT 3
#define IDM_ABOUT 4

global_variable bool running = true;
global_variable constexpr unsigned int window_width = 840;
global_variable constexpr unsigned int window_height = 640;
global_variable constexpr auto window_title = "Pong AI";

struct Render_State {
    int height, width;
    void* memory;
    BITMAPINFO bitmap_info;
};

global_variable Render_State render_state;

#include "platform_common.cpp"
#include "renderer.cpp"
#include "game.cpp"

LRESULT CALLBACK window_callback(HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam) {

    LRESULT result = 0;

    switch (uMsg) {
    case WM_CLOSE:
    case WM_DESTROY: {
        running = false;
    } break;

    case WM_SIZE: {
        RECT rect;
        GetClientRect(hwnd, &rect);
        render_state.width = rect.right - rect.left;
        render_state.height = rect.bottom - rect.top;

        int size = render_state.width * render_state.height * sizeof(unsigned int);

        if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
        render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
        render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
        render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
        render_state.bitmap_info.bmiHeader.biPlanes = 1;
        render_state.bitmap_info.bmiHeader.biBitCount = 32;
        render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
    } break;

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDM_OPEN:
            MessageBox(hwnd, "Open selected", "Menu", MB_OK);
            break;
        case IDM_SAVE:
            MessageBox(hwnd, "Ïðîåêò áûë íàïèñàí ñòóäåíòàìè 1-ãî êóðñà óíèâåðñèòåòà ÍÈÓ ÂØÝ, îáðàçîâàòåëüíîé ïðîãðàììû 'Èíôîðìàöèîííàÿ áåçîïàñíîñòü'. Ýéíóëëàåâ Ãåéäàð, ×èíêîâ Àðòåì, Ñàôèíà Àëåêñàíäðà.", "Menu", MB_OK);
            break;
        case IDM_EXIT:
            DestroyWindow(hwnd);
            break;
        case IDM_ABOUT:
            MessageBox(hwnd, "Äàííîå ïðèëîæåíèå ïðåäñòàâëÿåò èç ñåáÿ èãðó Ïèíã-Ïîíã, â êîòîðîé ìîæíî èãðàòü êàê ñ ÷åëîâåêîì, òàê è ñ AI", "About", MB_OK);
            break;
        }
    } break;

    default: {
        result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    }

    return result;
}

int WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd) {

    ShowCursor(TRUE);

    // Create Window Class
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = "Game Window Class";
    window_class.lpfnWndProc = window_callback;

    // Register Class
    RegisterClass(&window_class);

    // Create Window
    HWND window = CreateWindow(window_class.lpszClassName, window_title,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        window_width, window_height,
        0, 0, hInstance, 0);

    HMENU hMenu = CreateMenu();
    HMENU hSubMenuFile = CreatePopupMenu();
    HMENU hSubMenuHelp = CreatePopupMenu();

    AppendMenu(hSubMenuFile, MF_STRING, IDM_SAVE, "Î ðàçðàáîò÷èêàõ");
    AppendMenu(hSubMenuFile, MF_SEPARATOR, 0, NULL); // Ðàçäåëèòåëü
    AppendMenu(hSubMenuFile, MF_STRING, IDM_EXIT, "Âûõîä");

    AppendMenu(hSubMenuHelp, MF_STRING, IDM_ABOUT, "Èíñòðóêöèÿ ïî èãðå");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenuFile, "Î ïðîåêòå");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenuHelp, "Ïîìîùü");

    SetMenu(window, hMenu);

    // Fullscreen
    SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
    MONITORINFO mi = { sizeof(mi) };
    GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
    SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

    HDC hdc = GetDC(window);

    Input input = {};

    float delta_time = 0.016666f;
    LARGE_INTEGER frame_begin_time;
    QueryPerformanceCounter(&frame_begin_time);

    float performance_frequency;
    {
        LARGE_INTEGER perf;
        QueryPerformanceFrequency(&perf);
        performance_frequency = (float)perf.QuadPart;
    }

    while (running) {
        // Input
        MSG message;

        for (int i = 0; i < BUTTON_COUNT; i++) {
            input.buttons[i].changed = false;
        }

        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {

            switch (message.message) {
            case WM_KEYUP:
            case WM_KEYDOWN: {
                u32 vk_code = (u32)message.wParam;
                bool is_down = ((message.lParam & (1 << 31)) == 0);

#define process_button(b, vk)\
case vk: {\
input.buttons[b].changed = is_down != input.buttons[b].is_down;\
input.buttons[b].is_down = is_down;\
} break;

                switch (vk_code) {
                    process_button(BUTTON_UP, VK_UP);
                    process_button(BUTTON_DOWN, VK_DOWN);
                    process_button(BUTTON_W, 'W');
                    process_button(BUTTON_S, 'S');
                    process_button(BUTTON_LEFT, VK_LEFT);
                    process_button(BUTTON_RIGHT, VK_RIGHT);
                    process_button(BUTTON_ENTER, VK_RETURN);
                    process_button(BUTTON_ESC, VK_ESCAPE);
                }
            } break;

            default: {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
            }

        }

        // Simulate
        simulate_game(&input, delta_time);

        // Render
        StretchDIBits(hdc,
            0,
            0,
            render_state.width,
            render_state.height,
            0,
            0,
            render_state.width,
            render_state.height,
            render_state.memory,
            &render_state.bitmap_info,
            DIB_RGB_COLORS, SRCCOPY);

        LARGE_INTEGER frame_end_time;
        QueryPerformanceCounter(&frame_end_time);
        delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
        frame_begin_time = frame_end_time;
    }
    return EXIT_SUCCESS;
}
