//заголовочный файл, в котором объявляются функции, предоставляющие интерфейс доступа к виндовс апи
#include <windows.h> //чтобы мы могли использовать функционал в нашей программе, предоставляемый ос 

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//точка входа в графическое приложение виндовс, предоставляемая пользователем, винмэйн - условное имя для точки входа 
int WinMain(HINSTANCE hInstanse, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    //создаем класс виндовс
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = "game window class";
    window_class.lpfnWndProc = window_callback;
    //...
    RegisterClass(&window_class);

    //...
    CreateWindow(window_class.lpszClassName, "game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, )
}