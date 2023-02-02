#pragma once

#include "Ball.h"

//------------------------------------------------------------------------------------------------------------
enum class EGate_State : unsigned char
{
	Closed,
	Partially_Open,
	Fully_Open
};
//------------------------------------------------------------------------------------------------------------
enum class EGate_Transformation : unsigned char
{
	Unknown,

	Init,
	Active,
	Finalize,
};
//------------------------------------------------------------------------------------------------------------
class AGate : public AGraphics_Object
{
public:
	AGate(int x_pos, int y_pos);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Open_Gate(bool is_partially);
private:
	bool Act_For_Partially_Open();
	void Draw_Cup(HDC hdc, bool is_top);
	void Draw_Edges(HDC hdc);
	void Draw_Single_Edge(HDC hdc, int edge_y_offset, bool is_long);
	void Redraw_Gate();

	EGate_State Gate_State;
	EGate_Transformation Gate_Transformation;
	int X_Pos, Y_Pos;
	int Edges_Count;
	int Gate_Close_Timer;
	double Hole_Height;

	RECT Gate_Rect;

	static const double Max_Hole_Short_Height;
	static const double Hole_Height_Short_Step;
	static const int Width = 6;
	static const int Height = 19;
	static const int Short_Opening_Timeout = AsConfig::FPS; // 1 second
};
//------------------------------------------------------------------------------------------------------------
class AsBorder : public AHit_Checker, public AGraphics_Object
{
public:
	~AsBorder();
	AsBorder();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Redraw_Floor();
	void Open_Gate(int gate_index, bool is_partially);
	bool Is_Gate_Opened(int gate_index);

private:
	void Draw_Element(HDC hdc, RECT& paint_area, int x, int y, bool top_border);
	void Draw_Floor(HDC hdc, RECT& paint_area);

	RECT Floor_Rect;
	AGate* Gates[AsConfig::Gates_Count];
};
//------------------------------------------------------------------------------------------------------------