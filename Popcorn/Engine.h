#pragma once

#include <Windows.h>

#include "Border.h"
#include "Level.h"

//------------------------------------------------------------------------------------------------------------
enum EKey_Type
{
	EKT_Left,
	EKT_Right,
	EKT_Space
};
//------------------------------------------------------------------------------------------------------------
const int Timer_ID = WM_USER + 1;
//------------------------------------------------------------------------------------------------------------
class AsEngine;
class ALevel;
class AsPlatform;
class ABall
{
public:
	ABall();

	void init();

	void Draw(HDC hdc, RECT &paint_area, AsEngine *engine);
	void Move(AsEngine *engine, ALevel* level, AsPlatform* platform);

	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	double Ball_Direction;

	static const int Ball_Size = 4;

private:
	int Ball_X_Pos, Ball_Y_Pos;
	double Ball_Speed;

	RECT Ball_Rect, Prev_Ball_Rect;
};
//------------------------------------------------------------------------------------------------------------
class AsPlatform
{
public:
	AsPlatform();

	void init();
	void Redraw_Platform(AsEngine* engine);
	void Draw(HDC hdc, AsEngine* engine, RECT& paint_area);

	int X_Pos;
	int Width;
	int X_Step;

	static const int Y_Pos = 185;

private:
	int Inner_Width;

	RECT Platform_Rect, Prev_Platform_Rect; \

	HPEN Highlight_Pen, Platform_Circle_Pen, Platform_Inner_Pen;
	HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;

	static const int Height = 7;
	static const int Circle_Size = 7;

};
//------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	AsEngine();

	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key_Down(EKey_Type key_type);
	int On_Timer();

	HWND Hwnd;
	HPEN BG_Pen;
	HBRUSH BG_Brush;

	static const int Max_X_Pos = ALevel::Level_X_Offset + ALevel::Cell_Width * ALevel::Level_Width;
	static const int Max_Y_Pos = 199 - ABall::Ball_Size;

private:
	ABall Ball;
	ALevel Level;
	AsPlatform Platform;
	AsBorder Border;
};
//------------------------------------------------------------------------------------------------------------
