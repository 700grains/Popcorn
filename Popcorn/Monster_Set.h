#pragma once

#include "Border.h"

//------------------------------------------------------------------------------------------------------------
enum class EEye_State : unsigned char
{
	Closed,
	Opening,
	Staring,
	Closing
};
//------------------------------------------------------------------------------------------------------------
enum class EMonster_State : unsigned char
{
	Missing,
	Alive,
	Destroying
};
//------------------------------------------------------------------------------------------------------------
enum class EExplosive_Ball_State : unsigned char
{
	Idle,
	Expanding,
	Fading
};
//------------------------------------------------------------------------------------------------------------
class AExplosive_Ball : public AGraphics_Object
{
public:
	AExplosive_Ball();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Explode(int x_pos, int y_pos, int size, int step_count);

private:
	EExplosive_Ball_State Explosive_Ball_State;

	int X_Pos, Y_Pos;
	int Step_Count;
	double Max_Size, Size, Size_Step;
	RECT Ball_Rect;
};
//------------------------------------------------------------------------------------------------------------
class AMonster : public AGame_Object
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
	bool Is_Active();
	void Destroy();

private:
	void Draw_Alive(HDC hdc);
	void Draw_Destroying(HDC hdc, RECT& paint_area);

	EEye_State Eye_State;
	EMonster_State Monster_State;
	int X_Pos, Y_Pos;
	double Cornea_Height;
	int Start_Blinking_Time, Total_Animation_Time;
	RECT Monster_Rect;

	static const int Blink_Stages_Count = 7;
	static const int Explosive_Balls_Count = 10;

	int Blink_Ticks[Blink_Stages_Count];
	AExplosive_Ball Explosive_Balls[Explosive_Balls_Count];

	static const int Width = 16;
	static const int Height = 16;
	static const double Max_Cornea_Height;
	static const double Blinking_Timeouts[Blink_Stages_Count];
	static const EEye_State Blinking_States[Blink_Stages_Count];
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
