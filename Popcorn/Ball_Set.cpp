#include "Ball_Set.h"

// AsBall_Set
AsBall_Set::AsBall_Set()
	: Balls(AsConfig::Max_Balls_Count)
{
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Act()
{
	for (auto &ball: Balls)
	{
		if (ball.Get_State() == EBall_State::On_Platform)
			if (ball.Release_Timer_Tick != 0 && AsConfig::Current_Timer_Tick >= ball.Release_Timer_Tick)
				ball.Release();
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Release_From_The_Platform(double platform_x_pos)
{
	for (auto& ball : Balls)
		if (ball.Get_State() == EBall_State::On_Platform)
			ball.Set_State(EBall_State::Normal, platform_x_pos, AsConfig::Start_Ball_Y_Pos);
}
//------------------------------------------------------------------------------------------------------------
bool AsBall_Set::Release_Next_Ball()
{
	for (auto& ball : Balls)
	{
		if (ball.Get_State() == EBall_State::On_Platform)
		{
			ball.Release();
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Set_On_The_Platform(double platform_x_pos)
{
	int i;

	for (i = 0; i < 1; i++)
	{
		Balls[i].Set_State(EBall_State::Normal);
		Balls[i].Set_State(EBall_State::On_Platform, platform_x_pos, AsConfig::Start_Ball_Y_Pos);
		Balls[i].Release_Timer_Tick = 0;
	}

	for (; i < (int)Balls.size(); i++)
		Balls[i].Set_State(EBall_State::Disabled);
}
//------------------------------------------------------------------------------------------------------------
bool AsBall_Set::All_Balls_Are_Lost()
{
	int  active_balls_count = 0;
	int  lost_balls_count = 0;

	for (auto& ball : Balls)
	{
		if (ball.Get_State() == EBall_State::Disabled)
			continue;

		++active_balls_count;

		if (ball.Get_State() == EBall_State::Lost)
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
	return Balls[0].Is_Test_Finished(); // only ball number 0 used for tests
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Triple_Balls()
{ // Triplying the ball farthest from the platform

	ABall* further_ball = nullptr;
	ABall* left_candidate = 0, * right_candidate = 0;
	double current_ball_x, current_ball_y;
	double further_ball_x, further_ball_y;
	double previous_direction, direction_delta;

	// 1. Chosinbg the farthest ball accroding to Y coord
	for (auto& ball : Balls)
	{
		if (ball.Get_State() != EBall_State::Normal)
			continue;

		if (further_ball == nullptr)
			further_ball = &ball;
		else
		{
			ball.Get_Center(current_ball_x, current_ball_y);
			further_ball->Get_Center(further_ball_x, further_ball_y);

			if (current_ball_y < further_ball_y)
				further_ball = &ball;
		}
	}


	// 2. if (EBall_State::Normal) 
	//    {making it into three}
	if (further_ball == nullptr)
		return;

	for (auto& ball : Balls)
	{
		switch (ball.Get_State())
		{
		case EBall_State::Disabled:
		case EBall_State::Lost:
			if (left_candidate == 0)
				left_candidate = &ball;
			else
				if (right_candidate == 0)
				{
					right_candidate = &ball;
					break; // Both candidates found
				}
		}
	}
	// 3. Spread the side balls to the sides
	if (left_candidate != 0)
	{
		*left_candidate = *further_ball;

		previous_direction = left_candidate->Get_Direction();

		left_candidate->Set_Direction(previous_direction + AsConfig::Min_Ball_Angle);

		direction_delta = fabs(left_candidate->Get_Direction() - previous_direction);

		if (direction_delta < AsConfig::Min_Ball_Angle / 2.0)
			left_candidate->Set_Direction(previous_direction - AsConfig::Min_Ball_Angle / 2.0);

	}
	if (right_candidate != 0)
	{
		*right_candidate = *further_ball;

		previous_direction = right_candidate->Get_Direction();

		right_candidate->Set_Direction(previous_direction - AsConfig::Min_Ball_Angle);

		direction_delta = fabs(right_candidate->Get_Direction() - previous_direction);

		if (direction_delta < AsConfig::Min_Ball_Angle / 2.0)
			right_candidate->Set_Direction(previous_direction + AsConfig::Min_Ball_Angle / 2.0);

	}
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Inverse_Balls()
{//reverse direction for all balls
	for (auto& ball : Balls)
	{
		if (ball.Get_State() == EBall_State::Normal)
			ball.Set_Direction(ball.Get_Direction() + M_PI);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Accelerate()
{
	for (auto& ball : Balls)
	{
 		if (ball.Get_State() == EBall_State::Normal)
			ball.Set_Speed(ball.Get_Speed() * AsConfig::Ball_Acceleration);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::Reset_Speed()
{
	for (auto& ball : Balls)
	{
		if (ball.Get_State() == EBall_State::Normal)
			ball.Set_Speed(AsConfig::Normal_Ball_Speed);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBall_Set::On_Platform_Advance(double direction, double platform_speed, double max_speed)
{
	for (auto& ball : Balls)
	{
		if (ball.Get_State() == EBall_State::On_Platform)
			ball.Forced_Advance(direction, platform_speed, max_speed);
	}
}
//------------------------------------------------------------------------------------------------------------
bool AsBall_Set::Get_Next_Game_Object(int& index, AGame_Object** game_object)
{
	if (index < 0 || index >= (int)Balls.size())
		return false;
	
	*game_object = &Balls[index++];

	return true;
}
//------------------------------------------------------------------------------------------------------------