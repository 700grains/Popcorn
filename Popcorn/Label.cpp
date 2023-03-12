#include "Label.h"

// ALabel
//------------------------------------------------------------------------------------------------------------
ALabel::ALabel(int x_pos, int y_pos, int width, int height, const AFont& font, const AColor& color)
	: Last_Character_Entered(0), X_Pos(x_pos), Y_Pos(y_pos), Height(height), Width(width), Font(font), Color(color)
{
	const int scale = AsConfig::Global_Scale;

	Content_Rect.left = X_Pos * scale;
	Content_Rect.top = Y_Pos * scale;
	Content_Rect.right = Content_Rect.left + Width * scale;
	Content_Rect.bottom = Content_Rect.top + Height * scale;
}
//------------------------------------------------------------------------------------------------------------
void ALabel::Draw(HDC hdc)
{
	const int scale = AsConfig::Global_Scale;
	int str_left_offset, str_top_offset;
	SIZE str_size;


	// 2. Draw string
	SetBkMode(hdc, 0);
	Font.Select(hdc);

	GetTextExtentPoint32(hdc, Content.Get_Content(), Content.Get_Length(), &str_size); 	//Calculate the length of the string in the window with the player's name

	if (str_size.cx > Width * scale)
	{
		if (Last_Character_Entered != 0)
		{
			Content.Delete_Last_Symbol();
			Last_Character_Entered = 0;
			GetTextExtentPoint32(hdc, Content.Get_Content(), Content.Get_Length(), &str_size); 	//Calculate the length of the string in the window with the player's name
		}
	}

	str_left_offset = Content_Rect.left + (Content_Rect.right - Content_Rect.left) / 2 - str_size.cx / 2;
	str_top_offset = Content_Rect.top + (Content_Rect.bottom - Content_Rect.top) / 2 - str_size.cy / 2 - scale;

	// 2.1 Draw shadow
	SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
	TextOut(hdc, str_left_offset + 2 * scale, str_top_offset + 2 * scale, Content.Get_Content(), Content.Get_Length());

	// 2.2 Draw the string
	SetTextColor(hdc, Color.Get_RGB());
	TextOut(hdc, str_left_offset, str_top_offset, Content.Get_Content(), Content.Get_Length());
}
//------------------------------------------------------------------------------------------------------------
bool ALabel::Append(wchar_t symbol)
{ // convert lowercase letters to uppercase letters
	if (symbol >= L'a' && symbol <= L'z')
		symbol -= L'a' - L'A'; 
	
	// convert lowercase letters to uppercase letters (Cyrillic)
	if (symbol >= 0x430 && symbol <= 0x44F)
		symbol -= 0x20;

	if (symbol >= 0x450 && symbol <= 0x45F)
		symbol -= 0x50;

	if (symbol == 8)
		Content.Delete_Last_Symbol();
	else
	{
		if (symbol >= L' ')
		{
			Last_Character_Entered = symbol;
			Content.Append(symbol);
		}
	}

	AsTools::Invalidate_Rect(Content_Rect);

	if (symbol == 0x0d && Content.Get_Length() > 0)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------