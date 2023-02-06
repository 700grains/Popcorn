#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class AMonster: public AGame_Object
{
public:
	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Advance(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();
};
//------------------------------------------------------------------------------------------------------------
class AsMonster_Set : public AsGame_Objects_Set
{
private:
	bool Get_Next_GameObject(int& index, AGame_Object** game_object);

	static const int Max_Monsters_Count = 10;

	AMonster Monsters[Max_Monsters_Count];
};
//------------------------------------------------------------------------------------------------------------
