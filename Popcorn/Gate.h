#pragma once

#include "Config.h"

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
	bool Is_Opened();
	bool Is_Closed();
	void Get_Y_Size(int& gate_top_y, int& gate_bot_y);
	void Get_Pos(int &gate_x,int &gate_y);

	static const int Width = 6;
	static const int Height = 19;

private:
	bool Act_For_Open(bool Partially_Open, bool& correct_pos);
	void Draw_Cup(HDC hdc, bool is_top);
	void Draw_Edges(HDC hdc);
	void Draw_Long_Opening_Edges(HDC hdc);
	void Draw_Short_Opening_Edges(HDC hdc);
	void Draw_Single_Edge(HDC hdc, int edge_y_offset, bool is_long);
	void Draw_Red_Edge(HDC hdc, int edge_y_offset, bool is_long, bool highlight);
	void Draw_Electrical_Charge(HDC hdc);
	void Redraw_Gate();

	EGate_State Gate_State;
	EGate_Transformation Gate_Transformation;
	int X_Pos;
	double Y_Pos, Original_Y_Pos;
	int Edges_Count;
	int Gate_Close_Timer;
	double Hole_Height;

	RECT Gate_Rect;

	static const double Max_Hole_Short_Height;
	static const double Hole_Height_Short_Step;
	static const double Max_Hole_Long_Height;
	static const double Hole_Height_Long_Step;
	static const int Short_Opening_Timeout = AsConfig::FPS; // 1 second
	static const int Long_Opening_Timeout = AsConfig::FPS * 3; // 3 seconds
};
//------------------------------------------------------------------------------------------------------------
