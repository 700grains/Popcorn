#include "Mop.h"

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

	for (i = 0; i < 4; i++)
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
	RECT intersection_rect;

	if (!Acting)
		return;

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
void AsMop::Clear_Area(HDC hdc)
{
	const int scale = AsConfig::Global_Scale;
	RECT rect;

	if (!Acting)
		return;

	rect = Mop_Rect;
	rect.bottom = AsConfig::Max_Y_Pos * scale;

	AsTools::Rect(hdc, rect, AsConfig::BG_Color);
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