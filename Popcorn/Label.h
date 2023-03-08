#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class ALabel
{
public:
	ALabel(int x_pos, int y_pos, int height, int width, const AFont& font, const AColor& color);

	void Draw(HDC hdc);
	AString Content;
	RECT Content_Rect;

private:
	int X_Pos, Y_Pos;
	int Width, Height;
	const AFont& Font;
	const AColor& Color;
};
//------------------------------------------------------------------------------------------------------------