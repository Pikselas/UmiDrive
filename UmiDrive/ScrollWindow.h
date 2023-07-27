#pragma once
#include"CustomWindow.h"

class ScrollVerticalWindow : public CustomWindow
{
public:
	ScrollVerticalWindow( CustomWindow& Parent , int x , int y , int width , int height , int scrollSize , int maxSize)
		:
	 CustomWindow(&Parent , 0 , "" , WS_CHILD | WS_VSCROLL , x , y , width , height)
	{
		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
		si.nMin = 0;
		si.nMax = maxSize; 
		si.nPage = scrollSize;  // Set the visible area size
		si.nPos = 0;
		SetScrollInfo(window_handle, SB_VERT, &si, TRUE);
	}
protected:
	LRESULT EventHandler(HWND handle, UINT msgcode, WPARAM wparam, LPARAM lparam) override
	{
        switch (msgcode)
        {
            case WM_VSCROLL: 
            {
                SCROLLINFO si;
                si.cbSize = sizeof(SCROLLINFO);
                si.fMask = SIF_ALL;
                GetScrollInfo(window_handle, SB_VERT, &si);

                int yPrevOffset = si.nPos;

                switch (LOWORD(wparam))
                {
                case SB_LINEUP:
                    si.nPos -= 10;
                    break;
                case SB_LINEDOWN:
                    si.nPos += 10;
                    break;
                case SB_PAGEUP:
                    si.nPos -= si.nPage;
                    break;
                case SB_PAGEDOWN:
                    si.nPos += si.nPage;
                    break;
                case SB_THUMBTRACK:
                {
                    int yTrackPos = HIWORD(wparam);
                    si.nPos = yTrackPos;
                }
                break;
                default:
                    break;
                }

                // Ensure that position remains within bounds
                si.nPos = max(0, min(si.nPos, si.nMax));
                SetScrollInfo(window_handle, SB_VERT, &si, TRUE);
                struct Desc
                {
                    HWND h;
                    int offset;
                };
                Desc desc;
                desc.h = handle;
                desc.offset = si.nPos - yPrevOffset;
                EnumChildWindows(window_handle, [](HWND childWnd, LPARAM lparam)->BOOL 
                {
                    Desc* d = reinterpret_cast<Desc*>(lparam);
                    RECT rectChild;
                    GetWindowRect(childWnd, &rectChild);
                    ScreenToClient( d->h, (LPPOINT)&rectChild);
                    SetWindowPos(childWnd, nullptr, rectChild.left, rectChild.top - d->offset, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                    return TRUE;
                }, reinterpret_cast<LPARAM>(&desc));

                // Redraw the child windows
                InvalidateRect(window_handle, NULL, TRUE);
            }
        }
		return CustomWindow::EventHandler(handle, msgcode, wparam, lparam);
	}
};