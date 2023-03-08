#include "Final_Letter.h"

// AFinal_Letter
//------------------------------------------------------------------------------------------------------------
AFinal_Letter::AFinal_Letter(double x_pos, double y_pos, const wchar_t letter)
	: Letter(letter), X_Pos(x_pos), Y_Pos(y_pos)
{
}
//------------------------------------------------------------------------------------------------------------
void AFinal_Letter::Act()
{
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
void AFinal_Letter::Clear(HDC hdc, RECT& paint_area)
{
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
void AFinal_Letter::Draw(HDC hdc, RECT& paint_area)
{
	const double d_scale = AsConfig::D_Global_Scale;

	//Letter.Draw(hdc);

	SetBkMode(hdc, 0);
	AsConfig::Game_Over_Font.Select(hdc);
	SetTextColor(hdc, AsConfig::White_Color.Get_RGB());

	TextOut(hdc, (int)(X_Pos * d_scale), (int)(Y_Pos * d_scale), &Letter, 1);

}
//------------------------------------------------------------------------------------------------------------
bool AFinal_Letter::Is_Finished()
{
	//!!! TODO
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AFinal_Letter::Destroy()
{

}
//------------------------------------------------------------------------------------------------------------