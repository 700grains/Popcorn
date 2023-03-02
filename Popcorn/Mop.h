#pragma once
#include "Mop_Indicator.h"
#include "Mop_Cylinder.h"

//------------------------------------------------------------------------------------------------------------
enum class EMop_State : unsigned char
{
	Idle,

	Cleaning,
	Clean_Done,
	Showing,
	Show_Done
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

	void Activate(bool clearing);
	void Clear_Area(HDC hdc);
	EMop_State Get_Mop_State();

private:
	void Set_Mop();

	EMop_State Mop_State;
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