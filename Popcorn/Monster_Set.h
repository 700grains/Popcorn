#pragma once

#include "Border.h"

//------------------------------------------------------------------------------------------------------------
class AMonster: public AGame_Object
{
public:
	AMonster();

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Advance(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Activate(int x_pos,int y_pos);

	bool Is_Active;

private:
	int X_Pos, Y_Pos;
	double Cornea_Height;
	RECT Monster_Rect;

	static const int Width = 16;
	static const int Height = 16;
	static const double Max_Cornea_Height;
};
//------------------------------------------------------------------------------------------------------------
class AsMonster_Set : public AsGame_Objects_Set
{
public:
	AsMonster_Set();

	void Init(AsBorder* border);
	void Emit_At_Gate(int gate_index);

private:
	bool Get_Next_GameObject(int& index, AGame_Object** game_object);

	AsBorder *Border; // UNO
	static const int Max_Monsters_Count = 10;

	AMonster Monsters[Max_Monsters_Count];
};
//------------------------------------------------------------------------------------------------------------
