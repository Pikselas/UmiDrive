#include "Window.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    Window wnd( nullptr , 0 , "UmiDrive" , WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX , CW_USEDEFAULT , CW_USEDEFAULT , 640 , 480 );
    Window::MainLoop(nullptr);
    return 0;
}