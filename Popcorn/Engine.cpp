#include "Engine.h"



// AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
:Game_State (EGS_Lost_Ball), Rest_Distance(0), Life_Count(AsConfig::Initial_Life_Count), Movers{}, Modules{}
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

	AActive_Brick_Red_Blue::Setup_Colors();

	Level.Init();
	Platform.Init(&Ball_Set);

	AFalling_Letter::init();

	ABall::Add_Hit_Checker(&Border);
	ABall::Add_Hit_Checker(&Level);
	ABall::Add_Hit_Checker(&Platform);

	Level.Set_Current_Level(AsLevel::Level_01);

	//Ball.Set_State(EBS_Normal, Platform.X_Pos + Platform.Width / 2);
	
	//Platform.Set_State(EPS_Normal);
	//Platform.Set_State(EPS_Glue_Init);

	Platform.Redraw_Platform();

	SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);

	// Movers
	memset(Movers, 0, sizeof(Movers));
	Movers[0] = &Platform;
	Movers[1] = &Ball_Set;

	// Modules
	memset(Modules, 0, sizeof(Modules));
	Modules[0] = &Level;
	Modules[1] = &Border;
	Modules[2] = &Platform;
	Modules[3] = &Ball_Set;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// Drawing the game screen
	int i;

	SetGraphicsMode(hdc, GM_ADVANCED);
	for (i = 0; i < AsConfig::Max_Modules_Count; i++)
	{
		if (Modules[i] != 0)
		{
			Modules[i]->Clear(hdc, paint_area);
		}
	}
	for (i = 0; i < AsConfig::Max_Modules_Count; i++)
	{
		if (Modules[i] != 0)
		{
			Modules[i]->Draw(hdc, paint_area);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key(EKey_Type key_type, bool key_down)
{
	switch (key_type)
	{
	case EKT_Left:
		Platform.Move(true, key_down);
		break;


	case EKT_Right:
		Platform.Move(false, key_down);
		break;


	case EKT_Space:
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
	case EGS_Test_Ball:
		Ball_Set.Set_For_Test();
		Game_State = EGS_Play_Level;
		break;


	case EGS_Play_Level:
		Play_Level();
		break;


	case EGS_Lost_Ball:
		if (Platform.Get_State() == EPS_Missing)
		{
			Game_State = EGS_Restart_Level;
			Platform.Set_State(EPS_Roll_In);

		}
			break;
		

	case EGS_Restart_Level:
		if (Platform.Get_State() == EPS_Ready)
		{
			Game_State = EGS_Play_Level;
			Ball_Set.Set_On_The_Platform(Platform.Get_Middle_Pos() );
			//Platform.Set_State(EPS_Glue_Init);
		}
		break;
	}
	
	Act();

	return 0;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Play_Level()
{
	int  active_balls_count = 0;
	int  lost_balls_count = 0;
	//double ball_x, ball_y;

	Advance_Movers();
	
	if (Ball_Set.All_Balls_Are_Lost())
	{ // All balls are lost!
		Game_State = EGS_Lost_Ball;
		Level.Stop();
		Platform.Set_State(EPS_Pre_Meltdown);
	}
	else
		Ball_Set.Accelerate();

	if (Ball_Set.Is_Test_Finished())
		Game_State = EGS_Test_Ball;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Advance_Movers()
{
	int  i = 0;
	double current_speed, max_speed = 0.0;

	// 1. Getting maximum speed of moving objects
	for (i = 0; i < AsConfig::Max_Movers_Count; i++)
		if (Movers[i] != 0)
		{
			Movers[i]->Begin_Movement();

			current_speed = fabs(Movers[i]->Get_Speed());

			if (current_speed > max_speed)
				max_speed = current_speed;
		}

	// 2. Mving all the moving objects.
	Rest_Distance += max_speed;

	while (Rest_Distance > 0.0)
	{
		for (i = 0; i < AsConfig::Max_Movers_Count; i++)
			if (Movers[i] != 0)
				Movers[i]->Advance(max_speed);

		//Platform.Advance(max_speed);
		Rest_Distance -= AsConfig::Moving_step_size;
	}
	// 3. Finishiing all moves on the frame
	for (i = 0; i < AsConfig::Max_Movers_Count; i++)
		if (Movers[i] != 0)
			Movers[i]->Finish_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Act()
{
	int index = 0;
	AFalling_Letter* falling_letter;

	Platform.Act();
	Level.Act();

	while (Level.Get_Next_Falling_Letter(index, &falling_letter) )
	{
		if (Platform.Hit_By(falling_letter) )
		{
			On_Falling_Letter(falling_letter);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::On_Falling_Letter(AFalling_Letter* falling_letter)
{
	switch (falling_letter->Letter_Type)
	{
	case ELT_O: // "Cancel"
		Platform.Set_State(EPS_Glue_Finalize);
		break; // !!! Only glue is canceled so far

	case ELT_I: // "Inversion"
		Ball_Set.Inverse_Balls();
		break;
	case ELT_C: // "Speed"
		Ball_Set.Reset_Speed();
		break;
	//case ELT_M: // "Monsters"
	case ELT_G: // "Life"
		if (Life_Count < AsConfig::Max_Life_Count)
			++Life_Count; /// !!! should be displayed on the indicator
		break;
	case ELT_K: // "Glue"
		Platform.Set_State(EPS_Glue_Init);
		break;
	//case ELT_W: // "Wider"

	case ELT_T: // "Three"
		Ball_Set.Triple_Balls();
		break;

	//case ELT_L: // "Laser"
	case ELT_P: // "Floor"
		AsConfig::Level_Has_Floor = true;
		Border.Redraw_Floor();
		// !!! display on the indicator!
		break;
	//case ELT_Plus: // Moving to the next level
	default:
		AsConfig::Throw();
	}
	falling_letter->Finalize();
}
//------------------------------------------------------------------------------------------------------------
