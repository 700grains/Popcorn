#pragma once

#include "Config.h"

class  AMop_Cylinders : public AGraphics_Object
{
public:
	AMop_Cylinders(int x_pos, int y_pos, int width, int max_height);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Set_Height_For(double ratio);
	int Get_Height();

	void Set_Y_Pos(int y_pos);

	static const int Max_Cylinder_Height[4];

private:
	int X_Pos, Y_Pos;
	int Width, Height;
	int Max_Height;
	RECT Cylinder_Rect, Previous_Cylinder_Rect;

	static const int Binding_Height = 4;
	static const int Min_Height = Binding_Height + 1;
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
	RECT Mop_Rect, Previous_Mop_Rect;
	std::vector <AMop_Indicators*> Mop_Indicators;
	std::vector <AMop_Cylinders*> Mop_Cylinders;

	static const int Width = (AsConfig::Level_Width - 1) * AsConfig::Cell_Width + AsConfig::Brick_Width;
	static const int Height = AsConfig::Brick_Height;
	static const int Expansion_Timeout = AsConfig::FPS * 2;
};
//------------------------------------------------------------------------------------------------------------