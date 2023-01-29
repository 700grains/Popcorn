#include "Laser_Beam_Set.h"




// AsLaser_Beam_Set
//------------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Begin_Movement()
{
	int i;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		Laser_Beams[i].Begin_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Finish_Movement()
{
	int i;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		Laser_Beams[i].Finish_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Advance(double max_speed)
{
	int i;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		Laser_Beams[i].Advance(max_speed);
}
//------------------------------------------------------------------------------------------------------------
double AsLaser_Beam_Set::Get_Speed()
{
	int i;
	double max_speed = 0.0, current_speed;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		current_speed = Laser_Beams[i].Get_Speed();

		if (current_speed > max_speed)
			max_speed = current_speed;
	}

	return max_speed;
}
//------------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Act()
{
	// Stub. Not used, because the laser beam itself does nothing (no animation).
}
//------------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Clear(HDC hdc, RECT& paint_area)
{
	int i;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		Laser_Beams[i].Clear(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Draw(HDC hdc, RECT& paint_area)
{
	int i;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		Laser_Beams[i].Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
bool AsLaser_Beam_Set::Is_Finished()
{
	return false; // Stub. This method is not used.
}
//-----------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Fire(double left_gun_x_pos, double right_gun_x_pos)
{
	int i;
	ALaser_Beam* left_beam = nullptr, * right_beam = nullptr;

	for (i = 0; i < Max_Laser_Beam_Count; ++i)
	{
		if (Laser_Beams[i].Is_Active())
			continue;

		if (left_beam == nullptr)
			left_beam = &Laser_Beams[i];
		else
			if (right_beam == nullptr)
			{
				right_beam = &Laser_Beams[i];
				break;
			}
	}

	if (left_beam == nullptr || right_beam == nullptr)
		AsConfig::Throw(); // Not enough free laser beams in the array

	left_beam->Set_At(left_gun_x_pos, AsConfig::Platform_Y_Pos - 1);
	right_beam->Set_At(right_gun_x_pos, AsConfig::Platform_Y_Pos - 1);
}
//------------------------------------------------------------------------------------------------------------




