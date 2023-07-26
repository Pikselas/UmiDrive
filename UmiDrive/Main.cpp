#include "StandardWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    StandardWindow Window("UmiDrive");
    StandardWindow::MainLoop(nullptr);
    return 0;
}