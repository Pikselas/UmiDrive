#include"Label.h"
#include"TextButton.h"
#include"ScrollWindow.h"
#include "StandardWindow.h"
#include "ImageButton.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    StandardWindow window("UmiDrive");
    
    Label Label1(window, "Hello World!", 10, 10, 100, 100);
    
    ScrollVerticalWindow sw(window, 150, 20, 400, 400 , 800);

    TextButton Button1(sw, "Click Me!", 10, 120, 100, 100);

    Label LScroll1(sw, "INSIDE-SCROLL-1", 10, 10, 150, 20);
    Label LScroll2(sw, "INSIDE-SCROLL-2", 50, 100, 150, 20);
    Label LScroll3(sw, "INSIDE-SCROLL-3", 30, 200, 250, 40);
    Label LScroll4(sw, "INSIDE-SCROLL-4", 40, 650, 150, 20);

    const auto img = LoadImage(NULL, "D:/SeqDownLogo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    ImageButton imgwnd(sw, "TEST BUTTON" ,img, 10, 25, 200, 200);

    Button1.OnClick = [& , i = 0](TextButton& btn)  mutable
    {
        Label1.ChangeTitle("CLICKED" + std::to_string(++i));
	};
    
    StandardWindow::MainLoop();
    return 0;
}