#pragma once
#include"CustomWindow.h"

class ScrollVerticalWindow : public CustomWindow
{
public:
	ScrollVerticalWindow( CustomWindow& Parent , int x , int y , int width , int height , int maxSize)
		:
	 CustomWindow(&Parent , 0 , "" , WS_CHILD | WS_VSCROLL | WS_BORDER , x , y , width , height)
	{
		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
		si.nMin = 0;
		si.nMax = maxSize; 
		si.nPage = height;  // Set the visible area size
		si.nPos = 0;
		SetScrollInfo(window_handle, SB_VERT, &si, TRUE);
	}
protected:
	LRESULT CALLBACK EventHandler(HWND handle, UINT msgcode, WPARAM wparam, LPARAM lparam) override
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
                si.nPos = max(0, min(si.nPos, si.nMax - si.nPage));

                if (si.nPos != yPrevOffset)
				{
					ScrollWindow(window_handle, 0, yPrevOffset - si.nPos, NULL, NULL);
				}

                si.fMask = SIF_POS;
                SetScrollInfo(window_handle, SB_VERT, &si, TRUE);
				return 0;
            }
        }
		return CustomWindow::EventHandler(handle, msgcode, wparam, lparam);
	}
};