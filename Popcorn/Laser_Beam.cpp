#include "Laser_Beam.h"

// ALaser_Beam
AHit_Checker_List ALaser_Beam::Hit_Checker_List;
//------------------------------------------------------------------------------------------------------------
ALaser_Beam::ALaser_Beam()
	: Laser_Beam_State(ELaser_Beam_State::Disabled), X_Pos(0.0), Y_Pos(0.0), Speed(0.0), Beam_Rect{}
{
}
//------------------------------------------------------------------------------------------------------------

void ALaser_Beam::Begin_Movement()
{
	// This method is muted
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beam::Finish_Movement()
{
	if (Laser_Beam_State != ELaser_Beam_State::Disabled)
		Redraw_Beam();
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beam::Advance(double max_speed)
{
	double next_step;

	if (Laser_Beam_State != ELaser_Beam_State::Active)
		return;

	next_step = Speed / max_speed * AsConfig::Moving_Step_Size;

	Y_Pos -= next_step;

	if (Y_Pos < AsConfig::Level_Y_Offset)
		Stopping();

	if (Hit_Checker_List.Check_Hit(X_Pos, Y_Pos) )
		Stopping();
}
//------------------------------------------------------------------------------------------------------------
double ALaser_Beam::Get_Speed()
{
	return Speed;
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beam::Act()
{
	// This method is muted
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beam::Clear(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;

	if (Laser_Beam_State == ELaser_Beam_State::Disabled)
		return;

	if (Laser_Beam_State == ELaser_Beam_State::Cleanup)
		Laser_Beam_State = ELaser_Beam_State::Disabled;

	if (!IntersectRect(&intersection_rect, &paint_area, &Previous_Beam_Rect))
		return;

	AsConfig::BG_Color.Select(hdc);
	Rectangle(hdc, Previous_Beam_Rect.left, Previous_Beam_Rect.top, Previous_Beam_Rect.right - 1, Previous_Beam_Rect.bottom);
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beam::Draw(HDC hdc, RECT& paint_area)
{
	int x_pos, y_pos;
	RECT intersection_rect;

	if (Laser_Beam_State == ELaser_Beam_State::Disabled)
		return;

	if (Laser_Beam_State == ELaser_Beam_State::Stopping)
		Laser_Beam_State = ELaser_Beam_State::Cleanup;

	if (!IntersectRect(&intersection_rect, &paint_area, &Beam_Rect))
		return;

	AsConfig::Laser_Color.Select(hdc);

	x_pos = Beam_Rect.left + (Beam_Rect.right - Beam_Rect.left) / 2;
	y_pos = Beam_Rect.top;

	MoveToEx(hdc, x_pos, y_pos + 1, 0);
	LineTo(hdc, x_pos, y_pos + AsConfig::Platform_Height * AsConfig::Global_Scale - AsConfig::Global_Scale / 2 - 1);
}
//------------------------------------------------------------------------------------------------------------
bool ALaser_Beam::Is_Finished()
{
	return false; 	// This method is muted
}
//-----------------------------------------------------------------------------------------------------------
void ALaser_Beam::Set_At(double x_pos, double y_pos)
{
	X_Pos = x_pos;
	Y_Pos = y_pos;

	Laser_Beam_State = ELaser_Beam_State::Active;
	Speed = 10.0;

	Redraw_Beam();
}
//-----------------------------------------------------------------------------------------------------------
bool ALaser_Beam::Is_Active()
{
	if (Laser_Beam_State == ELaser_Beam_State::Active)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beam::Stopping()
{
	Laser_Beam_State = ELaser_Beam_State::Stopping;
	Speed = 0.0;
}
//------------------------------------------------------------------------------------------------------------
void ALaser_Beam::Redraw_Beam()
{
	double d_scale = AsConfig::D_Global_Scale;
	int scale = AsConfig::Global_Scale;

	Previous_Beam_Rect = Beam_Rect;


	Beam_Rect.left = (int)((X_Pos - (double)Width / 2.0) * d_scale);
	Beam_Rect.top = (int)(Y_Pos * d_scale);
	Beam_Rect.right = Beam_Rect.left + Width * scale;
	Beam_Rect.bottom = Beam_Rect.top + AsConfig::Platform_Height * scale;

	AsConfig::Invalidate_Rect(Beam_Rect);
	AsConfig::Invalidate_Rect(Previous_Beam_Rect);
}
//-----------------------------------------------------------------------------------------------------------