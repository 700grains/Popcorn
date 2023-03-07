#include "Engine.h"

// AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
: Timer_ID(WM_USER + 1), Game_State (EGame_State::Mop_Level), Rest_Distance(0), Modules{}
{
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{// Game setup at startup

	SYSTEMTIME sys_time;
	FILETIME file_time;

	GetSystemTime(&sys_time);
	SystemTimeToFileTime(&sys_time, &file_time);

	srand(file_time.dwLowDateTime);

	AsConfig::Hwnd = hwnd;

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

	//Level.Set_Current_Level(1);

	Platform.Redraw_Platform();

	SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);

	// Modules
	Modules.push_back(&Level);
	Modules.push_back(&Border);
	Modules.push_back(&Platform);
	Modules.push_back(&Ball_Set);
	Modules.push_back(&Laser_Beam_Set);
	Modules.push_back(&Monster_Set);
	Modules.push_back(&Information_Panel);

	Level.Mop_Level(1);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// Drawing the game screen

	SetGraphicsMode(hdc, GM_ADVANCED);

	for (auto* engine_module : Modules) 
		engine_module->Clear(hdc, paint_area);

	for (auto* engine_module : Modules)
		engine_module->Draw(hdc, paint_area);
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


	case EGame_State::Mop_Level:
		if (Level.Is_Level_Moping_Done())
			Restart_Level();
		break;


	case EGame_State::Play_Level:
		Play_Level();
		break;


	case EGame_State::Lost_Ball:
		if (Platform.Has_State(EPlatform_Substate_Regular::Missing))
		{
			if (!Information_Panel.Remove_A_Life())
				Game_Over();

			Restart_Level();
		}
			break;
		

	case EGame_State::Restart_Level:
		if (Platform.Has_State (EPlatform_Substate_Regular::Ready) )
		{
			Game_State = EGame_State::Play_Level;
			Ball_Set.Set_On_The_Platform(Platform.Get_Middle_Pos() );
			Monster_Set.Activate(6);
			Level.Hide_Title();
		}
		break;


	case EGame_State::Finish_Level:
		if (Monster_Set.Are_All_Destroyed() && Platform.Has_State(EPlatform_Substate_Regular::Missing) )
		{
			Level.Mop_Next_Level();
			Game_State = EGame_State::Mop_Level;
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
	Level.Show_Title();
	Border.Open_Gate(7, true);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Play_Level()
{

	Advance_Movers();
	
	if (Ball_Set.All_Balls_Are_Lost())
	{
		Game_State = EGame_State::Lost_Ball;
		Stop_Play();
	}
	else
		Ball_Set.Accelerate();

	if (Ball_Set.Is_Test_Finished())
		Game_State = EGame_State::Test_Ball;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Stop_Play()
{// All balls are lost!

	Level.Stop();
	Monster_Set.Destroy_All();
	Laser_Beam_Set.Disable_All();
	Platform.Set_State(EPlatform_State::Meltdown);
	Information_Panel.Floor_Panel.Reset();
	Information_Panel.Monsters_Panel.Reset();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Game_Over()
{
	Level.Game_Title.Show(false);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Game_Won()
{
	Level.Game_Title.Show(true);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Advance_Movers()
{
	double current_speed, max_speed = 0.0;

	// 1. Getting maximum speed of moving objects
	for (auto* engine_module : Modules)
		{
			engine_module->Begin_Movement();

			current_speed = fabs(engine_module->Get_Speed());

			if (current_speed > max_speed)
				max_speed = current_speed;
		}

	// 2. Mving all the moving objects.
	Rest_Distance += max_speed;

	while (Rest_Distance > 0.0)
	{
		for (auto* engine_module : Modules)
			engine_module->Advance(max_speed);

		Rest_Distance -= AsConfig::Moving_Step_Size;
	}

	// 3. Заканчиваем все движения на этом кадре
	for (auto* engine_module : Modules)
		engine_module->Finish_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Act()
{
	int index = 0;
	AFalling_Letter* falling_letter;

	// 1. Do all the things
	for (auto *engine_module : Modules)
		engine_module->Act();

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

	Handle_Message();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Handle_Message()
{
	AMessage* message;

	if (AsMessage_Manager::Get_Message(&message))
	{
		switch (message->Message_Type)
		{
		case EMessage_Type::Floor_Is_Over:
			AsConfig::Level_Has_Floor = false;
			Border.Redraw_Floor();
			delete message;
			break;


		case EMessage_Type::Unfreeze_Monsters:
			Monster_Set.Set_Freeze_State(false);
			break;


		case EMessage_Type::Level_Done:
			if (! Level.Can_Mop_Next_Level())
				Game_Won();
			else
			{
				Stop_Play();
				Ball_Set.Disable_All_Balls();
				Game_State = EGame_State::Finish_Level;
			}
			break;


		default:
			AsConfig::Throw();
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::On_Falling_Letter(AFalling_Letter * falling_letter)
{
	switch (falling_letter->Letter_Type)
	{
	case ELetter_Type::O: // "Cancel"
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		Information_Panel.Floor_Panel.Cancellation();
		Information_Panel.Monsters_Panel.Cancellation();
		break;

	case ELetter_Type::I: // "Inversion"
		Ball_Set.Inverse_Balls();
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;

	case ELetter_Type::C: // "Speed"
		Ball_Set.Reset_Speed();
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;

	case ELetter_Type::M: // "Monsters"
		Monster_Set.Set_Freeze_State(true);
		Information_Panel.Monsters_Panel.Restart(); 
		break;

	case ELetter_Type::G: // "Life"
		Information_Panel.Add_Life();
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
		Information_Panel.Floor_Panel.Restart();// !!! display on the indicator!
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;

	//case ELetter_Type::Plus: // Moving to the next level

	default:
		AsConfig::Throw();
	}

	falling_letter->Finalize();

	AsInformation_Panel::Update_Score(EScore_Event_Type::Catch_Letter);
}
//------------------------------------------------------------------------------------------------------------