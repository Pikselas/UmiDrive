#pragma once
#include"Window.h"

class WindowComponent : public Window
{
	friend class CustomWindow;
public:
	WindowComponent(Window& Parent, DWORD exStyle, const std::string& window_class, const std::string& title, DWORD Style, int x, int y, int width, int height)
		:
		Window(&Parent, exStyle, window_class, title, Style | WS_CHILD, x, y, width, height)
	{
		SetWindowLongPtr(window_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	}
protected:
	virtual void PerformDefaultAction(WPARAM) {}
};