#pragma once
#include "Mop_Indicator.h"
#include "Mop_Cylinder.h"

//------------------------------------------------------------------------------------------------------------
enum class EMop_State : unsigned char
{
	Idle,

	Ascending,
	Cleaning,
	Clean_Done,
	Showing,
	Descending,
	Descend_Done
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
	bool Is_Cleaning_Done();
	bool Is_Mopping_Done();

private:
	void Act_Lifting(bool ascending);
	void Set_Mop();
	int Get_Cylinders_Height();

	bool Mopping_Is_Done;
	EMop_State Mop_State;
	int Y_Pos, Max_Y_Pos;
	int Lifting_Height;
	int Starting_Tick;
	RECT Mop_Rect, Previous_Mop_Rect;
	std::vector <AMop_Indicators*> Mop_Indicators;
	std::vector <AMop_Cylinders*> Mop_Cylinders;

	static const int Width = (AsConfig::Level_Width - 1) * AsConfig::Cell_Width + AsConfig::Brick_Width;
	static const int Height = AsConfig::Brick_Height;
	static const int Expansion_Timeout = AsConfig::FPS * 2;
	static const int Ascending_Timeout = AsConfig::FPS;
};
//------------------------------------------------------------------------------------------------------------