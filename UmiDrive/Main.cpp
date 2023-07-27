#include"Label.h"
#include"TextButton.h"
#include "StandardWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    StandardWindow window("UmiDrive");
    
    Label Label1(&window, "Hello World!", 10, 10, 100, 100);
    TextButton Button1(&window, "Click Me!", 10, 120, 100, 100);
    
    Button1.OnClick = [& , i = 0](TextButton& btn)  mutable
    {
        Label1.ChangeTitle("CLICKED" + std::to_string(++i));
	};
    
    StandardWindow::MainLoop();
    return 0;
}