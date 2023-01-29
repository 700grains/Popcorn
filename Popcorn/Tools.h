#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

//------------------------------------------------------------------------------------------------------------
class ABall;
class AHit_Checker
{
public:
	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball) = 0;
	virtual bool Check_Hit(double next_x_pos, double next_y_pos);

	bool Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius, double& x);
};
//------------------------------------------------------------------------------------------------------------
class AMover
{
public:
	virtual ~AMover();

	virtual void Begin_Movement() = 0;
	virtual void Finish_Movement() = 0;
	virtual void Advance(double max_speed) = 0;
	virtual double Get_Speed() = 0;
};
//------------------------------------------------------------------------------------------------------------
class AGraphics_Object
{
public:
	virtual ~AGraphics_Object();

	virtual void Act() = 0;
	virtual void Clear(HDC hdc, RECT& paint_area) = 0;
	virtual void Draw(HDC hdc, RECT& paint_area) = 0;
	virtual bool Is_Finished() = 0;
};
//------------------------------------------------------------------------------------------------------------
