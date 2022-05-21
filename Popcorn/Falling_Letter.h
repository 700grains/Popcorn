#pragma once
#include "Active_Brick.h"

//------------------------------------------------------------------------------------------------------------
enum ELetter_Type
{
	ELT_O, // "Cancel"
	ELT_I, // "Inversion"
	ELT_C, // "Speed"
	ELT_M, // "Monsters"
	ELT_G, // "Life"
	ELT_K, // "Glue"
	ELT_W, // "Wider"

	ELT_T, // "Three"
	ELT_L, // "Laser"
	ELT_P, // "Floor"

	ELT_Plus, // Moving to the next level

	ELT_Max // Max value
};
//------------------------------------------------------------------------------------------------------------
enum EFalling_Letter_State
{
	EFLS_Normal,
	EFLS_Finalizing, // Let's start deleting the object
	EFLS_Finished // Object can be deleted
};
//------------------------------------------------------------------------------------------------------------
class AFalling_Letter : public AGraphics_Object
{
public:
	AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Get_Letter_Cell(RECT& rect);
	void Finalize();
	void Test_Draw_All_Steps(HDC hdc);

	static void init();
	static ELetter_Type Get_Random_Letter_Type();

	const EBrick_Type Brick_Type;
	const ELetter_Type Letter_Type;

private:
	void Set_Brick_Letter_Colors(bool is_switch_color, const AColor** front_color, const AColor** back_color);
	void Draw_Brick_Letter(HDC hdc);
	void Draw_C(HDC hdc);
	void Draw_Line(HDC hdc, int x_1, int y_1, int x_2, int y_2);
	void Draw_Line_To(HDC hdc, int x, int y);

	EFalling_Letter_State Falling_Letter_State;
	int X, Y;
	int Rotation_Step;
	int Next_Rotation_Tick;
	RECT Letter_Cell, Prev_Letter_Cell;

	static const int Ticks_Per_Step = 4;
	static const int Max_Rotation_Step = 16;
	static const int Brick_Half_Height = AsConfig::Brick_Height * AsConfig::Global_Scale / 2;

	static int All_Letters_Popularity;
	static int Letters_Popularity[ELT_Max]; // "Weight" of each letter
};
//------------------------------------------------------------------------------------------------------------
