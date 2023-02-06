#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class AsMonster_Set : public AsGame_Objects_Set
{
private:
	bool Get_Next_GameObject(int& index, AGame_Object** game_object);

	static const int Max_Monsters_Count = 10;

	AMonster Monsters[Max_Monsters_Count];
};
//------------------------------------------------------------------------------------------------------------
