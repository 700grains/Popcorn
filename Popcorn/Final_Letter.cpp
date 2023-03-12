#include "Final_Letter.h"

// AFinal_Letter
//------------------------------------------------------------------------------------------------------------
AFinal_Letter::AFinal_Letter(double x_pos, double y_pos, const wchar_t letter, int width, int height)
	: Got_Letter_Size(false), Final_Letter_State(EFinal_Letter_State::Showing_Letter), Letter(letter), X_Pos(x_pos), Y_Pos(y_pos), Width(width), Height(height),
	Final_Letter_Rect{}
{
	Color.Set_As(AsConfig::White_Color.R, AsConfig::White_Color.G, AsConfig::White_Color.B);
}
//------------------------------------------------------------------------------------------------------------
void AFinal_Letter::Act()
{
	if (Final_Letter_State == EFinal_Letter_State::Exploding)
	{
		if (Act_On_Explosion())
			Final_Letter_State = EFinal_Letter_State::Finished;
	}

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
	SIZE letter_size;

	switch (Final_Letter_State)
	{
	case EFinal_Letter_State::Showing_Letter:
		Draw_Letter(hdc, true);

		if (!Got_Letter_Size)
		{
			GetTextExtentPoint32(hdc, &Letter, 1, &letter_size);

			Width = letter_size.cx + letter_size.cx / 3;
			Height = letter_size.cy;

			Got_Letter_Size = true;
		}
		break;

	case EFinal_Letter_State::Hiding_Letter:
		Draw_Letter(hdc, false);
		Final_Letter_State = EFinal_Letter_State::Exploding;
		break;

	case EFinal_Letter_State::Exploding:
		Draw_Explosion(hdc, paint_area);
		break;

	case EFinal_Letter_State::Color_Letter:
		Draw_Letter(hdc, true);
		break;

	case EFinal_Letter_State::Finished:
		break;

	default:
		AsConfig::Throw();
		break;
	}
}
//------------------------------------------------------------------------------------------------------------
bool AFinal_Letter::Is_Finished()
{
	return Final_Letter_State == EFinal_Letter_State::Finished;
}
//------------------------------------------------------------------------------------------------------------
void AFinal_Letter::Destroy()
{
	Setup_Letter_Rect();

	Start_Explosion(Final_Letter_Rect);
	Final_Letter_State = EFinal_Letter_State::Hiding_Letter;
	AsTools::Invalidate_Rect(Final_Letter_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AFinal_Letter::Set_Color(unsigned char r, unsigned char g, unsigned char b)
{
	if (Final_Letter_State != EFinal_Letter_State::Color_Letter)
	{
		Final_Letter_State = EFinal_Letter_State::Color_Letter;
		Setup_Letter_Rect();
	}

	Color.Set_As(r, g, b);
	AsTools::Invalidate_Rect(Final_Letter_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AFinal_Letter::Setup_Letter_Rect()
{
	const double d_scale = AsConfig::D_Global_Scale;

	Final_Letter_Rect.left = (int)(X_Pos * d_scale);
	Final_Letter_Rect.top = (int)(Y_Pos * d_scale);
	Final_Letter_Rect.right = Final_Letter_Rect.left + Width;
	Final_Letter_Rect.bottom = Final_Letter_Rect.top + Height;
}
//------------------------------------------------------------------------------------------------------------
void AFinal_Letter::Draw_Letter(HDC hdc, bool is_colored)
{
	const double d_scale = AsConfig::D_Global_Scale;
	
	SetBkMode(hdc, 0);
	AsConfig::Game_Over_Font.Select(hdc);

	if (is_colored)
		SetTextColor(hdc, Color.Get_RGB());
	else
		SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());

	TextOut(hdc, (int)(X_Pos * d_scale), (int)(Y_Pos * d_scale), &Letter, 1);

}
//------------------------------------------------------------------------------------------------------------