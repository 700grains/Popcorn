#include "Monster_Set.h"

// AMonster
//------------------------------------------------------------------------------------------------------------
void AMonster::Begin_Movement()
{
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Finish_Movement()
{
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Advance(double max_speed)
{
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
double AMonster::Get_Speed()
{
	//!!! TODO
	return 0.0;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Act()
{
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Clear(HDC hdc, RECT& paint_area)
{
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Draw(HDC hdc, RECT& paint_area)
{
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
bool AMonster::Is_Finished()
{
	//!!! TODO
	return false;
}
//------------------------------------------------------------------------------------------------------------




// AsMonster_Set
//------------------------------------------------------------------------------------------------------------
AsMonster_Set::AsMonster_Set()
	: Border(nullptr)
{
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Init(AsBorder* border)
{
	Border = border;
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Emit_At_Gate(int gate_index)
{
	int i;
	int gate_x, gate_y;

	AMonster* monster = 0;

	if (gate_index < 0 || gate_index >= AsConfig::Gates_Count)
		AsConfig::Throw();

	for (i = 0; i < Max_Monsters_Count; i++)
	{
		if (Monsters[i].Is_Active)
		{
			monster = &Monsters[i];
			break;
		}
	}

	if (monster == 0)
		return; // There are no unused monsters.

	Border->Get_Gate_Pos(gate_index, gate_x, gate_y);

	monster->Activate(gate_x, gate_y);
}
//------------------------------------------------------------------------------------------------------------
bool AsMonster_Set::Get_Next_GameObject(int& index, AGame_Object** game_object)
{
	if (index < 0 || index >= AsConfig::Max_Balls_Count)
		return false;

	*game_object = &Monsters[index++];

	return true;
}
//------------------------------------------------------------------------------------------------------------