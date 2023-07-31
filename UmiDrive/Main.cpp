#include"Label.h"
#include"TextButton.h"
#include"ScrollWindow.h"
#include "StandardWindow.h"
#include "ImageButton.h"
#include "TextEntry.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    StandardWindow window("UmiDrive");
    Label Label1(window, "Hello World!", 10, 10, 100, 100);
    ScrollVerticalWindow sw(window, 150, 20, 600, 400 , 800);
    TextButton Button1(sw, "Click Me!", 10, 120, 100, 100);
    const auto img = LoadImage(NULL, "D:/SeqDownLogo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    ImageButton imgwnd1(sw, "TEST BUTTON" ,img, 5, 5, 200, 200);

    TextEntry entry(sw, 5, 210, 100, 20, "HELLO TEST");

    Button1.OnClick = [& , i = 0](TextButton& btn)  mutable
    {
        Label1.ChangeTitle("CLICKED" + std::to_string(++i));
	};
    
    StandardWindow::MainLoop();
    return 0;
}