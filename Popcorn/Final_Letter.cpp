#include "Final_Letter.h"

// AFinal_Letter
//------------------------------------------------------------------------------------------------------------
AFinal_Letter::AFinal_Letter(double x_pos, double y_pos, const wchar_t letter, int width, int height)
	: Exploding(false), Finished(false), Letter(letter), X_Pos(x_pos), Y_Pos(y_pos), Width(width), Height(height)
{
}
//------------------------------------------------------------------------------------------------------------
void AFinal_Letter::Act()
{
	Finished = Act_On_Explosion();

	AsTools::Invalidate_Rect(Final_Letter_Rect);
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

	if (Exploding)
		Draw_Explosion(hdc,paint_area);
	else
	{
		SetBkMode(hdc, 0);
		AsConfig::Game_Over_Font.Select(hdc);
		SetTextColor(hdc, AsConfig::White_Color.Get_RGB());

		TextOut(hdc, (int)(X_Pos * d_scale), (int)(Y_Pos * d_scale), &Letter, 1);
	}
}
//------------------------------------------------------------------------------------------------------------
bool AFinal_Letter::Is_Finished()
{
	return Finished;
}
//------------------------------------------------------------------------------------------------------------
void AFinal_Letter::Destroy()
{
	const int scale = AsConfig::Global_Scale;
	const double d_scale = AsConfig::D_Global_Scale;

	Final_Letter_Rect.left = (int)(X_Pos * d_scale);
	Final_Letter_Rect.top = (int)(Y_Pos * d_scale);
	Final_Letter_Rect.right = Final_Letter_Rect.left + Width * scale;
	Final_Letter_Rect.bottom = Final_Letter_Rect.top + Height * scale;

	Start_Explosion(Final_Letter_Rect);
	Exploding = true;
	AsTools::Invalidate_Rect(Final_Letter_Rect);
}
//------------------------------------------------------------------------------------------------------------