#include "Engine.h"

// AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
:Game_State (EGS_Lost_Ball)
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

	AFalling_Letter::init();

	ABall::Add_Hit_Checker(&Border);
	ABall::Add_Hit_Checker(&Level);
	ABall::Add_Hit_Checker(&Platform);

	Level.Set_Current_Level(AsLevel::Level_01);

	//Ball.Set_State(EBS_Normal, Platform.X_Pos + Platform.Width / 2);
	
	//Platform.Set_State(EPS_Normal);

	Platform.Redraw_Platform();

	SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// Drawing the game screen

	int i;

	SetGraphicsMode(hdc, GM_ADVANCED);

	Level.Draw(hdc, paint_area);
	Border.Draw(hdc, paint_area);
	Platform.Draw(hdc, paint_area);
	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		Balls[i].Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
	int i;

	switch (key_type)
	{
	case EKT_Left:
		Platform.Move(true);
		break;


	case EKT_Right:
		Platform.Move(false);
		break;


	case EKT_Space:
		if (Platform.Get_State() == EPS_Ready)
		{
			for (i = 0; i < AsConfig::Max_Balls_Count; i++)
				if (Balls[i].Get_State() == EBS_On_Platform)
					Balls[i].Set_State(EBS_Normal, Platform.X_Pos + Platform.Width / 2, AsConfig::Start_Ball_Y_Pos);

			Platform.Set_State(EPS_Normal);

		}
		break;
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
	int i;

	++AsConfig::Current_Timer_Tick;

	switch (Game_State)
	{
	case EGS_Test_Ball:
		Balls[0].Set_For_Test(); // only ball number 0 used for tests
		Game_State = EGS_Play_Level;
		break;


	case EGS_Play_Level:
		for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		{
			Balls[i].Move();

			if (Balls[i].Get_State() == EBS_Lost)
			{
				Game_State = EGS_Lost_Ball;
				Platform.Set_State(EPS_Meltdown);
			}

		}

		if (Balls[0].Is_Test_Finished()) // only ball number 0 used for tests
			Game_State = EGS_Test_Ball;

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

			for (i = 0; i < 3; i++)
				Balls[i].Set_State(EBS_On_Platform, Platform.X_Pos + Platform.Width / 2, AsConfig::Start_Ball_Y_Pos);

			for (; i < AsConfig::Max_Balls_Count; i++)
				Balls[i].Set_State (EBS_Disabled);
		}
		break;
	}
	
	Act();

	return 0;
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
	//case ELT_O: // "Cancel"
	//case ELT_I: // "Inversion"
	//case ELT_C: // "Speed"
	//case ELT_M: // "Monsters"
	//case ELT_G: // "Life"
	//case ELT_K: // "Glue"
	//case ELT_W: // "Wider"
	case ELT_T: // "Three"
		break;
	//case ELT_L: // "Laser"
	//case ELT_P: // "Floor"
	//case ELT_Plus: // Moving to the next level
	default:
		AsConfig::Throw();
	}
	falling_letter->Finalize();
}
//------------------------------------------------------------------------------------------------------------
