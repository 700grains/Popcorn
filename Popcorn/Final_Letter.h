#pragma once

#include "Explosion.h"

//------------------------------------------------------------------------------------------------------------
enum class EFinal_Letter_State : unsigned char
{
	Showing_Letter,
	Hiding_Letter,
	Exploding,
	Finished
};
//------------------------------------------------------------------------------------------------------------
class AFinal_Letter : public AGraphics_Object, public AExplosion
{
public:
	AFinal_Letter(double x_pos, double y_pos, const wchar_t letter, int width = 24, int height = 24);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Destroy();

	double X_Pos, Y_Pos;

private:
	void Draw_Letter(HDC hdc, bool is_colored);

	bool Got_Letter_Size;
	EFinal_Letter_State Final_Letter_State;
	wchar_t Letter;
	int Width, Height;
	RECT Final_Letter_Rect;
};
//------------------------------------------------------------------------------------------------------------