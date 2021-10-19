#include "Active_Brick.h"

//AActive_Brick
//------------------------------------------------------------------------------------------------------------
AActive_Brick::AActive_Brick()
	: Fade_Step(0)
{

}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick::Act(HWND hwnd)
{
	if (Fade_Step < Max_Fade_Step)
	{
		++Fade_Step;
		InvalidateRect(hwnd, &Brick_Rect, FALSE);
	}
}

void AActive_Brick::Draw(HDC hdc, RECT& paint_area)
{
	unsigned char r, g, b;
	RECT brick_rect;
	HPEN pen;
	HBRUSH brush;

// 	r = AsConfig::Blue_Brick_Color.R - Fade_Step * (AsConfig::Blue_Brick_Color.R / Max_Fade_Step);
// 	g = AsConfig::Blue_Brick_Color.G - Fade_Step * (AsConfig::Blue_Brick_Color.G / Max_Fade_Step);
// 	b = AsConfig::Blue_Brick_Color.B - Fade_Step * (AsConfig::Blue_Brick_Color.B / Max_Fade_Step);
// 
// 	AsConfig::Create_Pen_Brush(r, g, b, pen, brush);

	pen = Blue_Pens[Fade_Step];
	brush = Blue_Brushes[Fade_Step];

	SelectObject(hdc, pen);
	SelectObject(hdc, brush);

	Brick_Rect.left = (AsConfig::Level_X_Offset + AsConfig::Cell_Width) * AsConfig::Global_Scale;
	Brick_Rect.top = (AsConfig::Level_Y_Offset + AsConfig::Cell_Height) * AsConfig::Global_Scale;
	Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
	Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

	RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right, Brick_Rect.bottom, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);


}
//------------------------------------------------------------------------------------------------------------
