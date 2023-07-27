#pragma once
#include"CustomWindow.h"
#include"WindowComponent.h"

class Label : public WindowComponent
{
public:
	Label(CustomWindow& Parent, const std::string& text, int x, int y, int width, int height)
		:
	WindowComponent(Parent , 0 , "STATIC" , text , 0 , x , y , width , height)
	{}
};