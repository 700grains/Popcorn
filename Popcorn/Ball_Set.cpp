#include "Ball_Set.h"

// AsBall_Set
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Begin_Movement()
{
	int i;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		Balls[i].Begin_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Finish_Movement()
{
	int i;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		Balls[i].Finish_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Advance(double max_speed)
{
	int i;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		Balls[i].Advance(max_speed);
}
//------------------------------------------------------------------------------------------------------------
double AsBall_Set::Get_Speed()
{
	int i;
	double max_speed = 0.0, current_speed;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		current_speed = Balls[i].Get_Speed();

		if (current_speed > max_speed)
			max_speed = current_speed;
	}

	return max_speed;
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Draw(HDC hdc, RECT& paint_area)
{
	int i;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		Balls[i].Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Release_From_The_Platform(double platform_x_pos)
{
	int i;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
		if (Balls[i].Get_State() == EBS_On_Platform)
			Balls[i].Set_State(EBS_Normal, platform_x_pos, AsConfig::Start_Ball_Y_Pos);
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Set_On_The_Platform(double platform_x_pos)
{
	int i;

	Balls[0].Set_State(EBS_On_Platform, platform_x_pos, AsConfig::Start_Ball_Y_Pos);

	for (i = 1; i < AsConfig::Max_Balls_Count; i++)
		Balls[i].Set_State(EBS_Disabled);
}
//------------------------------------------------------------------------------------------------------------
bool AsBall_Set::All_Balls_Are_Lost()
{
	int  i;
	int  active_balls_count = 0;
	int  lost_balls_count = 0;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		if (Balls[i].Get_State() == EBS_Disabled)
			continue;

		++active_balls_count;

		if (Balls[i].Get_State() == EBS_Lost)
		{
			++lost_balls_count;
			continue;
		}
	}

	if (active_balls_count == lost_balls_count)
		return  true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Set_For_Test()
{
	Balls[0].Set_For_Test(); // only ball number 0 used for tests
}
//------------------------------------------------------------------------------------------------------------
bool AsBall_Set::Is_Test_Finished()
{
	return (Balls[0].Is_Test_Finished()); // only ball number 0 used for tests
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Triple_Balls()
{ // Triplying the ball farthest from the platform

	int i;
	ABall* current_ball;
	ABall* further_ball = nullptr;
	ABall* left_candidate = 0, * right_candidate = 0;
	double current_ball_x, current_ball_y;
	double further_ball_x, further_ball_y;
	// 1. Chosinbg the farthest ball accroding to Y coord
	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		current_ball = &Balls[i];

		if (current_ball->Get_State() != EBS_Normal)
			continue;

		if (further_ball == nullptr)
			further_ball = current_ball;
		else
		{
			current_ball->Get_Center(current_ball_x, current_ball_y);
			further_ball->Get_Center(further_ball_x, further_ball_y);

			if (current_ball_y < further_ball_y)
				further_ball = current_ball;
		}
	}


	// 2. if (EBS_Normal) 
	//    {making it into three}
	if (further_ball == nullptr)
		return;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		current_ball = &Balls[i];

		switch (current_ball->Get_State())
		{
		case EBS_Disabled:
		case EBS_Lost:
			if (left_candidate == 0)
				left_candidate = current_ball;
			else
				if (right_candidate == 0)
				{
					right_candidate = current_ball;
					break; // Both candidates found
				}
		}
	}
	// 3. Spread the side balls to the sides
	if (left_candidate != 0)
	{
		*left_candidate = *further_ball;
		left_candidate->Set_Direction(left_candidate->Get_Direction() + M_PI / 8.0);
	}
	if (right_candidate != 0)
	{
		*right_candidate = *further_ball;
		right_candidate->Set_Direction(right_candidate->Get_Direction() - M_PI / 8.0);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Inverse_Balls()
{//reverse direction for all balls
	int i;
	ABall* current_ball;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		current_ball = &Balls[i];
		if (current_ball->Get_State() == EBS_Normal)
			current_ball->Set_Direction(current_ball->Get_Direction() + M_PI);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Accelerate()
{
	int i;
	ABall* current_ball;

	for (i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
		current_ball = &Balls[i];

 		if (current_ball->Get_State() == EBS_Normal)
			current_ball->Set_Speed(current_ball->Get_Speed() * AsConfig::Ball_Acceleration);
	}
}
//------------------------------------------------------------------------------------------------------------
