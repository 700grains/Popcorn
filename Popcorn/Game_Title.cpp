#include "Game_Title.h"

// AsGame_Title
const double AsGame_Title::Lowest_Y_Pos = 135;
//------------------------------------------------------------------------------------------------------------
AsGame_Title::~AsGame_Title()
{
	for (auto* letter : Title_Letters)
		delete letter;

	Title_Letters.erase(Title_Letters.begin(), Title_Letters.end());
}
//------------------------------------------------------------------------------------------------------------
AsGame_Title::AsGame_Title()
	: Game_Title_State(EGame_Title_State::Idle), Starting_Tick(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AsGame_Title::Act()
{
	int current_tick;
	int explosion_index;
	double ratio;
	double y_pos;
	const double d_scale = AsConfig::D_Global_Scale;

	if (Game_Title_State == EGame_Title_State::Idle || Game_Title_State == EGame_Title_State::Finished)
		return;

	current_tick = AsConfig::Current_Timer_Tick - Starting_Tick;

	switch (Game_Title_State)
	{
	case EGame_Title_State::Game_Over_Descent:
	case EGame_Title_State::Game_Won_Descent:
		if (current_tick < Descent_Timeout)
			ratio = (double)current_tick / (double)Descent_Timeout;
		else
		{
			ratio = 1.0;
			if (Game_Title_State == EGame_Title_State::Game_Over_Descent)
				Game_Title_State = EGame_Title_State::Game_Over_Show;
			else
				Game_Title_State = EGame_Title_State::Game_Won_Animate;

			Starting_Tick = AsConfig::Current_Timer_Tick;
		}

		y_pos = Lowest_Y_Pos * ratio;

		for (auto* letter : Title_Letters)
			letter->Y_Pos = y_pos;

		Previous_Title_Rect = Title_Rect;

		Title_Rect.top = (int)(y_pos * d_scale);
		Title_Rect.bottom = (int)(Title_Rect.top + Height * d_scale);

		AsTools::Invalidate_Rect(Title_Rect);
		AsTools::Invalidate_Rect(Previous_Title_Rect);
		break;


	case EGame_Title_State::Game_Over_Show:
		if (current_tick > Game_Over_Showing_Timeout)
		{
			Game_Title_State = EGame_Title_State::Game_Over_Destroy;
			Starting_Tick = AsConfig::Current_Timer_Tick;
		}
		break;


	case EGame_Title_State::Game_Over_Destroy:
		if (current_tick % Explosion_Delay == 0)
		{
			explosion_index = current_tick / Explosion_Delay;

			if (explosion_index >= 0 && explosion_index < (int)Title_Letters.size())
				Title_Letters[explosion_index]->Destroy();
			else
				Game_Title_State = EGame_Title_State::Finished;
		}
		break;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsGame_Title::Clear(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;

	if (Game_Title_State == EGame_Title_State::Idle || Game_Title_State == EGame_Title_State::Finished)
		return;

	if (!IntersectRect(&intersection_rect, &paint_area, &Previous_Title_Rect))
		return;

	AsTools::Rect(hdc, Previous_Title_Rect, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
void AsGame_Title::Draw(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;

	if (Game_Title_State == EGame_Title_State::Idle || Game_Title_State == EGame_Title_State::Finished)
		return;

	if (!IntersectRect(&intersection_rect, &paint_area, &Title_Rect))
		return;

	for (auto* letter : Title_Letters)
		letter->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
bool AsGame_Title::Is_Finished()
{
	//!!! TODO
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsGame_Title::Show(bool is_victory)
{
	const double d_scale = AsConfig::D_Global_Scale;
	const int scale = AsConfig::Global_Scale;
	double title_x, title_y;
	double title_x_end;

	title_x = 39.0;
	title_y = 0.0;

	if (!is_victory)
	{
		Title_Letters.push_back(new AFinal_Letter(title_x, title_y, L'G'));
		Title_Letters.push_back(new AFinal_Letter(title_x + 14.0, title_y, L'A'));
		Title_Letters.push_back(new AFinal_Letter(title_x + 30.0, title_y, L'M'));
		Title_Letters.push_back(new AFinal_Letter(title_x + 49.0, title_y, L'E'));

		Title_Letters.push_back(new AFinal_Letter(title_x + 68.0, title_y, L'O'));
		Title_Letters.push_back(new AFinal_Letter(title_x + 85.0, title_y, L'V'));
		Title_Letters.push_back(new AFinal_Letter(title_x + 100.0, title_y, L'E'));
		Title_Letters.push_back(new AFinal_Letter(title_x + 115.0, title_y, L'R'));

		Game_Title_State = EGame_Title_State::Game_Over_Descent;
	}
	else
	{
		Title_Letters.push_back(new AFinal_Letter(title_x, title_y, L'V'));
		Title_Letters.push_back(new AFinal_Letter(title_x + 17.0, title_y, L'I'));
		Title_Letters.push_back(new AFinal_Letter(title_x + 33.0, title_y, L'C'));
		Title_Letters.push_back(new AFinal_Letter(title_x + 50.0, title_y, L'T'));

		Title_Letters.push_back(new AFinal_Letter(title_x + 66.0, title_y, L'O'));
		Title_Letters.push_back(new AFinal_Letter(title_x + 85.0, title_y, L'R'));
		Title_Letters.push_back(new AFinal_Letter(title_x + 100.0, title_y, L'Y'));
		Title_Letters.push_back(new AFinal_Letter(title_x + 115.0, title_y, L'!'));

		Game_Title_State = EGame_Title_State::Game_Won_Descent;
	}

	title_x_end = Title_Letters[Title_Letters.size() - 1]->X_Pos + 16;

	Title_Rect.left = (int)(title_x * d_scale);
	Title_Rect.top = (int)(title_y * d_scale);
	Title_Rect.right = Title_Rect.left + (int)(title_x_end * d_scale);
	Title_Rect.bottom = Title_Rect.top + Height * scale;

	Starting_Tick = AsConfig::Current_Timer_Tick;

	AsTools::Invalidate_Rect(Title_Rect);
}
//------------------------------------------------------------------------------------------------------------
bool AsGame_Title::Is_Visible()
{
	if (Game_Title_State != EGame_Title_State::Idle)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------