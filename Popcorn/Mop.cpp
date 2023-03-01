#include "Mop.h"

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
	AsTools::Rect(hdc, X_Pos + Width - 3, Y_Pos + 1, 1, Binding_Height - 2, AsConfig::BG_Color);
	AsTools::Rect(hdc, X_Pos + Width - 5, Y_Pos + 1, 1, Binding_Height - 2, AsConfig::BG_Color);

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




// AMop_Indicators
AColor_Fade AMop_Indicators::Fading_Blue_Colors(AsConfig::Blue_Color, AsConfig::Red_Color, Max_Fade_Step);
//------------------------------------------------------------------------------------------------------------
AMop_Indicators::AMop_Indicators(int x_pos, int y_pos, int time_offset)
	: X_Pos(x_pos), Y_Pos(y_pos), Time_Offset(time_offset), Current_Color(&AsConfig::Red_Color)
{
	Set_Y_Pos(Y_Pos);
}
//------------------------------------------------------------------------------------------------------------
void AMop_Indicators::Act()
{
	int total_timeout = Normal_Timeout + Max_Fade_Step;
	int current_tick = (AsConfig::Current_Timer_Tick + Time_Offset) % total_timeout;
	int current_offset;

	if (current_tick < Normal_Timeout)
		Current_Color = &AsConfig::Red_Color;
	else
	{
		current_offset = current_tick - Normal_Timeout;

		if (current_offset < 0 || current_offset >= Max_Fade_Step)
			AsConfig::Throw();

		Current_Color = Fading_Blue_Colors.Get_Color(current_offset);
	}

	AsTools::Invalidate_Rect(Indicator_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AMop_Indicators::Clear(HDC hdc, RECT& paint_area)
{
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
void AMop_Indicators::Draw(HDC hdc, RECT& paint_area)
{
	const int scale = AsConfig::Global_Scale;
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Indicator_Rect))
		return;


	AsTools::Rect(hdc, Indicator_Rect, *Current_Color);

	// Indicator frame
	AsConfig::Highlight_Color.Select_Pen(hdc);
	MoveToEx(hdc, (X_Pos)*scale, (Y_Pos + Height) * scale, 0);
	LineTo(hdc, X_Pos * scale, Y_Pos * scale);
	LineTo(hdc, (X_Pos + Width) * scale, (Y_Pos)*scale);

	AsConfig::Shadow_Color.Select_Pen(hdc);
	MoveToEx(hdc, (X_Pos + Width) * scale, Y_Pos * scale, 0);
	LineTo(hdc, (X_Pos + Width) * scale, (Y_Pos + Height) * scale);
	LineTo(hdc, X_Pos * scale, (Y_Pos + Height) * scale);
}
//------------------------------------------------------------------------------------------------------------
bool AMop_Indicators::Is_Finished()
{
	return false;
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
void AMop_Indicators::Set_Y_Pos(int y_pos)
{
	const int scale = AsConfig::Global_Scale;

	Y_Pos = y_pos;

	Indicator_Rect.left = X_Pos * scale;
	Indicator_Rect.top = Y_Pos * scale;
	Indicator_Rect.right = Indicator_Rect.left + Width * scale;
	Indicator_Rect.bottom = Indicator_Rect.top + Height * scale;
}
//------------------------------------------------------------------------------------------------------------




// AsMop
//------------------------------------------------------------------------------------------------------------
AsMop::~AsMop()
{
	for (auto* indicator : Mop_Indicators)
		delete indicator;

	Mop_Indicators.erase(Mop_Indicators.begin(), Mop_Indicators.end());

	for (auto* cylinder : Mop_Cylinders)
		delete cylinder;

	Mop_Cylinders.erase(Mop_Cylinders.begin(), Mop_Cylinders.end());
}
//------------------------------------------------------------------------------------------------------------
AsMop::AsMop()
	:Y_Pos(0), Starting_Tick(0), Acting(false)
{
	int i;
	int x_pos, y_pos;
	AMop_Indicators* indicator;
	AMop_Cylinders* cylinder;

	for (i = 0; i < 10; i++)
	{
		indicator = new AMop_Indicators(AsConfig::Level_X_Offset + 1 + i * 19, AsConfig::Level_Y_Offset + 1, i * 80);
		Mop_Indicators.push_back(indicator);
	}

	for (i = 0; i < 5; i++)
	{
		x_pos = AsConfig::Level_X_Offset + Width / 2 - 6 - i;
		y_pos = AsConfig::Level_Y_Offset + 7 + i * 5;

		cylinder = new AMop_Cylinders(x_pos, y_pos, 13 + i * 2, AMop_Cylinders::Max_Cylinder_Height[i]);
		Mop_Cylinders.push_back(cylinder);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsMop::Begin_Movement()
{
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
void AsMop::Finish_Movement()
{
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
void AsMop::Advance(double max_speed)
{
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
double AsMop::Get_Speed()
{
	return 0.0;
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
void AsMop::Act()
{
	int time_offset;
	double ratio;

	if (!Acting)
		return;

	Previous_Mop_Rect = Mop_Rect;

	time_offset = AsConfig::Current_Timer_Tick - Starting_Tick;

	if (time_offset <= Expansion_Timeout)
	{
		ratio = (double)time_offset / (double)Expansion_Timeout;

		for (auto* cylinder : Mop_Cylinders)
			cylinder->Set_Height_For(ratio);

		Set_Mop();
	}

	for (auto* indicator : Mop_Indicators)
		indicator->Act();

	AsTools::Invalidate_Rect(Mop_Rect);
	AsTools::Invalidate_Rect(Previous_Mop_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AsMop::Clear(HDC hdc, RECT& paint_area)
{
	if (!Acting)
		return;

	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Previous_Mop_Rect))
		return;

	AsTools::Rect(hdc, Previous_Mop_Rect, AsConfig::BG_Color);

	for (auto* cylinder : Mop_Cylinders)
		cylinder->Clear(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AsMop::Draw(HDC hdc, RECT& paint_area)
{
	if (!Acting)
		return;

	AsTools::Rect(hdc, AsConfig::Level_X_Offset, Y_Pos, Width, Height, AsConfig::Red_Color);

	for (auto* indicator : Mop_Indicators)
		indicator->Draw(hdc, paint_area);

	for (auto* cylinder : Mop_Cylinders)
		cylinder->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
bool AsMop::Is_Finished()
{
	return false;
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
void AsMop::Erase_Level()
{
	Starting_Tick = AsConfig::Current_Timer_Tick;
	Y_Pos = 172;

	Acting = true;

	Set_Mop();
}
//------------------------------------------------------------------------------------------------------------
void AsMop::Set_Mop()
{
	int i;
	int current_y_pos = 0;
	int total_cylinder_height = 0;
	const int scale = AsConfig::Global_Scale;

	for (auto* cylinder : Mop_Cylinders)
		total_cylinder_height += cylinder->Get_Height();

	Y_Pos = AsConfig::Max_Y_Pos - total_cylinder_height - Height + 1;

	for (auto* indicator : Mop_Indicators)
		indicator->Set_Y_Pos(Y_Pos + 1);

	for (i = 0; i < (int)Mop_Cylinders.size(); i++)
	{
		Mop_Cylinders[i]->Set_Y_Pos(Y_Pos + Height + current_y_pos);

		current_y_pos += Mop_Cylinders[i]->Get_Height();
	}

	Mop_Rect.left = AsConfig::Level_X_Offset * scale;
	Mop_Rect.top = Y_Pos * scale;
	Mop_Rect.right = Mop_Rect.left + Width * scale;
	Mop_Rect.bottom = Mop_Rect.top + Height * scale;
}
//------------------------------------------------------------------------------------------------------------