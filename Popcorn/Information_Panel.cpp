#include "Information_Panel.h"

// AString
//------------------------------------------------------------------------------------------------------------
AString::~AString()
{
}
//------------------------------------------------------------------------------------------------------------
AString::AString()
{
}
//------------------------------------------------------------------------------------------------------------
AString::AString(const wchar_t* str)
	: Content(str)
{
}
//------------------------------------------------------------------------------------------------------------
void AString::Append(int value)
{
	wchar_t buffer[32];

	_itow_s(value, buffer, 32, 10);

	Content += buffer;
}
//------------------------------------------------------------------------------------------------------------




// AsInformation_Panel
//------------------------------------------------------------------------------------------------------------
AsInformation_Panel::~AsInformation_Panel()
{
	delete Shadow_Color;
	delete Highlight_Color;
	delete Shaded_Blue;
	delete Dark_Red_Color;

	if (Logo_Corn_Font)
		DeleteObject(Logo_Corn_Font);

	if (Logo_Pop_Font)
		DeleteObject(Logo_Pop_Font);

	if (Name_Font)
		DeleteObject(Name_Font);

	if (Score_Font)
		DeleteObject(Score_Font);

}
//------------------------------------------------------------------------------------------------------------
AsInformation_Panel::AsInformation_Panel()
	: Logo_Corn_Font(0), Logo_Pop_Font(0), Name_Font(0), Score_Font(0), Shadow_Color(0), Highlight_Color(0), Shaded_Blue(0), Dark_Red_Color(0),
	Letter_P(EBrick_Type::Blue, ELetter_Type::P, 214 * AsConfig::Global_Scale + 1, 153 * AsConfig::Global_Scale),
	Letter_G(EBrick_Type::Blue, ELetter_Type::G, 256 * AsConfig::Global_Scale, 153 * AsConfig::Global_Scale),
	Letter_M(EBrick_Type::Blue, ELetter_Type::M, 297 * AsConfig::Global_Scale - 1, 153 * AsConfig::Global_Scale)
{
	Letter_P.Show_Full_Size();
	Letter_G.Show_Full_Size();
	Letter_M.Show_Full_Size();
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Begin_Movement()
{
	//!!! TODO!
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Finish_Movement()
{
	//!!! TODO!
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Advance(double max_speed)
{
	//!!! TODO!
}
//------------------------------------------------------------------------------------------------------------
double AsInformation_Panel::Get_Speed()
{
	//!!! TODO!
	return 0.0;
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Act()
{
	const int scale = AsConfig::Global_Scale;

	RECT rect;
	rect.left = 211 * scale;
	rect.top = 5 * scale;
	rect.right = rect.left + 104 * scale;
	rect.bottom = 199 * scale;

	AsTools::Invalidate_Rect(rect);
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Clear(HDC hdc, RECT& paint_area)
{
	//!!! TODO!
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Draw(HDC hdc, RECT& paint_area)
{
	int i, j, k;
	const int scale = AsConfig::Global_Scale;
	int logo_x_pos = 212 * scale;
	int logo_y_pos = 0;
	int shade_x_offset = 5 * scale;
	int shade_y_offset = 6 * scale;
	RECT rect;

	const wchar_t* pop_str = L"POP";
	const wchar_t* corn_str = L"CORN";
	wchar_t buffer[32];
	//const wchar_t* player_str = L"Qopa"; // 11 symbols max!
	//const wchar_t* player_score = L"SCORE:000000"; // 11 symbols max!
	AString player_score(L"SCORE:");
	// 1. Game logo
	// 1.1 Frame
	AsTools::Rect(hdc, 211, 5, 104, 100, AsConfig::Blue_Color);

	SetBkMode(hdc, 0);

	// 1.2 Shadow
	SelectObject(hdc, Logo_Pop_Font);
	SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
	TextOut(hdc, logo_x_pos + shade_x_offset, logo_y_pos + shade_y_offset, pop_str, wcslen(pop_str));
	SelectObject(hdc, Logo_Corn_Font);
	TextOut(hdc, logo_x_pos - 5 * scale + shade_x_offset, logo_y_pos + 48 * scale + shade_y_offset, corn_str, wcslen(corn_str));

	// 1.3 logo
	SelectObject(hdc, Logo_Pop_Font);
	SetTextColor(hdc, AsConfig::Red_Color.Get_RGB());
	TextOut(hdc, logo_x_pos, logo_y_pos, pop_str, wcslen(pop_str));
	SelectObject(hdc, Logo_Corn_Font);
	TextOut(hdc, logo_x_pos - 5 * scale, logo_y_pos + 48 * scale, corn_str, wcslen(corn_str));

	// 2. Score table
	// 2.1 frame
	AsTools::Rect(hdc, Score_X_Pos, Score_Y_Pos, Score_Width, 2, *Dark_Red_Color);
	AsTools::Rect(hdc, Score_X_Pos, Score_Y_Pos + Score_Height - 2, Score_Width, 2, *Dark_Red_Color);
	AsTools::Rect(hdc, Score_X_Pos, Score_Y_Pos, 2, Score_Height, *Dark_Red_Color);
	AsTools::Rect(hdc, Score_X_Pos + Score_Width - 2, Score_Y_Pos, 2, Score_Height, *Dark_Red_Color);

	// 2.2 The table itself
	AsTools::Rect(hdc, Score_X_Pos + 2, Score_Y_Pos + 2, Score_Width - 4, Score_Height - 4, *Shaded_Blue);

	// 2.3 Highlights (light and shades)
	Highlight_Color->Select_Pen(hdc);
	MoveToEx(hdc, (Score_X_Pos + 2) * scale, (Score_Y_Pos + Score_Height - 2) * scale, 0);
	LineTo(hdc, (Score_X_Pos + 2) * scale, (Score_Y_Pos + 2) * scale);
	LineTo(hdc, (Score_X_Pos + Score_Width - 2) * scale, (Score_Y_Pos + 2) * scale);

	Shadow_Color->Select_Pen(hdc);
	MoveToEx(hdc, (Score_X_Pos + Score_Width - 2) * scale, (Score_Y_Pos + 2) * scale, 0);
	LineTo(hdc, (Score_X_Pos + Score_Width - 2) * scale, (Score_Y_Pos + Score_Height - 2) * scale);
	LineTo(hdc, (Score_X_Pos + 2) * scale, (Score_Y_Pos + Score_Height - 2) * scale);

	// 3.1 Player name
	rect.left = (Score_X_Pos + 5) * scale;
	rect.top = (Score_Y_Pos + 5) * scale;
	rect.right = rect.left + (Score_Width - 2 * 5) * scale;
	rect.bottom = rect.top + 16 * scale;

	Player_Name = L"OLOLOSHKA";
	Draw_String(hdc, rect, Player_Name, true);

	// 3.2 Player score
	rect.top += Score_Val_Offset * scale;
	rect.bottom += Score_Val_Offset * scale;

	//player_score = L"SCORE:";

	//_itow_s(AsConfig::Current_Timer_Tick, buffer, 32, 10);
	//player_score += buffer;

	player_score.Append(AsConfig::Current_Timer_Tick);

	Draw_String(hdc, rect, player_score, false);

	// 4. Letter indicators
	Letter_P.Draw(hdc, paint_area);
	Letter_G.Draw(hdc, paint_area);
	Letter_M.Draw(hdc, paint_area);

	// 5. indicators
	// 5.1 Floor
	AsTools::Rect(hdc, Score_X_Pos + 8, Score_Y_Pos + 55, 12, 30, AsConfig::Teleport_Portal_Color);

	// 5.2 Life
	AsTools::Rect(hdc, Score_X_Pos + 27, Score_Y_Pos + 55, 56, 30, AsConfig::Teleport_Portal_Color);

	// 5.3 Monster
	AsTools::Rect(hdc, Score_X_Pos + 90, Score_Y_Pos + 55, 12, 30, AsConfig::Teleport_Portal_Color);

	// 6. Extra lifes 
	for (i = 0; i < 4; i++)
		for (j = 0; j < 3; j++)
			Draw_Extra_Life(hdc, 27 + 6 + j * 18, 55 + 3 + i * 7);
}
//------------------------------------------------------------------------------------------------------------
bool AsInformation_Panel::Is_Finished()
{
	return false;
	//!!! TODO!
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Init()
{
	Shadow_Color = new AColor(AsConfig::BG_Color, AsConfig::Global_Scale);
	Highlight_Color = new AColor(AsConfig::White_Color, AsConfig::Global_Scale);
	Shaded_Blue = new AColor(0, 170, 170);
	Dark_Red_Color = new AColor(190, 30, 30);

	LOGFONT log_font{};

	log_font.lfHeight = -96;
	log_font.lfWeight = 900;
	log_font.lfOutPrecision = 3;
	log_font.lfClipPrecision = 2;
	log_font.lfQuality = 1;
	log_font.lfPitchAndFamily = 34;

	wcscpy_s(log_font.lfFaceName, L"Arial Black");
	Logo_Corn_Font = CreateFontIndirect(&log_font);

	log_font.lfHeight = -128;
	Logo_Pop_Font = CreateFontIndirect(&log_font);

	log_font.lfHeight = -48;
	log_font.lfWeight = 700;
	log_font.lfOutPrecision = 3;
	log_font.lfClipPrecision = 2;
	log_font.lfQuality = 1;
	log_font.lfPitchAndFamily = 49;

	wcscpy_s(log_font.lfFaceName, L"Consolas");
	Name_Font = CreateFontIndirect(&log_font);

	log_font.lfHeight = -44;
	Score_Font = CreateFontIndirect(&log_font);
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Choose_Font()
{
	CHOOSEFONT cf{};
	LOGFONT lf{};

	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.lpLogFont = &lf;
	cf.Flags = CF_SCREENFONTS;
	cf.nFontType = SCREEN_FONTTYPE;

	ChooseFont(&cf);
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Draw_String(HDC hdc, RECT& rect, AString& str, bool name)
{
	const int scale = AsConfig::Global_Scale;
	int str_left_offset, str_top_offset;
	SIZE str_size;

	// 1. Draw background plate.
	AsTools::Rect(hdc, rect, *Dark_Red_Color);

	// 2. Draw string
	if (name)
		SelectObject(hdc, Name_Font);
	else
		SelectObject(hdc, Score_Font);

	GetTextExtentPoint32(hdc, str, wcslen(str), &str_size); 	//Calculate the length of the string in the window with the player's name

	str_left_offset = rect.left + (rect.right - rect.left) / 2 - str_size.cx / 2;
	str_top_offset = rect.top + (rect.bottom - rect.top) / 2 - str_size.cy / 2 - scale;

	// 2.1 Draw shadow
	SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
	TextOut(hdc, str_left_offset + 2 * scale, str_top_offset + 2 * scale, str, wcslen(str));

	// 2.2 Draw the string
	if (name)
		SetTextColor(hdc, AsConfig::Blue_Color.Get_RGB());
	else
		SetTextColor(hdc, AsConfig::White_Color.Get_RGB());

	TextOut(hdc, str_left_offset, str_top_offset, str, wcslen(str));
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Draw_Extra_Life(HDC hdc, int x, int y)
{
	const int scale = AsConfig::Global_Scale;
	RECT rect;

	rect.left = (Score_X_Pos + x) * scale;
	rect.top = (Score_Y_Pos + y) * scale;
	rect.right = rect.left + 8 * scale;
	rect.bottom = rect.top + 3 * scale;


	AsConfig::Blue_Color.Select(hdc);
	AsTools::Round_Rect(hdc, rect);

	rect.left = (Score_X_Pos + x - 2) * scale;
	rect.top = (Score_Y_Pos + y - 1) * scale;
	rect.right = rect.left + 4 * scale;
	rect.bottom = rect.top + 5 * scale;

	AsTools::Ellipse(hdc, rect, AsConfig::Red_Color);

	rect.left += 8 * scale;
	rect.right += 8 * scale;

	AsTools::Ellipse(hdc, rect, AsConfig::Red_Color);
}
//------------------------------------------------------------------------------------------------------------