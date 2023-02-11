#pragma once

#include "Explosive_Ball.h"
#include "Gate.h"

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
	Emitting,
	Alive,
	Destroying
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

	void Activate(int x_pos, int y_pos, bool moving_right);
	bool Is_Active();
	void Destroy();

	static const int Width = 16;
	static const int Height = 16;

private:
	void Draw_Alive(HDC hdc);
	void Draw_Destroying(HDC hdc, RECT& paint_area);
	void Act_Alive();
	void Act_Destroying();
	void Get_Monster_Rect(double x_pos, double y_pos, RECT& rect);
	void Redraw_Monster();

	EEye_State Eye_State;
	EMonster_State Monster_State;
	double X_Pos, Y_Pos;
	double Speed, Direction;
	double Cornea_Height;
	int Start_Blinking_Time, Total_Animation_Time;
	int Next_Direction_Switch_Tick, Monster_Is_Alive_Timer;
	RECT Monster_Rect, Previous_Monster_Rect;

	static const int Blink_Stages_Count = 7;
	static const int Explosive_Balls_Count = 20;

	int Blink_Ticks[Blink_Stages_Count];
	AExplosive_Ball Explosive_Balls[Explosive_Balls_Count];

	static const double Max_Cornea_Height;
	static const double Blinking_Timeouts[Blink_Stages_Count];
	static const EEye_State Blinking_States[Blink_Stages_Count];
};
//------------------------------------------------------------------------------------------------------------