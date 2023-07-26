#include "StandardWindow.h"

StandardWindow::WindowClass StandardWindow::WindowClass::wndcls;

StandardWindow::WindowClass::WindowClass::WindowClass()
{
	hinst = GetModuleHandle(nullptr);
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.hInstance = hinst;
	wc.lpszClassName = classNm;
	wc.style = CS_DBLCLKS;//Enables window to take double click events
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpfnWndProc = DefWindowProc;
	//wc.hIcon = static_cast<HICON>(LoadImage(hinst, MAKEINTRESOURCE(IDI_ICON1) , IMAGE_ICON,48 , 48 , 0 ));
	//wc.hIconSm = static_cast<HICON>(LoadImage(hinst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32 , 32 , 0));
	RegisterClassEx(&wc);
}
StandardWindow::WindowClass::WindowClass::~WindowClass()
{
	UnregisterClass(classNm, hinst);
}

constexpr const char* StandardWindow::WindowClass::WindowClass::GetName()
{
	return classNm;
}

HINSTANCE StandardWindow::WindowClass::WindowClass::GetInstance()
{
	return wndcls.hinst;
}

StandardWindow::StandardWindow(const std::string& Title, unsigned int Width, unsigned int Height , StandardWindow* Parent)
	:
Window(Parent , 0 , WindowClass::GetName() , Title , WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT , Width , Height )
{}
