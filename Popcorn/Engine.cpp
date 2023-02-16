#include "Engine.h"

// AsInformation_Panel
//------------------------------------------------------------------------------------------------------------
AsInformation_Panel::~AsInformation_Panel()
{
	delete Shadow_Color;
	delete Highlight_Color;
	delete Shaded_Blue;
	delete Dark_Red_Color;
}
//------------------------------------------------------------------------------------------------------------
AsInformation_Panel::AsInformation_Panel()
	: Logo_Corn_Font(0), Logo_Pop_Font(0), Name_Font(0), Score_Font(0), Shadow_Color(0), Highlight_Color(0), Shaded_Blue(0), Dark_Red_Color(0)
{
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
	//!!! TODO!
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Clear(HDC hdc, RECT& paint_area)
{
	//!!! TODO!
}
//------------------------------------------------------------------------------------------------------------
void AsInformation_Panel::Draw(HDC hdc, RECT& paint_area)
{
	const int scale = AsConfig::Global_Scale;
	int logo_x_pos = 212 * scale;
	int logo_y_pos = 0;
	int shade_x_offset = 5 * scale;
	int shade_y_offset = 6 * scale;

	const wchar_t* pop_str = L"POP";
	const wchar_t* corn_str = L"CORN";
	const wchar_t* player_str = L"Qopa"; // 11 symbols max!
	const wchar_t* player_score = L"SCORE: 9999"; // 11 symbols max!
	RECT rect;

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
	// 3.1.1 Shadow

	// 3.1.2 the name
	rect.left = (Score_X_Pos + 5) * scale;
	rect.top = (Score_Y_Pos + 5) * scale;
	rect.right = rect.left + (Score_Width - 2 * 5) * scale;
	rect.bottom = rect.top + 16 * scale;

	Draw_String(hdc, rect, player_str);

	// 3.2 Player score
	//AsTools::Rect(hdc, Score_X_Pos + 5, Score_Y_Pos + 27, Score_Width - 2 * 5, 16, *Dark_Red_Color);
	rect.top += Score_Val_Offset * scale;
	rect.bottom += Score_Val_Offset * scale;

	Draw_String(hdc, rect, player_score);
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
void AsInformation_Panel::Draw_String(HDC hdc, RECT& rect, const wchar_t* str)
{
	const int scale = AsConfig::Global_Scale;
	int str_left_offset, str_top_offset;
	SIZE str_size;

	// 1. Draw background plate.
	AsTools::Rect(hdc, rect, *Dark_Red_Color);

	// 2. Draw string
	SelectObject(hdc, Name_Font);

	GetTextExtentPoint32(hdc, str, wcslen(str), &str_size); 	//Calculate the length of the string in the window with the player's name

	str_left_offset = rect.left + (rect.right - rect.left) / 2 - str_size.cx / 2;
	str_top_offset = rect.top + (rect.bottom - rect.top) / 2 - str_size.cy / 2 - scale;

	// 2.1 Draw shadow
	SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
	TextOut(hdc, str_left_offset + 2 * scale, str_top_offset + 2 * scale, str, wcslen(str));

	// 2.2 Draw the string
	SetTextColor(hdc, AsConfig::Blue_Color.Get_RGB());
	TextOut(hdc, str_left_offset, str_top_offset, str, wcslen(str));
}
//------------------------------------------------------------------------------------------------------------




// AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
: Timer_ID(WM_USER + 1), Game_State (EGame_State::Lost_Ball), Rest_Distance(0), Life_Count(AsConfig::Initial_Life_Count), Modules{}
{
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{// Game setup at startup
	int index;

	SYSTEMTIME sys_time;
	FILETIME file_time;

	GetSystemTime(&sys_time);
	SystemTimeToFileTime(&sys_time, &file_time);

	srand(file_time.dwLowDateTime);

	AsConfig::Hwnd = hwnd;

	AActive_Brick_Red_Blue::Setup_Colors();
	AExplosive_Ball::Setup_Colors();

	Level.Init();
	Platform.Init(&Ball_Set, &Laser_Beam_Set);
	Monster_Set.Init(&Border);
	Information_Panel.Init();

	AFalling_Letter::Init();

	ABall::Hit_Checker_List.Add_Hit_Checker(&Border);
	ABall::Hit_Checker_List.Add_Hit_Checker(&Level);
	ABall::Hit_Checker_List.Add_Hit_Checker(&Platform);
	ABall::Hit_Checker_List.Add_Hit_Checker(&Monster_Set);

	ALaser_Beam::Hit_Checker_List.Add_Hit_Checker(&Level);
	ALaser_Beam::Hit_Checker_List.Add_Hit_Checker(&Monster_Set);

	AsPlatform::Hit_Checker_List.Add_Hit_Checker(&Monster_Set);

	Level.Set_Current_Level(AsLevel::Level_01);

	Platform.Redraw_Platform();

	SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);

	// Modules
	memset(Modules, 0, sizeof(Modules));
	index = 0;

	Add_Next_Module(index, &Level);
	Add_Next_Module(index, &Border);
	Add_Next_Module(index, &Platform);
	Add_Next_Module(index, &Ball_Set);
	Add_Next_Module(index, &Laser_Beam_Set);
	Add_Next_Module(index, &Monster_Set);
	Add_Next_Module(index, &Information_Panel);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// Drawing the game screen
	int i;

	SetGraphicsMode(hdc, GM_ADVANCED);

	for (i = 0; i < AsConfig::Max_Modules_Count; i++)
		if (Modules[i] != 0)
			Modules[i]->Clear(hdc, paint_area);

	for (i = 0; i < AsConfig::Max_Modules_Count; i++)
		if (Modules[i] != 0)
			Modules[i]->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key(EKey_Type key_type, bool key_down)
{
	switch (key_type)
	{
	case EKey_Type::Left:
		Platform.Move(true, key_down);
		break;


	case EKey_Type::Right:
		Platform.Move(false, key_down);
		break;


	case EKey_Type::Space:
		Platform.On_Space_Key(key_down);
		break;
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
	++AsConfig::Current_Timer_Tick;

	switch (Game_State)
	{
	case EGame_State::Test_Ball:
		Ball_Set.Set_For_Test();
		Game_State = EGame_State::Play_Level;
		break;


	case EGame_State::Play_Level:
		Play_Level();
		break;


	case EGame_State::Lost_Ball:
		if (Platform.Has_State(EPlatform_Substate_Regular::Missing))
			Restart_Level();
			break;
		

	case EGame_State::Restart_Level:
		if (Platform.Has_State (EPlatform_Substate_Regular::Ready) )
		{
			Game_State = EGame_State::Play_Level;
			Ball_Set.Set_On_The_Platform(Platform.Get_Middle_Pos() );
			Monster_Set.Activate(5);
		}
		break;
	}
	
	Act();

	return 0;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Restart_Level()
{
	Game_State = EGame_State::Restart_Level;
	Border.Open_Gate(7, true);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Play_Level()
{

	Advance_Movers();
	
	if (Ball_Set.All_Balls_Are_Lost())
	{ // All balls are lost!
		Game_State = EGame_State::Lost_Ball;
		Level.Stop();
		Monster_Set.Destroy_All();
		Laser_Beam_Set.Disable_All();
		Platform.Set_State(EPlatform_State::Meltdown);
	}
	else
		Ball_Set.Accelerate();

	if (Ball_Set.Is_Test_Finished())
		Game_State = EGame_State::Test_Ball;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Advance_Movers()
{
	int  i;
	double current_speed, max_speed = 0.0;

	// 1. Getting maximum speed of moving objects
	for (i = 0; i < AsConfig::Max_Movers_Count; i++)
		if (Modules[i] != 0)
		{
			Modules[i]->Begin_Movement();

			current_speed = fabs(Modules[i]->Get_Speed());

			if (current_speed > max_speed)
				max_speed = current_speed;
		}

	// 2. Mving all the moving objects.
	Rest_Distance += max_speed;

	while (Rest_Distance > 0.0)
	{
		for (i = 0; i < AsConfig::Max_Movers_Count; i++)
			if (Modules[i] != 0)
				Modules[i]->Advance(max_speed);

		Rest_Distance -= AsConfig::Moving_Step_Size;
	}

	// 3. Заканчиваем все движения на этом кадре
	for (i = 0; i < AsConfig::Max_Movers_Count; i++)
		if (Modules[i] != 0)
			Modules[i]->Finish_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Act()
{
	int i;
	int index = 0;
	AFalling_Letter* falling_letter;

	// 1. Do all the things
	for (i = 0; i < AsConfig::Max_Modules_Count; i++)
		if (Modules[i] != 0)
			Modules[i]->Act();

	// 2. Catching falling letters
	while (Level.Get_Next_Falling_Letter(index, &falling_letter) )
	{
		if (Platform.Hit_By(falling_letter) )
		{
			On_Falling_Letter(falling_letter);
		}
	}

	// 3. Restarting level (if we have to)
	if (Game_State == EGame_State::Restart_Level)
		if (Border.Is_Gate_Opened(AsConfig::Gates_Count - 1) )
			Platform.Set_State(EPlatform_State::Rolling);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::On_Falling_Letter(AFalling_Letter* falling_letter)
{
	switch (falling_letter->Letter_Type)
	{
	case ELetter_Type::O: // "Cancel"
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break; // !!! Only glue is canceled so far

	case ELetter_Type::I: // "Inversion"
		Ball_Set.Inverse_Balls();
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;

	case ELetter_Type::C: // "Speed"
		Ball_Set.Reset_Speed();
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;

	//case ELetter_Type::M: // "Monsters"

	case ELetter_Type::G: // "Life"
		if (Life_Count < AsConfig::Max_Life_Count)
			++Life_Count; // !!! should be displayed on the indicator
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;

	case ELetter_Type::K: // "Glue"
		Platform.Set_State(EPlatform_State::Glue);
		break;

	case ELetter_Type::W: // "Wider"
		Platform.Set_State(EPlatform_State::Expanding);
		break;

	case ELetter_Type::T: // "Three"
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		Ball_Set.Triple_Balls();
		break;

	case ELetter_Type::L: // "Laser"
		Platform.Set_State(EPlatform_State::Laser);
		break;

	case ELetter_Type::P: // "Floor"
		AsConfig::Level_Has_Floor = true;
		Border.Redraw_Floor();
		// !!! display on the indicator!
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;

	//case ELetter_Type::Plus: // Moving to the next level

	default:
		AsConfig::Throw();
	}
	falling_letter->Finalize();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Add_Next_Module(int& index, AGame_Object* game_object)
{
	if (index >= 0 && index < AsConfig::Max_Modules_Count)
		Modules[index++] = game_object;
	else
		AsConfig::Throw();

}
//------------------------------------------------------------------------------------------------------------