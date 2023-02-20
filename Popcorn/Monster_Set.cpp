#include "Monster_Set.h"

// AsMonster_Set
//------------------------------------------------------------------------------------------------------------
AsMonster_Set::~AsMonster_Set()
{
	for (auto* monster : Monsters)
		delete monster;

	Monsters.erase(Monsters.begin(), Monsters.end());
}
//------------------------------------------------------------------------------------------------------------
AsMonster_Set::AsMonster_Set()
	: Monster_Set_State(EMonster_Set_State::Idle), Border(nullptr), Current_Gate_Index(-1), Max_Monsters_Alive(0)
{
}
//------------------------------------------------------------------------------------------------------------
bool AsMonster_Set::Check_Hit(double next_x_pos, double next_y_pos, ABall_Object* ball)
{
	for (auto monster : Monsters)
		if (monster->Check_Hit(next_x_pos, next_y_pos, ball) )
			return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsMonster_Set::Check_Hit(double next_x_pos, double next_y_pos)
{
	for (auto monster : Monsters)
		if (monster->Check_Hit(next_x_pos, next_y_pos) )
			return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsMonster_Set::Check_Hit(RECT& rect)
{
	for (auto monster : Monsters)
		if (monster->Check_Hit(rect))
			return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Act()
{
	int current_monsters_alive_count;
	switch (Monster_Set_State)
	{
	case EMonster_Set_State::Idle:
		break;


	case EMonster_Set_State::Selecting_Next_Gate:
		// We count living monsters
		current_monsters_alive_count = 0;

		for (auto monster : Monsters)
			if (monster->Is_Finished())
				++current_monsters_alive_count;

		// Add a monster if possible
		if (current_monsters_alive_count < Max_Monsters_Alive)
		{
			Current_Gate_Index = Border->Long_Open_Gate();
			Monster_Set_State = EMonster_Set_State::Waiting_For_Gate_To_Open;
		}
		break;


	case EMonster_Set_State::Waiting_For_Gate_To_Open:
		if (Border->Is_Gate_Opened(Current_Gate_Index))
		{
			Emit_At_Gate(Current_Gate_Index);
			Monster_Set_State = EMonster_Set_State::Waiting_For_Gate_To_Close;
		}
		break;


	case EMonster_Set_State::Waiting_For_Gate_To_Close:
		if (Border->Is_Gate_Closed(Current_Gate_Index))
			Monster_Set_State = EMonster_Set_State::Selecting_Next_Gate;
		break;


	default:
		AsConfig::Throw();
		break;
	}

	if (Monster_Set_State != EMonster_Set_State::Idle)
	{
		auto it = Monsters.begin();

		while (it != Monsters.end())
		{
			if ((*it)->Is_Finished())
			{
				delete* it;
				it = Monsters.erase(it);
			}
			else
				it++;
		}
	}

	AGame_Objects_Set::Act();
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Init(AsBorder* border)
{
	Border = border;
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Emit_At_Gate(int gate_index)
{
	bool gate_is_left;
	int i;
	int gate_x, gate_y;
	int monster_type;

	AMonster* monster = 0;

	if (gate_index < 0 || gate_index >= AsConfig::Gates_Count)
		AsConfig::Throw();

	for (i = 0; i < Max_Monsters_Count; i++)
	{
		if (Monsters[i] == 0)
		{
			monster_type = AsTools::Rand(2);

			if (monster_type == 1)
				monster = new AMonster_Comet();
			else
				monster = new AMonster_Eye();

			Monsters[i] = monster;
			break;
		}
	}

	if (monster == 0)
		return; // There are no unused monsters.

	Border->Get_Gate_Pos(gate_index, gate_x, gate_y);

	if (gate_index % 2 == 0)
		gate_is_left = true;
	else
		gate_is_left = false;

	if (! gate_is_left)
		gate_x -= monster->Width - AGate::Width;

	monster->Activate(gate_x, gate_y + 1, gate_is_left);
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Activate(int max_monsters_alive)
{
	Monster_Set_State = EMonster_Set_State::Selecting_Next_Gate;
	Max_Monsters_Alive = max_monsters_alive;
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Destroy_All()
{
	int i;

	for (i = 0; i < Max_Monsters_Count; i++)
		if (Monsters[i] != 0 && ! Monsters[i]->Is_Finished())
			Monsters[i]->Destroy();

	Monster_Set_State = EMonster_Set_State::Idle;
}
//------------------------------------------------------------------------------------------------------------
bool AsMonster_Set::Get_Next_Game_Object(int& index, AGame_Object** game_object)
{
	AMonster* monster;

	if (index < 0 || index >= Max_Monsters_Count)
		return false;

	while (index < Max_Monsters_Count)
	{
		monster = Monsters[index++];

		if (monster != 0)
		{
			*game_object = monster;

			return true;
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------