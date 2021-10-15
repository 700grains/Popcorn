#pragma once
#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();

	void init();

	void Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush);
	void Move(HWND hwnd, ALevel* level, int platform_x_pos, int platform_width);

	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	double Ball_Direction;

	static const int Ball_Size = 4;

private:
	int Ball_X_Pos, Ball_Y_Pos;
	double Ball_Speed;

	RECT Ball_Rect, Prev_Ball_Rect;
};
//------------------------------------------------------------------------------------------------------------
