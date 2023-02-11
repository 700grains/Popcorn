#include "Config.h"

// AsConfig
bool AsConfig::Level_Has_Floor = true;
int AsConfig::Current_Timer_Tick = 0;

//const AColor AsConfig::BG_Color(15, 63, 31);
const AColor AsConfig::BG_Color(30, 60, 10);
const AColor AsConfig::Red_Color(255, 85, 85);
const AColor AsConfig::Blue_Color(85, 255, 255);
const AColor AsConfig::White_Color(255, 255, 255);
const AColor AsConfig::Letter_Color(AsConfig::White_Color, AsConfig::Global_Scale);
const AColor AsConfig::Laser_Color(AsConfig::White_Color, AsConfig::Global_Scale);
const AColor AsConfig::Gate_Color(AsConfig::White_Color, AsConfig::Global_Scale);
const AColor AsConfig::Unbreakable_Blue_Highlight(AsConfig::Blue_Color, AsConfig::Global_Scale);
const AColor AsConfig::Unbreakable_Red_Highlight(AsConfig::Red_Color, 3 * AsConfig::Global_Scale);
const AColor AsConfig::Teleport_Portal_Color(AsConfig::Blue_Color, AsConfig::BG_Color, AsConfig::Global_Scale);
const AColor AsConfig::Advertisement_Blue_Table(0, 159, 159, AsConfig::Global_Scale);
const AColor AsConfig::Advertisement_Red_Table(AsConfig::Red_Color, 2 * AsConfig::Global_Scale);
const AColor AsConfig::Dark_Red_Color(190, 30, 30);
const AColor AsConfig::Eye_Monster_Cornea_Color(AsConfig::BG_Color, AsConfig::White_Color, AsConfig::Global_Scale * 2 / 3);
const AColor AsConfig::Eye_Monster_Iris_Color(AsConfig::BG_Color, AsConfig::Blue_Color, AsConfig::Global_Scale * 2 / 3);
const AColor AsConfig::BG_Outline_Color(AsConfig::BG_Color, AsConfig::Global_Scale * 2 / 3);
const AColor AsConfig::Explosion_Red_Color(AsConfig::White_Color, AsConfig::Red_Color, 0);
const AColor AsConfig::Explosion_Blue_Color(AsConfig::White_Color, AsConfig::Blue_Color, 0);

HWND AsConfig::Hwnd;

const double AsConfig::D_Global_Scale = (double)Global_Scale;
const double AsConfig::Moving_Step_Size = 1.0 / AsConfig::Global_Scale;
const double AsConfig::Start_Ball_Y_Pos = 184.0;
const double AsConfig::Ball_Acceleration = 1.001;
const double AsConfig::Normal_Ball_Speed = 3.0;
const double AsConfig::Min_Ball_Angle = M_PI / 8.0;
//------------------------------------------------------------------------------------------------------------
void AsConfig::Throw()
{
	throw 13;
}
//------------------------------------------------------------------------------------------------------------




// AsTools
//------------------------------------------------------------------------------------------------------------
int AsTools::Rand(int range)
{// Calculates a pseudo-random number in the range [0, .. range -1]

	return rand() * range / RAND_MAX;
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Round_Rect(HDC hdc, RECT& rect, int corner_radius)
{
	int radius = corner_radius * AsConfig::Global_Scale;

	RoundRect(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1, radius, radius);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Rect(HDC hdc, RECT& rect, const AColor& color)
{
	color.Select(hdc);
	Rectangle(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Rect(HDC hdc, int x_pos, int y_pos, int width, int height, const AColor& color)
{
	int scale = AsConfig::Global_Scale;

	color.Select(hdc);
	Rectangle(hdc, x_pos * scale, y_pos * scale,
		(x_pos + width) * scale - 1, (y_pos + height) * scale - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Ellipse(HDC hdc, RECT& rect, const AColor& color)
{
	color.Select(hdc);
	::Ellipse(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Invalidate_Rect(RECT& rect)
{
	InvalidateRect(AsConfig::Hwnd, &rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
unsigned char AsTools::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step, int max_step)
{
	return color - step * (color - bg_color) / (max_step - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsTools::Get_Fading_Color(const AColor& origin_color, int step, AColor& result_color, int max_step)
{
	unsigned char r, g, b;

	r = Get_Fading_Channel(origin_color.R, AsConfig::BG_Color.R, step, max_step);
	g = Get_Fading_Channel(origin_color.G, AsConfig::BG_Color.G, step, max_step);
	b = Get_Fading_Channel(origin_color.B, AsConfig::BG_Color.B, step, max_step);

	result_color = AColor(r, g, b);
}
//------------------------------------------------------------------------------------------------------------
bool AsTools::Reflect_On_Circle(double next_x_pos, double next_y_pos, double circle_x, double circle_y, double circle_radius, ABall* ball)
{
	double dx, dy;
	//double platform_ball_x, platform_ball_y; //, platform_ball_radius;
	double distance, two_radiuses;
	double alpha, beta, gamma;
	double related_ball_direction;
	const double pi_2 = 2.0 * M_PI;

	//circle_radius = (double)AsConfig::Platform_Circle_Size / 2.0;

	dx = next_x_pos - circle_x;
	dy = next_y_pos - circle_y;

	distance = sqrt(dx * dx + dy * dy);
	two_radiuses = circle_radius + ball->Radius;

	if (distance + AsConfig::Moving_Step_Size < two_radiuses)
	{// The ball touched the side ball

		beta = atan2(-dy, dx);

		related_ball_direction = ball->Get_Direction();
		related_ball_direction -= beta;

		if (related_ball_direction > pi_2)
			related_ball_direction -= pi_2;

		if (related_ball_direction < 0.0)
			related_ball_direction += pi_2;

		if (related_ball_direction > M_PI_2 && related_ball_direction < M_PI + M_PI_2)
		{
			alpha = beta + M_PI - ball->Get_Direction();
			gamma = beta + alpha;

			ball->Set_Direction(gamma);

			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------------------------------------