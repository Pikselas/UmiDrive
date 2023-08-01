#pragma once
#include"CustomWindow.h"

class TextEntry : public WindowComponent
{
public:
	TextEntry(CustomWindow& Parent, int x, int y, int width, int height, const std::string& default_text = "")
		:
		WindowComponent(Parent, WS_EX_CLIENTEDGE, "EDIT", default_text, ES_LEFT | ES_AUTOHSCROLL, x, y, width, height)
	{}
public:
	std::string GetText() const
	{
		std::vector<char> text(GetWindowTextLength(window_handle) + 1 , 0);
		int length = GetWindowText(window_handle, (LPSTR)text.data(), text.size());
		return std::string(text.begin() , text.begin() + length);
	}
};