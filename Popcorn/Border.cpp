﻿#include "Border.h"

// AsBorder
//------------------------------------------------------------------------------------------------------------
AsBorder::AsBorder()
{
	Floor_Rect.left = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
	Floor_Rect.right = AsConfig::Max_X_Pos * AsConfig::Global_Scale;
	Floor_Rect.top = (AsConfig::Max_Y_Pos - 1) * AsConfig::Global_Scale;
	Floor_Rect.bottom = AsConfig::Max_Y_Pos * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw(HDC hdc, RECT &paint_area)
{// Draws a level border.

	int i;

	// 1. Left line
	for (i = 0; i < 50; i++)
		Draw_Element(hdc, 2, 1 + i * 4, false);

	// 2. Right line
	for (i = 0; i < 50; i++)
		Draw_Element(hdc, 201, 1 + i * 4, false);

	// 3. Top line
	for (i = 0; i < 50; i++)
		Draw_Element(hdc, 3 + i * 4, 0, true);
	// 4. if(Floor)
	if (AsConfig::Level_Has_Floor)
		if (IntersectRect)
		{

		}
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Redraw_Floor()
{
	InvalidateRect(AsConfig::Hwnd, &Floor_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{
	bool got_hit = false;

	// Correcting the position when reflected from the frame
	if (next_x_pos - ball->Radius < AsConfig::Border_X_Offset)
	{
		got_hit = true;
		ball->Reflect(false);
	}

	if (next_y_pos - ball->Radius < AsConfig::Border_Y_Offset)
	{
		got_hit = true;
		ball->Reflect (true);
	}

	if (next_x_pos + ball->Radius > AsConfig::Max_X_Pos)
	{
		got_hit = true;
		ball->Reflect(false);
	}

	if (next_y_pos + ball->Radius > AsConfig::Max_Y_Pos)
	{
		if (AsConfig::Level_Has_Floor)
		{
			got_hit = true;
			ball->Reflect(true);
		}
		else
		{
			if (next_y_pos + ball->Radius > (double)AsConfig::Max_Y_Pos + ball->Radius * 4.0) // We check the position below the visible border.
				ball->Set_State (EBS_Lost);
		}
	}

	return got_hit;
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border)
{// Draws a level border element

 // The main line
	AsConfig::Blue_Color.Select(hdc);

	if (top_border)
		Rectangle(hdc, x * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale - 1, (y + 4) * AsConfig::Global_Scale - 1);
	else
		Rectangle(hdc, (x + 1) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale - 1, (y + 4) * AsConfig::Global_Scale - 1);

	// White border
	AsConfig::White_Color.Select(hdc);

	if (top_border)
		Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale - 1, (y + 1) * AsConfig::Global_Scale - 1);
	else
		Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 1) * AsConfig::Global_Scale - 1, (y + 4) * AsConfig::Global_Scale - 1);

	// Perforation
	AsConfig::BG_Color.Select(hdc);


	if (top_border)
		Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale - 1, (y + 3) * AsConfig::Global_Scale - 1);
	else
		Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale - 1, (y + 2) * AsConfig::Global_Scale - 1);
}
//------------------------------------------------------------------------------------------------------------
