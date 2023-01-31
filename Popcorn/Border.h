﻿#pragma once

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
	void Draw_Cup(HDC hdc, bool is_top);
	void Draw_Edges(HDC hdc);
	void Draw_Single_Edge(HDC hdc, int edge_y_offset, bool is_long);

	EGate_State Gate_State;
	EGate_Transformation Gate_Transformation;
	int X_Pos, Y_Pos;
	int Edges_Count;

	RECT Gate_Rect;

	static const int Width = 6;
	static const int Height = 19;
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

private:
	void Draw_Element(HDC hdc, RECT& paint_area, int x, int y, bool top_border);
	void Draw_Floor(HDC hdc, RECT& paint_area);

	RECT Floor_Rect;
	AGate* Gates[AsConfig::Gates_Count];
};
//------------------------------------------------------------------------------------------------------------