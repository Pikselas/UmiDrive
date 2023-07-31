#pragma once
#include"CustomWindow.h"

class ImageWindow : public CustomWindow
{
private:
	const HANDLE hImage;
public:
	ImageWindow(CustomWindow& Parent ,const HANDLE hImage  , int x , int y , int width , int height)
		:
	CustomWindow(&Parent , 0 , "" , WS_CHILD , x , y , width , height),
	hImage(hImage)
	{}
protected:
	LRESULT CALLBACK EventHandler(HWND handle, UINT msgcode, WPARAM wparam, LPARAM lparam) override
	{
		switch (msgcode)
		{
			case WM_PAINT:
			{
				BITMAP bm;
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(handle, &ps);		// start painting window
				GetObject(hImage, sizeof(bm), &bm);		// get bitmap details
				HDC hdcMem = CreateCompatibleDC(hdc);	// create a memory device context
				SelectObject(hdcMem, hImage);			// select the bitmap into the device context

				// paint on actual window (with scaling)
				StretchBlt(hdc, 0, 0, GetWidth(), GetHeight(), hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
				
				DeleteDC(hdcMem);
				EndPaint(handle, &ps);
				return 0;
			}
		}
		return CustomWindow::EventHandler(handle, msgcode, wparam, lparam);
	}
};