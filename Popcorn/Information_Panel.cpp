#include "Information_Panel.h"

// AIndicator
//------------------------------------------------------------------------------------------------------------
AIndicator::AIndicator(EMessage_Type message_type, int x_pos, int y_pos)
	: Message_Type(message_type), Message_Was_Sent(true), X_Pos(x_pos), Y_Pos(y_pos), Indicator_Lifetime()
{
	const int scale = AsConfig::Global_Scale;

	Indicator_Rect.left = X_Pos * scale;
	Indicator_Rect.top = Y_Pos * scale;
	Indicator_Rect.right = Indicator_Rect.left + Width * scale;
	Indicator_Rect.bottom = Indicator_Rect.top + Height * scale;
}
//------------------------------------------------------------------------------------------------------------
void AIndicator::Act()
{	
	if (! Is_Finished() )
		AsTools::Invalidate_Rect(Indicator_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AIndicator::Clear(HDC hdc, RECT& paint_area)
{
	//!!! TODO!
}
//------------------------------------------------------------------------------------------------------------
void AIndicator::Draw(HDC hdc, RECT& paint_area)
{
	const int scale = AsConfig::Global_Scale;

	double ratio;
	int current_height;
	RECT rect;

	AsTools::Rect(hdc, X_Pos, Y_Pos, Width, Height, AsConfig::Teleport_Portal_Color); // border

	if (Indicator_Lifetime == 0 || Is_Finished())
		return;

	ratio = (double)(Indicator_Lifetime - AsConfig::Current_Timer_Tick) / (double)Indicator_Timeout;

	current_height = (int)((double)(Inner_Height * scale) * ratio);

	if (current_height == 0)
		return;

	rect.left = (X_Pos + 1) * scale;
	rect.top = (Y_Pos + 1) * scale + (Inner_Height * scale - current_height);
	rect.right = rect.left + Inner_Width * scale;
	rect.bottom = (Y_Pos + 1 + Inner_Height) * scale;

	AsTools::Rect(hdc, rect, AsConfig::Red_Color); // inner part of the indicator
}
//------------------------------------------------------------------------------------------------------------
bool AIndicator::Is_Finished()
{
	AMessage* message;

	if (AsConfig::Current_Timer_Tick > Indicator_Lifetime)
	{
		if (! Message_Was_Sent)
		{
			message = new AMessage(Message_Type);

			AsMessage_Manager::Add_Message(message);

			Message_Was_Sent = true;
		}

		return true;
	}
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AIndicator::Restart()
{
	Indicator_Lifetime = AsConfig::Current_Timer_Tick + Indicator_Timeout;

	Message_Was_Sent = false;
}
//------------------------------------------------------------------------------------------------------------
void AIndicator::Reset()
{
	Indicator_Lifetime = 0;
	AsTools::Invalidate_Rect(Indicator_Rect);
}
//------------------------------------------------------------------------------------------------------------




// AsInformation_Panel
//------------------------------------------------------------------------------------------------------------
int AsInformation_Panel::Score = 0;
int AsInformation_Panel::Lives_Left = 5;
RECT AsInformation_Panel::Logo_Rect;
RECT AsInformation_Panel::Data_Rect;
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
	Letter_M(EBrick_Type::Blue, ELetter_Type::M, 297 * AsConfig::Global_Scale - 1, 153 * AsConfig::Global_Scale),
	Floor_Panel(EMessage_Type::Floor_Is_Over, Score_X_Pos + 8, Score_Y_Pos + Indicator_Y_Offset), 
	Monsters_Panel(EMessage_Type::Unfreeze_Monsters, Score_X_Pos + 90, Score_Y_Pos + Indicator_Y_Offset)
{
	const int scale = AsConfig::Global_Scale;

	// creating rect for logo
	Logo_Rect.left = Score_X_Pos * scale;
	Logo_Rect.top = 5 * scale;
	Logo_Rect.right = Logo_Rect.left + 104 * scale;
	Logo_Rect.bottom = Logo_Rect.top + 100 * scale;

	Data_Rect.left = Score_X_Pos * scale;
	Data_Rect.top = Score_Y_Pos * scale;
	Data_Rect.right = Data_Rect.left + Score_Width * scale;
	Data_Rect.bottom = Data_Rect.top + Score_Height * scale;

	// Chose_Font();
	Letter_P.Show_Full_Size();
	Letter_G.Show_Full_Size();
	Letter_M.Show_Full_Size();
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Begin_Movement()
{
	// Not used
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Finish_Movement()
{
	// Not used
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Advance(double max_speed)
{
	// Not used
}
//------------------------------------------------------------------------------------------------------------
double AsInformation_Panel::Get_Speed()
{
	// Not used
	return 0.0;
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Act()
{
	Floor_Panel.Act();
	Monsters_Panel.Act();
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Clear(HDC hdc, RECT& paint_area)
{
	// Not used
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Draw(HDC hdc, RECT& paint_area)
{
	const int scale = AsConfig::Global_Scale;
	RECT rect, intersection_rect;

	const wchar_t* pop_str = L"POP";
	const wchar_t* corn_str = L"CORN";
	AString player_score(L"SCORE:");

	// 1. Game logo
	if (IntersectRect(&intersection_rect, &paint_area, &Logo_Rect))
	{
		// 1.1 Frame
		AsTools::Rect(hdc, 211, 5, 104, 100, AsConfig::Blue_Color);

		SetBkMode(hdc, 0);

		// 1.2 Shadow
		SelectObject(hdc, Logo_Pop_Font);
		SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
		TextOut(hdc, (Logo_X_Pos + Shade_X_Offset) * scale, (Logo_Y_Pos + Shade_Y_Offset) * scale, pop_str, wcslen(pop_str));

		SelectObject(hdc, Logo_Corn_Font);
		TextOut(hdc, (Logo_X_Pos + Shade_X_Offset - 5) * scale , (Logo_Y_Pos + Shade_Y_Offset + 48) * scale , corn_str, wcslen(corn_str));

		// 1.3 logo
		SelectObject(hdc, Logo_Pop_Font);
		SetTextColor(hdc, AsConfig::Red_Color.Get_RGB());
		TextOut(hdc, Logo_X_Pos * scale, Logo_Y_Pos * scale, pop_str, wcslen(pop_str));

		SelectObject(hdc, Logo_Corn_Font);
		TextOut(hdc, (Logo_X_Pos - 5) * scale, (Logo_Y_Pos + 48) * scale, corn_str, wcslen(corn_str));
	}

	// 2. Score table
	if (IntersectRect(&intersection_rect, &paint_area, &Data_Rect))
	{
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

		player_score.Append(Score);

		Draw_String(hdc, rect, player_score, false);

		// 4. Letter indicators
		Letter_P.Draw(hdc, paint_area);
		Letter_G.Draw(hdc, paint_area);
		Letter_M.Draw(hdc, paint_area);

		// 5. indicators
		// 5.1 Floor
		Floor_Panel.Draw(hdc, paint_area);

		// 5.2 Life
		Show_Extra_Lives(hdc);

		// 5.3 Monster
		Monsters_Panel.Draw(hdc, paint_area);
	}
}
//------------------------------------------------------------------------------------------------------------
bool AsInformation_Panel::Is_Finished()
{
	return false;
	// Not used
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
void AsInformation_Panel::Update_Score(EScore_Event_Type event_type)
{

	switch (event_type)
	{
	case EScore_Event_Type::Hit_Brick:
		Score += 10;
		break;

	case EScore_Event_Type::Hit_Monster:
		Score += 30;
		break;

	case EScore_Event_Type::Catch_Letter:
		Score += 20;
		break;

	default:
		break;
	}

	AsTools::Invalidate_Rect(Data_Rect);
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

	GetTextExtentPoint32(hdc, str.Get_Content(), str.Get_Length(), &str_size); 	//Calculate the length of the string in the window with the player's name

	str_left_offset = rect.left + (rect.right - rect.left) / 2 - str_size.cx / 2;
	str_top_offset = rect.top + (rect.bottom - rect.top) / 2 - str_size.cy / 2 - scale;

	// 2.1 Draw shadow
	SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
	TextOut(hdc, str_left_offset + 2 * scale, str_top_offset + 2 * scale, str.Get_Content(), str.Get_Length());

	// 2.2 Draw the string
	if (name)
		SetTextColor(hdc, AsConfig::Blue_Color.Get_RGB());
	else
		SetTextColor(hdc, AsConfig::White_Color.Get_RGB());

	TextOut(hdc, str_left_offset, str_top_offset, str.Get_Content(), str.Get_Length());
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Show_Extra_Lives(HDC hdc)
{
	int i, j;
	int lives_count = Lives_Left;

	AsTools::Rect(hdc, Score_X_Pos + 27, Score_Y_Pos + Indicator_Y_Offset, 56, 30, AsConfig::Teleport_Portal_Color);

	for (j = 0; j < 3; j++)
		for (i = 0; i < 4; i++)
		{
			if (lives_count <= 0)
				break;

			Draw_Extra_Life(hdc, 27 + 6 + j * 18, 55 + 3 + i * 7);
			lives_count--;
		}
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
