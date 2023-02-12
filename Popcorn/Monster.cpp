#include "Monster.h"

// AMonster
const double AMonster::Max_Cornea_Height = 11.0;
const double AMonster::Blinking_Timeouts[AMonster::Blink_Stages_Count] = { 0.4, 0.3, 1, 0.4, 0.4, 0.4, 0.9 };
const EEye_State AMonster::Blinking_States[AMonster::Blink_Stages_Count] =
{
	EEye_State::Closed,
	EEye_State::Opening,
	EEye_State::Staring,
	EEye_State::Closing,
	EEye_State::Opening,
	EEye_State::Staring,
	EEye_State::Closing
};
//------------------------------------------------------------------------------------------------------------
AMonster::AMonster()
	:Eye_State(EEye_State::Closed), Monster_State(EMonster_State::Missing), X_Pos(0.0), Y_Pos(0.0), Speed(0.0), Direction(0.0),
	Blink_Ticks{}, Cornea_Height(Max_Cornea_Height), Start_Blinking_Time(0), Total_Animation_Time(0), Next_Direction_Switch_Tick(0), Monster_Is_Alive_Timer(0), Monster_Rect{},
	Previous_Monster_Rect{}
{
}
//------------------------------------------------------------------------------------------------------------
bool AMonster::Check_Hit(double next_x_pos, double next_y_pos, ABall_Object* ball)
{ // Checking if the ball hit the monster

	double radius = (double)Width / 2.0;

	if (! AsTools::Reflect_On_Circle(next_x_pos, next_y_pos, X_Pos + radius, Y_Pos + radius, radius, ball))
		return false;

	Destroy();

	return true;
}
//------------------------------------------------------------------------------------------------------------
bool AMonster::Check_Hit(double next_x_pos, double next_y_pos)
{// Return "true" if at given positions our laser beam hits a monster

	if (!(Monster_State == EMonster_State::Emitting || Monster_State == EMonster_State::Alive))
		return false;

	if (next_x_pos >= X_Pos && next_x_pos <= X_Pos + (double)Width && next_y_pos >= Y_Pos && next_y_pos <= Y_Pos + (double)Height)
	{
		Destroy();
		return true;
	}
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Begin_Movement()
{
	// Not used
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Finish_Movement()
{
	if (!Is_Active())
		return;

	Redraw_Monster();
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Advance(double max_speed)
{
	int i;
	double original_direction;
	double next_step;
	double next_x_pos, next_y_pos;
	bool changed_direction = false;
	RECT monster_rect;

	if (! (Monster_State == EMonster_State::Emitting || Monster_State == EMonster_State::Alive) )
		return;

	next_step = Speed / max_speed * AsConfig::Moving_Step_Size;

	original_direction = Direction;

	for (i = 0; i < 16; ++i)
	{
		next_x_pos = X_Pos + next_step * cos(Direction);
		next_y_pos = Y_Pos - next_step * sin(Direction);

		Get_Monster_Rect(next_x_pos, next_y_pos, monster_rect);

		if (AsLevel::Has_Brick_At(monster_rect))
			Direction += M_PI / 8.0;
		else
		{
			changed_direction = true;
			break;
		}
	}

	if (! changed_direction)
	{
		Direction = original_direction - M_PI;
		return;
	}

	// This code will restrict monster movement to level border.
	if (Monster_State == EMonster_State::Alive)
	{
		if (next_x_pos < (double)AsConfig::Level_X_Offset)
			next_x_pos = (double)AsConfig::Level_X_Offset;

		if (next_x_pos + (double)Width > (double)AsConfig::Max_X_Pos)
			next_x_pos = (double)AsConfig::Max_X_Pos - (double)Width;

		if (next_y_pos < (double)AsConfig::Level_Y_Offset)
			next_y_pos = (double)AsConfig::Level_Y_Offset;

		if (next_y_pos + (double)Height > (double)AsConfig::Floor_Y_Pos)
			next_y_pos = (double)AsConfig::Floor_Y_Pos - (double)Height;
	}

	Y_Pos = next_y_pos;
	X_Pos = next_x_pos;
}
//------------------------------------------------------------------------------------------------------------
double AMonster::Get_Speed()
{
	return Speed;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Act()
{
	switch (Monster_State)
	{
	case EMonster_State::Missing:
		return;

	case EMonster_State::Emitting:
		if (Monster_Is_Alive_Timer < AsConfig::Current_Timer_Tick)
			Monster_State = EMonster_State::Alive;
		return;

	case EMonster_State::Alive:
		Act_Alive();
		break;

	case EMonster_State::Destroying:
		Act_Destroying();
		break;

	default:
		AsConfig::Throw();
		break;
	}

	AsTools::Invalidate_Rect(Monster_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Clear(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Previous_Monster_Rect))
		return;

	AsTools::Ellipse(hdc, Previous_Monster_Rect, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Draw(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Monster_Rect))
		return;

	switch (Monster_State)
	{
	case EMonster_State::Missing:
		break;

	case EMonster_State::Emitting:
	case EMonster_State::Alive:
		Draw_Alive(hdc);
		break;

	case EMonster_State::Destroying:
		Draw_Destroying(hdc, paint_area);
		break;

	default:
		AsConfig::Throw();
		break;
	}
}
//------------------------------------------------------------------------------------------------------------
bool AMonster::Is_Finished()
{
	//!!! TODO
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Activate(int x_pos, int y_pos, bool moving_right)
{
	int i;
	int tick_offset;
	int random_speed;
	int emitting_offset;
	double current_timeout = 0.0;

	Monster_State = EMonster_State::Emitting;

	X_Pos = x_pos;
	Y_Pos = y_pos;

	random_speed = AsTools::Rand(5) + 1;

	Speed = (double)random_speed / 10.0;

	emitting_offset = (int)((double)AGate::Width / Speed);

	Monster_Is_Alive_Timer = AsConfig::Current_Timer_Tick + emitting_offset;

	if (moving_right)
		Direction = 0.0;
	else
		Direction = M_PI;

	// Blink animation ticks calculation
	current_timeout;
	Start_Blinking_Time = AsConfig::Current_Timer_Tick;

	for (i = 0; i < Blink_Stages_Count; i++)
	{
		current_timeout += Blinking_Timeouts[i];

		tick_offset = (int)((double)AsConfig::FPS * current_timeout);
		Blink_Ticks[i] = tick_offset;
	}

	Total_Animation_Time = tick_offset;

	Redraw_Monster();
}
//------------------------------------------------------------------------------------------------------------
bool AMonster::Is_Active()
{
	if (Monster_State == EMonster_State::Missing)
		return false;
	else
		return true;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Destroy()
{
	bool is_red;

	int scale = AsConfig::Global_Scale;
	double d_scale = AsConfig::D_Global_Scale;

	int half_width = Width * scale / 2;
	int half_height = Height * scale / 2;
	int i;
	int x_pos = (int)(X_Pos * d_scale) + half_width;
	int y_pos = (int)(Y_Pos * d_scale) + half_height;
	int x_offset, y_offset;
	int size, half_size, remained_size;
	int time_offset;

	half_size = half_width;

	if (half_height < half_size)
		half_size = half_height;

	for (i = 0; i < Explosive_Balls_Count; i++)
	{
		x_offset = AsTools::Rand(half_width) - half_width / 2;
		y_offset = AsTools::Rand(half_height) - half_height / 2;

		remained_size = half_size - (int)sqrt(x_offset * x_offset + y_offset * y_offset);

		size = AsTools::Rand(remained_size / 2) + remained_size;

		if (size < scale)
			size = scale;

		time_offset = AsTools::Rand(AsConfig::FPS * 3 / 2);

		is_red = (bool)AsTools::Rand(2);
		Explosive_Balls[i].Explode(x_pos + x_offset, y_pos + y_offset, size, is_red, time_offset, 10);
	}

	Monster_State = EMonster_State::Destroying;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Draw_Alive(HDC hdc)
{
	const int scale = AsConfig::Global_Scale;
	const double d_scale = AsConfig::D_Global_Scale;
	const int half_scale = scale / 2;

	HRGN region;
	RECT rect, cornea_rect;

	if (Monster_State == EMonster_State::Missing)
		return;

	// 1. Draw background
	// 1.1 Creating a rect to draw background
	rect = Monster_Rect;
	++rect.right;
	++rect.bottom;

	region = CreateEllipticRgnIndirect(&rect);
	SelectClipRgn(hdc, region);

	// 1.2 Dark red part of the BG of the "eye" monster
	AsTools::Ellipse(hdc, Monster_Rect, AsConfig::Dark_Red_Color);

	// 1.3 Red part of the BG of the "eye" monster
	rect = Monster_Rect;
	rect.left -= 2 * scale;
	rect.top -= 3 * scale;
	rect.right -= 2 * scale;
	rect.bottom -= 3 * scale;

	AsTools::Ellipse(hdc, rect, AsConfig::Red_Color);

	SelectClipRgn(hdc, 0);
	DeleteObject(region);

	// 2 Draw the eye itself
	if (Eye_State == EEye_State::Closed)
		return;

	// 2.1 Draw the cornea of the "eye" monster

	cornea_rect = Monster_Rect;
	cornea_rect.left += scale + half_scale;
	cornea_rect.top += 2 * scale + (int)((Max_Cornea_Height / 2.0 - Cornea_Height / 2.0) * d_scale);
	cornea_rect.right -= scale + half_scale;
	cornea_rect.bottom = (int)(Cornea_Height * d_scale) + cornea_rect.top;

	// 2.2 Limiting the output of the monster's eye to the size of the cornea
	region = CreateEllipticRgnIndirect(&cornea_rect);
	SelectClipRgn(hdc, region);


	AsTools::Ellipse(hdc, cornea_rect, AsConfig::Eye_Monster_Cornea_Color);

	// 2.3 Draw the iris of the "eye" monster
	rect = Monster_Rect;
	rect.left += 4 * scale + half_scale;
	rect.top += 4 * scale;
	rect.right -= 4 * scale + half_scale;
	rect.bottom -= 5 * scale;

	AsTools::Ellipse(hdc, rect, AsConfig::Eye_Monster_Iris_Color);

	// 2.4 Draw the pupil of the "eye" monster
	rect = Monster_Rect;
	rect.left += 7 * scale;
	rect.top += 6 * scale;
	rect.right = rect.left + 2 * scale;
	rect.bottom = rect.top + 3 * scale;

	AsTools::Ellipse(hdc, rect, AsConfig::BG_Color);

	SelectClipRgn(hdc, 0);
	DeleteObject(region);

	// 2.5 Outlining the cornea
	AsConfig::BG_Outline_Color.Select_Pen(hdc);

	Arc(hdc, cornea_rect.left, cornea_rect.top, cornea_rect.right - 1, cornea_rect.bottom - 1, 0, 0, 0, 0);
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Draw_Destroying(HDC hdc, RECT& paint_area)
{
	int i;

	for (i = 0; i < Explosive_Balls_Count; i++)
		Explosive_Balls[i].Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Act_Alive()
{
	int i;
	int current_tick_offset, previous_tick;
	double ratio;
	double direction_delta;

	if (Monster_State == EMonster_State::Missing)
		return;

	current_tick_offset = (AsConfig::Current_Timer_Tick - Start_Blinking_Time) % Total_Animation_Time;

	for (i = 0; i < Blink_Stages_Count; i++)
	{
		if (current_tick_offset < Blink_Ticks[i])
		{
			Eye_State = Blinking_States[i];
			break;
		}
	}

	if (i == 0)
		previous_tick = 0;
	else
		previous_tick = Blink_Ticks[i - 1];

	ratio = (double)(current_tick_offset - previous_tick) / (double)(Blink_Ticks[i] - previous_tick);

	switch (Eye_State)
	{
	case EEye_State::Closed:
		Cornea_Height = 0.0;
		break;

	case EEye_State::Opening:
		Cornea_Height = Max_Cornea_Height * ratio;
		break;

	case EEye_State::Staring:
		Cornea_Height = Max_Cornea_Height;
		break;

	case EEye_State::Closing:
		Cornea_Height = Max_Cornea_Height * (1.0 - ratio);
		break;

	default:
		AsConfig::Throw();
		break;
	}

	if (AsConfig::Current_Timer_Tick > Next_Direction_Switch_Tick)
	{
		Next_Direction_Switch_Tick += AsTools::Rand(AsConfig::FPS);

		// Random direction of movement in the range of +/- 45 degrees.
		direction_delta = (double)(AsTools::Rand(90) - 45) * M_PI / 180.0;

		Direction += direction_delta;
	}
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Act_Destroying()
{
	int i;
	bool destroyed = true;

	for (i = 0; i < Explosive_Balls_Count; i++)
	{
		Explosive_Balls[i].Act();

		destroyed &= Explosive_Balls[i].Is_Finished();
	}

	if (destroyed)
		Monster_State = EMonster_State::Missing;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Get_Monster_Rect(double x_pos, double y_pos, RECT& rect)
{
	const int scale = AsConfig::Global_Scale;
	const double d_scale = AsConfig::D_Global_Scale;

	rect.left = (int)(x_pos * d_scale);
	rect.top = (int)(y_pos * d_scale);
	rect.right = rect.left + Width * scale;
	rect.bottom = rect.top + Height * scale;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Redraw_Monster()
{
	Previous_Monster_Rect = Monster_Rect;

	Get_Monster_Rect(X_Pos, Y_Pos, Monster_Rect);

	AsTools::Invalidate_Rect(Monster_Rect);
	AsTools::Invalidate_Rect(Previous_Monster_Rect);
}
//------------------------------------------------------------------------------------------------------------