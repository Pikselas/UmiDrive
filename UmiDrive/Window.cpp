#include"Window.h"

Window::WindowClass Window::WindowClass::wndcls;

Window::WindowClass::WindowClass()
{
	hinst = GetModuleHandle(nullptr);
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.hInstance = hinst;
	wc.lpszClassName = classNm;
	wc.style = CS_DBLCLKS;//Enables window to take double click events
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpfnWndProc = DefWindowProc;
	//wc.hIcon = static_cast<HICON>(LoadImage(hinst, MAKEINTRESOURCE(IDI_ICON1) , IMAGE_ICON,48 , 48 , 0 ));
	//wc.hIconSm = static_cast<HICON>(LoadImage(hinst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32 , 32 , 0));
	RegisterClassEx(&wc);
}
Window::WindowClass::~WindowClass()
{
	UnregisterClass(classNm , hinst);
}

constexpr const char* Window::WindowClass::GetName()
{
	return classNm;
}

HINSTANCE Window::WindowClass::GetInstance()
{
	return wndcls.hinst;
}

std::atomic_uint Window::WindowCount = 0;

LRESULT Window::StaticEventHandler(HWND handle, UINT msgcode, WPARAM wparam, LPARAM lparam)
{
	Window* const wnd_ptr = reinterpret_cast<Window*>(GetWindowLongPtr(handle, GWLP_USERDATA));
	return wnd_ptr->EventHandler(handle , msgcode , wparam , lparam);
}

LRESULT Window::EventHandler(HWND handle, UINT msgcode, WPARAM wparam, LPARAM lparam)
{
	switch (msgcode)
	{
	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	case WM_MOUSEMOVE:
	{
		POINTS pt = MAKEPOINTS(lparam);
		mouse.x = pt.x;
		mouse.y = pt.y;
	}
	}
	switch (msgcode)
	{
	 case WM_CLOSE:
		 Closed = true;
		 --WindowCount;
		 break;
	 case WM_MOUSEMOVE:
		 if (mouse.OnMove)
		 {
			 mouse.OnMove(*this);
		 }
		 break;
	 case WM_LBUTTONUP:
		 mouse.LeftPressed = false;
		 if (mouse.OnLeftRelease)
		 {
			 mouse.OnLeftRelease(*this);
		 }
	 break;
	 case WM_LBUTTONDOWN:
		 mouse.LeftPressed = true;
		 if (mouse.OnLeftPress)
		 {
			 mouse.OnLeftPress(*this);
		 }
	 break;
	 case WM_LBUTTONDBLCLK:
		 if (mouse.OnLeftDoubleClick)
		 {
			 mouse.OnLeftDoubleClick(*this);
		}
	 break;
	 case WM_RBUTTONUP:
		 mouse.RightPressed = false;
		 if (mouse.OnRightRelease)
		 {
			 mouse.OnRightRelease(*this);
		 }
	 break;
	 case WM_RBUTTONDOWN:
		 mouse.RightPressed = true;
		 if (mouse.OnRightPress)
		 {
			 mouse.OnRightPress(*this);
		 }
	 break;
	 case WM_RBUTTONDBLCLK:
		 if (mouse.OnRightDoubleClick)
		 {
			 mouse.OnRightDoubleClick(*this);
		 }
	 break;
	 case WM_MOUSEWHEEL:
		 {
			mouse.wheelDelta = GET_WHEEL_DELTA_WPARAM(wparam) / WHEEL_DELTA;
			if (mouse.OnWheel)
			{
				mouse.OnWheel(*this);
			}
		 }
	 break;
	 case WM_KEYDOWN:
	 {
		 //if repeated then bit 30 of lparam will be true
		 constexpr unsigned int Mask = 0b1 << 30;
		 bool repeated = ((Mask & lparam) != 0);
		 if (!repeated || keyboard.IsRepeatEnabled())
		 {
			 keyboard.KEY_STAT[static_cast<unsigned char>(wparam)] = true;
			 if (keyboard.OnKeyPress)
			 {
				 keyboard.OnKeyPress({*this , static_cast<unsigned char>(wparam)  , repeated});
			 }
		 }
	 }
	 break;
	 case WM_KEYUP:
	 {
		 keyboard.KEY_STAT[static_cast<unsigned char>(wparam)] = false;
		 if (keyboard.OnKeyRelease)
		 {
		   keyboard.OnKeyRelease({ *this , static_cast<unsigned char>(wparam)  , false });
		 }
	 }
	 break;
	 case WM_CHAR:
	 {
		 constexpr unsigned int Mask = 0b1 << 30;
		 bool repeated = ((Mask & lparam) != 0);
		 if (keyboard.OnCharInput)
		 {
			 keyboard.OnCharInput({ *this , static_cast<unsigned char>(wparam) , repeated });
		 }
	 }
	 break;
	 case WM_KILLFOCUS:
		 mouse.Reset();
		 keyboard.Reset();
		 break;
	}
	return DefWindowProc(handle , msgcode , wparam , lparam);
}

Window::Window(HWND Parent , DWORD exStyle , const std::string& name, DWORD Style , int x , int y , int width, int height) 
	: 
height(height), width(width)
{

	RECT wr = {0};
	wr.right = width;
	wr.bottom = height;

	AdjustWindowRect(&wr, Style, FALSE);

	window_handle = CreateWindowEx(exStyle, WindowClass::GetName(), name.c_str(), Style, x, y, wr.right - wr.left, wr.bottom - wr.top, Parent, nullptr,
		WindowClass::GetInstance(), nullptr);

	if (window_handle == nullptr)
	{
		throw Exception(GetLastError());
	}
	else
	{
		SetWindowLongPtr(window_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		SetWindowLongPtr(window_handle , GWLP_WNDPROC , reinterpret_cast<LONG_PTR>(StaticEventHandler));
		++WindowCount;
	}
}

void Window::Update() const
{
	UpdateWindow(window_handle);
}

void Window::Redraw() const
{
	RedrawWindow(window_handle, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
}

Window::~Window()
{
	if (!Closed)
	{
		DestroyWindow(window_handle);
		Closed = true;
		--WindowCount;
	}
}

unsigned int Window::GetHeight() const
{
	return height;
}

unsigned int Window::GetWidth() const
{
	return width;
}

bool Window::IsOpen() const
{
	return !Closed;
}

void Window::ChangeTitle(const std::string& title)
{
	SetWindowText(window_handle, title.c_str());
}

void Window::SetFocus()
{
	SetForegroundWindow(window_handle);
}

void Window::ProcessEvents(EventDispatcher e) const
{
	e(window_handle);
}

void Window::MainLoop(const Window* const window, EventDispatcher e)
{
	MSG msg;
	if (window != nullptr)
	{
		while (!window->Closed)
		{
			e(window->window_handle);
		}
	}
	else
	{
		while (WindowCount > 0)
		{
			e(nullptr);
		}
	}
}

void Window::DispatchWindowEvents(const HWND handle)
{
	MSG msg;
	if (GetMessage(&msg, handle, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::DispatchWindowEventsNonBlocking(const HWND handle)
{
	MSG msg;
	if (PeekMessage(&msg, handle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

unsigned int Window::GetWindowCount()
{
	return WindowCount;
}

int Window::Mouse::GetWheelDelta() const
{
	return wheelDelta;
}

bool Window::Mouse::IsLeftPressed() const
{
	return LeftPressed;
}

bool Window::Mouse::IsRightPressed() const
{
	return RightPressed;
}

int Window::Mouse::GetX() const
{
	return x;
}

int Window::Mouse::GetY() const
{
	return y;
}

std::pair<int, int> Window::Mouse::GetXY() const
{
	return {x , y};
}

void Window::Mouse::Reset()
{
	x = y = -1;
	LeftPressed = RightPressed = false;
}

bool Window::KeyBoard::IsKeyDown(unsigned char keycode) const
{
	return KEY_STAT[keycode];
}

bool Window::KeyBoard::IsRepeatEnabled() const
{
	return REPEAT_ENABLED;
}

void Window::KeyBoard::EnableKeyRepeat()
{
	REPEAT_ENABLED = true;
}

void Window::KeyBoard::DisableKeyRepeat()
{
	REPEAT_ENABLED = false;
}

void Window::KeyBoard::Reset()
{
	KEY_STAT.reset();
}

Window::KeyBoard::EventT::EventT(Window& wnd, unsigned char code, bool repeat) : window(wnd) , KEY_CODE(code) , IS_REPEATED(repeat)
{}

Window::Exception::Exception(HRESULT hr , std::source_location s) : code(hr)
{
	char * pcharBuff = nullptr;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&pcharBuff), 0, nullptr);
	message = pcharBuff;
	file = s.file_name();
	line = s.line();
	LocalFree(pcharBuff);
}

std::string Window::Exception::GetReason() const
{
	return message;
}

unsigned int Window::Exception::GetLine() const
{
	return line;
}

std::string Window::Exception::GetFile() const
{
	return file;
}

HRESULT Window::Exception::GetErrorCode() const
{
	return code;
}
