
#include"Label.h"
#include"TextButton.h"
#include"ScrollWindow.h"
#include "StandardWindow.h"
#include "ImageButton.h"
#include "TextEntry.h"

#include "DriveRuntime.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    StandardWindow window("UmiDrive");
    
    TextEntry token_entry(window , 5 , 10 , 150 , 30 , "Enter Token Here...");
    TextEntry email_entry(window , 5 , 50 , 150 , 30 , "Enter Email Here...");
    TextButton login_button(window ,"Login" , 5, 90, 150, 30);

    ScrollVerticalWindow contents_window(window , 170 , 10 , 600 , 550 , 800);
    
    std::string token = "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";
    std::string email = "pikselkichi@outlook.com";

   /* auto hDl = LoadLibrary(R"(D:\GithubReps\UmiDrive-Runtime\main.dll)");
    if (hDl == NULL)
	{
		MessageBox(NULL, "Failed to load main.dll", "Error", MB_OK);
		return 0;
	}

    auto LDrive = (decltype(&LoadDrive))GetProcAddress(hDl, "LoadDrive");
    if (LDrive == NULL)
	{
        MessageBox(NULL, "Failed to load LoadDrive", "Error", MB_OK);
		return 0;
    }*/
    //auto ID = LoadDrive(const_cast<char*>(token.c_str()), token.length(), const_cast<char*>(email.c_str()), email.length());

    //OutputDebugStringA(std::to_string(ID).c_str());

    //auto ID = LoadDrive(const_cast<char*>(token.c_str()), token.length(), const_cast<char*>(email.c_str()), email.length());
    //const auto img = LoadImage(NULL, "D:/SeqDownLogo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    


    StandardWindow::MainLoop();
    return 0;
}