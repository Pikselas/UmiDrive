#pragma once
#include"WindowComponent.h"
#include"CustomWindow.h"

class TextButton : public WindowComponent
{
public:
	std::function<void(TextButton& button)> OnClick = nullptr;
	std::function<void(TextButton& button)> OnDblClick = nullptr;
public:
	TextButton(CustomWindow& Parent, const std::string& text, int x, int y, int width, int height)
		:
		WindowComponent(Parent, 0, "Button", text, 0, x, y, width, height)
	{}
protected:
	void PerformDefaultAction(WPARAM wparam) override
	{
		if (HIWORD(wparam) == BN_CLICKED && OnClick != nullptr)
		{
			OnClick(*this);
		}
		else if (HIWORD(wparam) == BN_DBLCLK && OnDblClick != nullptr)
		{
			OnDblClick(*this);
		}
	}
};