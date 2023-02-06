#include "Laser_Beam_Set.h"

// AsLaser_Beam_Set
//------------------------------------------------------------------------------------------------------------
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
bool AsLaser_Beam_Set::Get_Next_GameObject(int& index, AGame_Object** game_object)
{
	if (index < 0 || index >= AsConfig::Max_Balls_Count)
		return false;

	*game_object = &Laser_Beams[index++];

	return true;
}
//------------------------------------------------------------------------------------------------------------