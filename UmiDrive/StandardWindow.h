#pragma once
#include"Window.h"

class StandardWindow : public Window
{
private:
	class WindowClass
	{
	private:
		constexpr static const char* classNm = "PikselasWindowClass";
		WindowClass();
		~WindowClass();
		static WindowClass wndcls;
		HINSTANCE hinst;
	public:
		constexpr static const char* GetName();
		static HINSTANCE GetInstance();
	};
public:
	StandardWindow(const std::string& Title = "Window", unsigned int Width = 800, unsigned int Height = 600 , StandardWindow* Parent = nullptr);
};