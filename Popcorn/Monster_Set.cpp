#include "Monster_Set.h"




//AExplosive_Ball
AColor AExplosive_Ball::Fading_Red_Colors[Max_Fade_Step];
//------------------------------------------------------------------------------------------------------------
AExplosive_Ball::AExplosive_Ball()
	:Explosive_Ball_State(EExplosive_Ball_State::Idle),  X_Pos(0), Y_Pos(0), Step_Count(0), Start_Fading_Tick(0), Max_Size(0.0), Size(0.0), Size_Step(0.0), Ball_Rect{}
{

}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Act()
{
	switch (Explosive_Ball_State)
	{
	case EExplosive_Ball_State::Idle:
		break;


	case EExplosive_Ball_State::Expanding:
		Size += Size_Step;

		if (Size > Max_Size)
		{
			Explosive_Ball_State = EExplosive_Ball_State::Fading;
			Start_Fading_Tick = AsConfig::Current_Timer_Tick;
		}
		else
			Update_Ball_Rect();
		break;


	case EExplosive_Ball_State::Fading:
		if (AsConfig::Current_Timer_Tick >= Start_Fading_Tick + Fading_Time)
			Explosive_Ball_State = EExplosive_Ball_State::Idle;
		break;

	default:
		AsConfig::Throw();
		break;
	}
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Clear(HDC hdc, RECT& paint_area)
{
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Draw(HDC hdc, RECT& paint_area)
{
	int current_time_interval;
	double ratio;

	switch (Explosive_Ball_State)
	{
	case EExplosive_Ball_State::Idle:
		break;


	case EExplosive_Ball_State::Expanding:
		AsTools::Ellipse(hdc, Ball_Rect, AsConfig::Dark_Red_Color);
		break;


	case EExplosive_Ball_State::Fading:
		current_time_interval = AsConfig::Current_Timer_Tick - Start_Fading_Tick;

		if (current_time_interval > Fading_Time)
			current_time_interval = Fading_Time;

		ratio = current_time_interval / Fading_Time;
		break;


	default:
		break;
	}
}
//------------------------------------------------------------------------------------------------------------
bool AExplosive_Ball::Is_Finished()
{
	//!!! TODO
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Explode(int x_pos, int y_pos, int size, int step_count)
{
	Explosive_Ball_State = EExplosive_Ball_State::Expanding;

	X_Pos = x_pos;
	Y_Pos = y_pos;
	Max_Size = size;
	Size = 0.0;
	Step_Count = step_count;

	Size_Step = (double)Max_Size / (double)Step_Count;

	Update_Ball_Rect();
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Setup_Colors()
{
	int i;

	for (i = 0; i < Max_Fade_Step; i++)
		AsTools::Get_Fading_Color(AsConfig::Red_Color, i, Fading_Red_Colors[i], Max_Fade_Step);
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Update_Ball_Rect()
{
	Ball_Rect.left = X_Pos - (int)(Size / 2.0);
	Ball_Rect.top = Y_Pos - (int)(Size / 2.0);
	Ball_Rect.right = Ball_Rect.left + (int)Size;
	Ball_Rect.bottom = Ball_Rect.top + (int)Size;
}
//------------------------------------------------------------------------------------------------------------




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
	:Eye_State(EEye_State::Closed), Monster_State(EMonster_State::Missing), X_Pos(0), Y_Pos(0), Blink_Ticks{}, Cornea_Height(Max_Cornea_Height), Start_Blinking_Time(0), Total_Animation_Time(0), Monster_Rect{}
{
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Begin_Movement()
{
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Finish_Movement()
{
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Advance(double max_speed)
{
	//!!! TODO
}
//------------------------------------------------------------------------------------------------------------
double AMonster::Get_Speed()
{
	//!!! TODO
	return 0.0;
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Act()
{
	switch (Monster_State)
	{
	case EMonster_State::Missing:
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
	//!!! TODO
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
void AMonster::Activate(int x_pos, int y_pos)
{
	int i;
	int tick_offset;
	double current_timeout = 0.0;
	const int scale = AsConfig::Global_Scale;

	Monster_State = EMonster_State::Alive;

	X_Pos = x_pos + 10;
	Y_Pos = y_pos;

	Monster_Rect.left = X_Pos * scale;
	Monster_Rect.top = Y_Pos * scale;
	Monster_Rect.right = Monster_Rect.left + Width * scale;
	Monster_Rect.bottom = Monster_Rect.top + Height * scale;

	// Blink animation tick calculation
	current_timeout;
	Start_Blinking_Time = AsConfig::Current_Timer_Tick;

	for (i = 0; i < Blink_Stages_Count; i++)
	{
		current_timeout += Blinking_Timeouts[i];

		tick_offset = (int)((double)AsConfig::FPS * current_timeout);
		Blink_Ticks[i] = tick_offset;
	}

	Total_Animation_Time = tick_offset;
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
	Monster_State = EMonster_State::Destroying;

	Explosive_Balls[0].Explode(Monster_Rect.left + 20, Monster_Rect.top + 20, 30, 55);
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
}
//------------------------------------------------------------------------------------------------------------
void AMonster::Act_Destroying()
{
	int i;

	for (i = 0; i < Explosive_Balls_Count; i++)
		Explosive_Balls[i].Act();
}
//------------------------------------------------------------------------------------------------------------




// AsMonster_Set
//------------------------------------------------------------------------------------------------------------
AsMonster_Set::AsMonster_Set()
	: Border(nullptr)
{
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Init(AsBorder* border)
{
	Border = border;
}
//------------------------------------------------------------------------------------------------------------
void AsMonster_Set::Emit_At_Gate(int gate_index)
{
	int i;
	int gate_x, gate_y;

	AMonster* monster = 0;

	if (gate_index < 0 || gate_index >= AsConfig::Gates_Count)
		AsConfig::Throw();

	for (i = 0; i < Max_Monsters_Count; i++)
	{
		if (! Monsters[i].Is_Active())
		{
			monster = &Monsters[i];
			break;
		}
	}

	if (monster == 0)
		return; // There are no unused monsters.

	Border->Get_Gate_Pos(gate_index, gate_x, gate_y);

	monster->Activate(gate_x, gate_y);

	monster->Destroy();
}
//------------------------------------------------------------------------------------------------------------
bool AsMonster_Set::Get_Next_GameObject(int& index, AGame_Object** game_object)
{
	if (index < 0 || index >= AsConfig::Max_Balls_Count)
		return false;

	*game_object = &Monsters[index++];

	return true;
}
//------------------------------------------------------------------------------------------------------------
