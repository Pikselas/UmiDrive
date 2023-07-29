#pragma once
#include"Label.h"
#include"ImageWindow.h"

class ImageButton : protected CustomWindow
{
	private:
		Label title;
		ImageWindow icon;
	public:
		ImageButton(CustomWindow& Parent, const std::string& title, const HANDLE icon, int x, int y, int width, int height)
			:
		CustomWindow(&Parent , 0 , "", WS_CHILD | WS_BORDER , x , y , width , height),
		icon(*this, icon, 0, 0, width, (float)(height) * (2.0 / 3.0)),
		title(*this, title, 0, (float) height * (2.0f / 3.0f), width, height / 3)
		{

		}	
};