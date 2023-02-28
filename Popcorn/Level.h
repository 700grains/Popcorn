#pragma once

#include "Falling_Letter.h"
#include "Information_Panel.h"
#include "Level_Data.h"

//------------------------------------------------------------------------------------------------------------
class APoint
{
public:
	APoint();
	APoint(int x, int y);

	int X, Y;
};
//------------------------------------------------------------------------------------------------------------
class  AMop_Cylinders : public AGraphics_Object
{
public:
	AMop_Cylinders(int x_pos, int y_pos, int width, int height, int max_height);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Set_Height_For(double ratio);

	int Y_Pos;
	static const int Max_Cylinder_Height[4];

private:
	int X_Pos;
	int Width, Height;
	int Max_Height;

};
//------------------------------------------------------------------------------------------------------------
class AMop_Indicators : public AGraphics_Object
{
public:
	AMop_Indicators(int x_pos, int y_pos, int time_offset);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Set_Y_Pos(int y_pos);

private:
	int X_Pos, Y_Pos;
	RECT Indicator_Rect;
	const AColor* Current_Color;
	int Time_Offset;

	static const int Width = 17;
	static const int Height = 5;
	static const int Max_Fade_Step = AsConfig::FPS * 4 / 10; // 0.4 seconds 
	static const int Normal_Timeout = AsConfig::FPS; // 1 sec

	static AColor_Fade Fading_Blue_Colors;
};
//------------------------------------------------------------------------------------------------------------
class AsMop : public AGame_Object
{
public:
	~AsMop();
	AsMop();

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Advance(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Erase_Level();

private:
	void Set_Mop();

	bool Acting;
	int Y_Pos;
	int Starting_Tick;
	std::vector <AMop_Indicators*> Mop_Indicators;
	std::vector <AMop_Cylinders*> Mop_Cylinders;

	static const int Width = (AsConfig::Level_Width - 1) * AsConfig::Cell_Width + AsConfig::Brick_Width;
	static const int Height = AsConfig::Brick_Height;
	static const int Expansion_Timeout = AsConfig::FPS * 5;
};
//------------------------------------------------------------------------------------------------------------
class AsLevel : public AHit_Checker, public AGame_Object
{
public:
	~AsLevel();
	AsLevel();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall_Object* ball);
	virtual bool Check_Hit(double next_x_pos, double next_y_pos);

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Advance(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Init();
	void Set_Current_Level(int level_number);
	bool Get_Next_Falling_Letter(int &index, AFalling_Letter** falling_letter);
	void Stop();

	static bool Has_Brick_At(int level_x, int level_y);
	static bool Has_Brick_At(RECT& monster_rect);

private: 
	bool On_Hit(int brick_x, int brick_y, ABall_Object* ball, bool vertical_hit);
	void Redraw_Brick(int brick_x, int brick_y);
	bool Add_Falling_Letter(int brick_x, int brick_y, EBrick_Type brick_type);
	bool Create_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type, ABall_Object* ball, bool vertical_hit);
	void Add_Active_Brick_Teleport(int brick_x, int brick_y, ABall_Object* ball, bool vertical_hit);
	AActive_Brick_Teleport* Select_Destination_Teleport(int source_x, int source_y);
	bool Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall_Object* ball, double &reflection_pos);
	bool Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall_Object* ball, double &reflection_pos);
	void Draw_Brick(HDC hdc, RECT &brick_rect, int level_x, int level_y);
	void Draw_Parachute_In_Level(HDC hdc, RECT& brick_rect);
	void Draw_Parachute_Part(HDC hdc, RECT& brick_rect, int offset, int width);
	void Delete_Objects(std::vector<AGraphics_Object*>& falling_letters);
	void Act_Objects(std::vector<AGraphics_Object*>& falling_letters);
	void Cancel_All_Activity();


	RECT Level_Rect;
	bool Need_To_Cancel_All;

	double Current_Brick_Left_X, Current_Brick_Right_X;
	double Current_Brick_Top_Y, Current_Brick_Low_Y;

	AColor Parachute_Color;

	char Current_Level[AsConfig::Level_Height][AsConfig::Level_Width];

	std::vector <AGraphics_Object*> Active_Bricks;
	std::vector <AGraphics_Object*> Falling_Letters;
	std::vector<APoint> Teleport_Bricks_Pos;
	std::vector <ALevel_Data*> Levels_Data;

	AAdvertisement* Advertisement;
	AsMop Mop; // A mop clearing current level

	static AsLevel* Level;
};
//------------------------------------------------------------------------------------------------------------