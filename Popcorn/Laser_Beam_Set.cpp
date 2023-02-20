#include "Laser_Beam_Set.h"

// AsLaser_Beam_Set
//------------------------------------------------------------------------------------------------------------
AsLaser_Beam_Set::AsLaser_Beam_Set()
	: Laser_Beams(Max_Laser_Beam_Count)
{
}
//------------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Fire(double left_gun_x_pos, double right_gun_x_pos)
{
	ALaser_Beam* left_beam = nullptr, * right_beam = nullptr;

	for (auto &laser_beam : Laser_Beams)
		{
			if (laser_beam.Is_Active())
				continue;

			if (left_beam == nullptr)
				left_beam = &laser_beam;
			else
				if (right_beam == nullptr)
				{
					right_beam = &laser_beam;
					break;
				}
		}

	if (left_beam == nullptr || right_beam == nullptr)
		AsConfig::Throw(); // Not enough free laser beams in the array

	left_beam->Set_At(left_gun_x_pos, AsConfig::Platform_Y_Pos - 1);
	right_beam->Set_At(right_gun_x_pos, AsConfig::Platform_Y_Pos - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Disable_All()
{
	for (auto& laser_beam : Laser_Beams)
		laser_beam.Stopping();
}
//------------------------------------------------------------------------------------------------------------
bool AsLaser_Beam_Set::Get_Next_Game_Object(int& index, AGame_Object** game_object)
{
	if (index < 0 || index >= Laser_Beams.size())
		return false;

	*game_object = &Laser_Beams[index++];

	return true;
}
//------------------------------------------------------------------------------------------------------------