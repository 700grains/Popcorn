#include "Mop_Cylinder.h"

// AMop_Cylinders
const int AMop_Cylinders::Max_Cylinder_Height[4] = { 88, 47, 37, 15 };
//------------------------------------------------------------------------------------------------------------
AMop_Cylinders::AMop_Cylinders(int x_pos, int y_pos, int width, int max_height)
	:X_Pos(x_pos), Y_Pos(y_pos), Width(width), Height(Min_Height), Max_Height(max_height)
{
}
//------------------------------------------------------------------------------------------------------------
void AMop_Cylinders::Act()
{
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
void AMop_Cylinders::Clear(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Previous_Cylinder_Rect))
		return;

	AsTools::Rect(hdc, Previous_Cylinder_Rect, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
void AMop_Cylinders::Draw(HDC hdc, RECT& paint_area)
{
	const int scale = AsConfig::Global_Scale;
	RECT rect, intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Cylinder_Rect))
		return;

	// 1. cylinder mount
	rect.left = X_Pos * scale;
	rect.top = Y_Pos * scale;
	rect.right = rect.left + Width * scale;
	rect.bottom = rect.top + Binding_Height * scale;

	AsConfig::Red_Color.Select(hdc);

	AsTools::Round_Rect(hdc, rect);
	AsTools::Rect(hdc, X_Pos + 2, Y_Pos + 1, 1, Binding_Height - 2, AsConfig::BG_Color);
	AsTools::Rect(hdc, X_Pos + 4, Y_Pos + 1, 1, Binding_Height - 2, AsConfig::BG_Color);
	AsTools::Rect(hdc, X_Pos + Width - 5, Y_Pos + 1, 1, Binding_Height - 2, AsConfig::BG_Color);
	AsTools::Rect(hdc, X_Pos + Width - 3, Y_Pos + 1, 1, Binding_Height - 2, AsConfig::BG_Color);

	// 2. cylinder body
	AsTools::Rect(hdc, X_Pos + 2, Y_Pos + Binding_Height, 2, Height - Binding_Height, AsConfig::White_Color);
	AsTools::Rect(hdc, X_Pos + 4, Y_Pos + Binding_Height, 1, Height - Binding_Height, AsConfig::Blue_Color);
	AsTools::Rect(hdc, X_Pos + 5, Y_Pos + Binding_Height, 1, Height - Binding_Height, AsConfig::White_Color);
	AsTools::Rect(hdc, X_Pos + 6, Y_Pos + Binding_Height, Width - 8, Height - Binding_Height, AsConfig::Blue_Color);
}
//------------------------------------------------------------------------------------------------------------
bool AMop_Cylinders::Is_Finished()
{
	return false;
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
void AMop_Cylinders::Set_Height_For(double ratio)
{
	Height = (int)((double)Max_Height * ratio);

	if (Height < Min_Height)
		Height = Min_Height;
	else
		if (Height > Max_Height)
			Height = Max_Height;
}
//------------------------------------------------------------------------------------------------------------
int AMop_Cylinders::Get_Height()
{
	return Height;
}
//------------------------------------------------------------------------------------------------------------
void AMop_Cylinders::Set_Y_Pos(int y_pos)
{
	const int scale = AsConfig::Global_Scale;

	Y_Pos = y_pos;

	Previous_Cylinder_Rect = Cylinder_Rect;

	Cylinder_Rect.left = X_Pos * scale;
	Cylinder_Rect.top = Y_Pos * scale;
	Cylinder_Rect.right = Cylinder_Rect.left + Width * scale;
	Cylinder_Rect.bottom = Cylinder_Rect.top + Height * scale;

	AsTools::Invalidate_Rect(Cylinder_Rect);
	AsTools::Invalidate_Rect(Previous_Cylinder_Rect);
}
//------------------------------------------------------------------------------------------------------------