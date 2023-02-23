#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <vector>

//------------------------------------------------------------------------------------------------------------
enum class EBall_State : unsigned char
{
	Disabled, // not drawn, not moved, doesn't interact

	Normal,
	Lost,
	On_Platform,
	On_Parachute,
	Off_Parachute,
	Teleporting
};
//------------------------------------------------------------------------------------------------------------
class ABall_Object
{
public:
	virtual double Get_Direction() = 0;
	virtual void Set_Direction(double new_direction) = 0;
	virtual EBall_State Get_State() = 0;
	virtual void Set_State(EBall_State new_state, double x_pos = 0, double y_pos = 0) = 0;
	virtual void Reflect(bool from_horizontal) = 0;
	virtual void Get_Center(double& x_pos, double& y_pos) = 0;
	virtual void Draw_Teleporting(HDC hdc, int step) = 0;
	virtual void Set_On_Parachute(int brick_x, int brick_y) = 0;
	virtual bool Is_Moving_Up() = 0;
	virtual bool Is_Moving_Left() = 0;
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
class AGame_Object: public AMover, public AGraphics_Object
{
};
//------------------------------------------------------------------------------------------------------------
class AGame_Objects_Set: public AGame_Object
{
public:
	virtual void Begin_Movement() override;
	virtual void Finish_Movement() override;
	virtual void Advance(double max_speed) override;
	virtual double Get_Speed() override;

	virtual void Act() override;
	virtual void Clear(HDC hdc, RECT& paint_area) override;
	virtual void Draw(HDC hdc, RECT& paint_area) override;
	virtual bool Is_Finished() override;

protected:
	virtual bool Get_Next_Game_Object(int &index, AGame_Object **game_object) = 0;
};
//------------------------------------------------------------------------------------------------------------
class AString
{
public:
	~AString();
	AString();
	AString(const wchar_t* str);

	void Append(int value);
	const wchar_t* Get_Content();
	int Get_Length();

private:
	std::wstring Content;
};
//------------------------------------------------------------------------------------------------------------
enum class EMessage_Type : unsigned char
{
	Floor_Is_Over
};
//------------------------------------------------------------------------------------------------------------
class AMessage
{
public:
	AMessage(EMessage_Type message_type);

	const EMessage_Type Message_Type;
};
//------------------------------------------------------------------------------------------------------------