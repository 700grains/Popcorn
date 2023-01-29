﻿#pragma once

#include "Falling_Letter.h"
#include "Ball_Set.h"
#include "Laser_Beam_Set.h"

//------------------------------------------------------------------------------------------------------------
enum class EPlatform_State : unsigned char
{
	Unknown,

	Regular,
	Meltdown,
	Rolling,
	Glue,
	Expanding,
	Laser,
};
//------------------------------------------------------------------------------------------------------------
enum class EPlatform_Substate_Regular : unsigned char
{
	Unknown,

	Missing,
	Normal,
	Ready,
};
//------------------------------------------------------------------------------------------------------------
enum class EPlatform_Substate_Meltdown : unsigned char
{
	Unknown,
	
	Init,
	Active,

};
//------------------------------------------------------------------------------------------------------------
enum class EPlatform_Substate_Rolling : unsigned char
{
	Unknown,

	Roll_In,
	Expand_Roll_In,
};
//------------------------------------------------------------------------------------------------------------
enum class EPlatform_Transformation : unsigned char
{
	Unknown,

	Init,
	Active,
	Finalize,
};
//------------------------------------------------------------------------------------------------------------
enum class EPlatform_Moving_State : unsigned char
{
	Stopping, 
	Stop,
	Moving_Left,
	Moving_Right
};
//------------------------------------------------------------------------------------------------------------
enum class EFigure_Type: unsigned char
{
	Ellipse,
	Rectangle,
	Round_Rect_3x
};
//------------------------------------------------------------------------------------------------------------
class AsPlatform_State
{
public:
	AsPlatform_State();

	operator EPlatform_State() const;
	void operator = (EPlatform_State new_state);

	void Set_Next_State(EPlatform_State next_state);
	EPlatform_State Get_Next_State();
	EPlatform_State Set_State(EPlatform_Substate_Regular new_regular_state);
	EPlatform_State Set_Next_Or_Regular_State(EPlatform_Substate_Regular new_regular_state);


	EPlatform_Substate_Regular Regular;
	EPlatform_Substate_Meltdown Meltdown;
	EPlatform_Substate_Rolling Rolling;
	EPlatform_Transformation Glue;
	EPlatform_Transformation Expanding;
	EPlatform_Transformation Laser;

	EPlatform_Moving_State Moving;

private:
	EPlatform_State Current_State;
	EPlatform_State Next_State; // We enter this state from AsPlatform::Set_State(EPlatform_Substate_Regular new_regular_state)
};
//------------------------------------------------------------------------------------------------------------
class AsPlatform_Glue
{
public:
	AsPlatform_Glue(AsPlatform_State &platform_state);

	bool Act(AsBall_Set* ball_set, EPlatform_State& next_state);
	void Draw(HDC hdc, double x_pos);
	void Reset();

private:
	void Draw_Glue_Spot(HDC hdc, int x_offset, int width, int height, double x_pos);

	double Glue_Spot_Height_Ratio;
	AsPlatform_State* Platform_State;

	static const double Max_Glue_Spot_Height_Ratio, Min_Glue_Spot_Height_Ratio, Glue_Spot_Height_Ratio_Step;

};
//------------------------------------------------------------------------------------------------------------
class AsPlatform_Expanding
{
public:
	~AsPlatform_Expanding();
	AsPlatform_Expanding(AsPlatform_State& platform_state);

	void Init(AColor& highlight_color, AColor& circle_color, AColor& inner_color);
	bool Act(double &x_pos, EPlatform_State& next_state, bool& correct_pos);
	void Draw_State(HDC hdc, double& x);
	void Draw_Circle_Highlight(HDC hdc, int x, int y);
	void Reset();

	double Expanding_Platform_Width;
private:
	void Draw_Expanding_Platform_Ball(HDC hdc, double x, bool is_left);
	void Draw_Expanding_Truss(HDC hdc, RECT& inner_rect, bool is_left);
	
	AsPlatform_State* Platform_State;
	AColor* Inner_Color, * Highlight_Color, * Circle_Color; // UNO, Use, Not Own!
	AColor *Truss_Color;

	static const double Min_Expanding_Platform_Width, Max_Expanding_Platform_Width, Expanding_Platform_Width_Step;
};
//------------------------------------------------------------------------------------------------------------
class AsPlatform_Laser
{
public:
	~AsPlatform_Laser();
	AsPlatform_Laser(AsPlatform_State &platform_state);

	void Init(AsLaser_Beam_Set *laser_beam_set, AColor &highlight_color, AColor& circle_color, AColor& inner_color);
	bool Act(EPlatform_State& next_state, double x_pos);
	void Draw_State(HDC hdc, double x_pos, RECT& platform_rect);
	void Reset();
	void Fire(bool fire_on);

private:
	void Draw_Laser_Wing(HDC hdc, double x_pos, bool is_left);
	void Draw_Laser_Inner_Part(HDC hdc, double x);
	void Draw_Laser_Leg(HDC hdc, double x_pos, bool is_left);
	void Draw_Laser_Cabin(HDC hdc, double x);

	void Draw_Expanding_Figure(HDC hdc, EFigure_Type figure_type, double start_x, double start_y, double start_width, double start_height, double ratio, double end_x, double end_y, double end_width, double end_height);
	int Get_Expanding_Value(double start, double end, double ratio);
	double Get_Gun_Pos(double platform_x_pos, bool is_left);

	bool Enable_Laser_Firing;
	AsPlatform_State* Platform_State;
	AColor* Inner_Color, * Circle_Color; // UNO, Use, Not Own!
	AColor* Gun_Color;

	AsLaser_Beam_Set *Laser_Beam_Set; // UNO
	int Last_Laser_Shot_Time;
	int Laser_Transformation_Step;

	static const int Max_Laser_Transformation_Step = 20;
	static const int Laser_Shot_Timeout = AsConfig::FPS / 2; // currently 20 FPS, so laser can shoot each 0.5 seconds
};
//------------------------------------------------------------------------------------------------------------
class AsPlatform : public AHit_Checker, public AMover, public AGraphics_Object
{
public:
	~AsPlatform();
	AsPlatform();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball);

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Advance(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Init(AsBall_Set *ball_set, AsLaser_Beam_Set *laser_beam_set);
	EPlatform_State Get_State();
	void Set_State(EPlatform_State new_state);
	void Set_State(EPlatform_Substate_Regular new_regular_state);
	bool Has_State(EPlatform_Substate_Regular regular_state);
	void Redraw_Platform();
	void Move(bool to_left, bool key_down);
	void On_Space_Key(bool key_down);
	bool Hit_By(AFalling_Letter* falling_letter);
	double Get_Middle_Pos();

	double X_Pos;

	static const int Height = 7;
	static const int Circle_Size = 7;
	static const int Normal_Width = 28;
	static const int Normal_Platform_Inner_Width = Normal_Width - Circle_Size;
	static const int Expanding_Platform_Inner_Width = 12;


private:
	bool Set_Transformation_State(EPlatform_State new_state, EPlatform_Transformation& transformation_state);
	void Act_For_Meltdown_State();
	void Act_For_Rolling_State();

	void Draw_Circle_Highlight(HDC hdc, int x, int y);
	void Draw_Normal_State(HDC hdc, RECT &paint_area);
	void Draw_Meltdown_State(HDC hdc, RECT &paint_area);
	void Draw_Rolling_State(HDC hdc, RECT& paint_area);
	void Draw_Roll_In_State(HDC hdc, RECT& paint_area);
	void Draw_Expanding_Figure(HDC hdc, EFigure_Type figure_type, double start_x, double start_y, double start_width, double start_height, double ratio, double end_x, double end_y, double end_width, double end_height);
	int Get_Expanding_Value(double start, double end, double ratio);
	bool Reflect_On_Circle(double next_x_pos, double next_y_pos, double platform_ball_x_offset, ABall *ball);
	bool Get_Platform_Image_Stroke_Color(int x, int y, const AColor **color, int &stroke_len);
	void Get_Normal_Platform_Image(HDC hdc);
	double Get_Current_Width();
	bool Correct_Platform_Pos();

	AsPlatform_State Platform_State;
	bool Right_Key_Down, Left_Key_Down;
	int Inner_Width;
	int Rolling_Step;
	int Last_Redraw_Timer_Tick;
	double Speed;
	AsBall_Set* Ball_Set;
	AsPlatform_Glue Platform_Glue;
	AsPlatform_Expanding Platform_Expanding;
	AsPlatform_Laser Platform_Laser;

	int Normal_Platform_Image_Width, Normal_Platform_Image_Height;
	int* Normal_Platform_Image; // Platform image pixels on window background

	int Meltdown_Platform_Y_Pos[Normal_Width * AsConfig::Global_Scale];

	RECT Platform_Rect, Prev_Platform_Rect;

	AColor Highlight_Color, Platform_Circle_Color, Platform_Inner_Color;

	static const int Meltdown_Speed = 3;
	static const int Max_Rolling_Step = 16;
	static const int Roll_In_Platform_End_X_Pos = 99;
	static const int Rolling_Platform_Speed = 10; // was 3
	static const int X_Step = 6;

};
//------------------------------------------------------------------------------------------------------------
