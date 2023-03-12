#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class ALabel
{
public:
	ALabel(int x_pos, int y_pos, int height, int width, const AFont& font, const AColor& color);

	void Draw(HDC hdc);
	bool Append(wchar_t symbol);

	RECT Content_Rect;
	AString Content;

private:
	wchar_t Last_Character_Entered;
	int X_Pos, Y_Pos;
	int Width, Height;
	const AFont& Font;
	const AColor& Color;
};
//------------------------------------------------------------------------------------------------------------