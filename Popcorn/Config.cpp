#include "Config.h"

// AColor
//------------------------------------------------------------------------------------------------------------
AColor::~AColor()
{
	if (Pen != 0)
		DeleteObject(Pen);

	if (Brush != 0)
		DeleteObject(Brush);
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor()
	: R(0), G(0), B(0), Pen(0), Brush(0)
{
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
	: R(r), G(g), B(b), Pen(0), Brush(0)
{
	Pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	Brush = CreateSolidBrush(RGB(r, g, b));
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor(const AColor& color, int pen_size)
	: R(color.R), G(color.G), B(color.B), Pen(0), Brush(0)
{
	Pen = CreatePen(PS_SOLID, pen_size, color.Get_RGB());
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor(unsigned char r, unsigned char g, unsigned char b, int pen_size)
	: R(r), G(g), B(b), Pen(0), Brush(0)
{
	Pen = CreatePen(PS_SOLID, pen_size, RGB(r, g, b));
}
//------------------------------------------------------------------------------------------------------------
AColor::AColor(const AColor& pen_color, const AColor& brush_color, int pen_size)
	: R(0), G(0), B(0), Pen(0), Brush(0)
{
	Pen = CreatePen(PS_SOLID, pen_size, pen_color.Get_RGB());
	Brush = CreateSolidBrush(brush_color.Get_RGB());
}
//------------------------------------------------------------------------------------------------------------
void AColor::operator = (const AColor& another)
{
	AsConfig::Throw(); // can't set value throuh = operator. Use Set_As();
}
//------------------------------------------------------------------------------------------------------------
void AColor::Set_As(unsigned char r, unsigned char g, unsigned char b)
{
	R = r;
	G = g;
	B = b;

	if (Pen != 0)
		DeleteObject(Pen);

	if (Brush != 0)
		DeleteObject(Brush);

	Pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	Brush = CreateSolidBrush(RGB(r, g, b));
}
//------------------------------------------------------------------------------------------------------------
int AColor::Get_RGB() const
{
	return RGB(R, G, B);
}
//------------------------------------------------------------------------------------------------------------
void AColor::Select(HDC hdc) const
{
	SelectObject(hdc, Pen);
	SelectObject(hdc, Brush);
}
//------------------------------------------------------------------------------------------------------------
void AColor::Select_Pen(HDC hdc) const
{
	SelectObject(hdc, Pen);
}
//------------------------------------------------------------------------------------------------------------
HBRUSH AColor::Get_Brush() const
{
	return Brush;
}
//------------------------------------------------------------------------------------------------------------




//AColor_Fade
//------------------------------------------------------------------------------------------------------------
AColor_Fade::~AColor_Fade()
{
	for (auto* color : Fading_Colors)
		delete color;

	Fading_Colors.erase(Fading_Colors.begin(), Fading_Colors.end());
}
//------------------------------------------------------------------------------------------------------------
AColor_Fade::AColor_Fade(const AColor& color, int max_fade_steps)
{
	int i;
	AColor* current_color;

	for (i = 0; i < max_fade_steps; i++)
	{
		current_color = AsTools::Get_Fading_Color(color, i, max_fade_steps);
		Fading_Colors.push_back(current_color);
	}

}
//------------------------------------------------------------------------------------------------------------
AColor_Fade::AColor_Fade(const AColor& origin_color, const AColor& base_color, int max_fade_steps)
{
	int i;
	AColor* current_color;

	for (i = 0; i < max_fade_steps; i++)
	{
		current_color = AsTools::Get_Fading_Color(origin_color, base_color, i, max_fade_steps);
		Fading_Colors.push_back(current_color);
	}

}
//------------------------------------------------------------------------------------------------------------
AColor* AColor_Fade::Get_Color(int fade_step)
{
	if (fade_step < 0 || fade_step >(int)Fading_Colors.size())
		AsConfig::Throw();

	return Fading_Colors[fade_step];
}
//------------------------------------------------------------------------------------------------------------




// AFont
//------------------------------------------------------------------------------------------------------------
AFont::~AFont()
{
	if (Font_Handle)
		DeleteObject(Font_Handle);
}
//------------------------------------------------------------------------------------------------------------
AFont::AFont(int height, int weight, int family, const wchar_t* face_name, bool is_italic)
{
	LOGFONT log_font{};

	log_font.lfHeight = height;
	log_font.lfWeight = weight;
	log_font.lfOutPrecision = 3;
	log_font.lfClipPrecision = 2;
	log_font.lfQuality = 1;
	log_font.lfPitchAndFamily = family;

	if (is_italic)
		log_font.lfItalic = 255;

	wcscpy_s(log_font.lfFaceName, face_name);
	Font_Handle = CreateFontIndirect(&log_font);
}
//------------------------------------------------------------------------------------------------------------
void AFont::Select(HDC hdc) const
{
	SelectObject(hdc, Font_Handle);
}
//------------------------------------------------------------------------------------------------------------




// AsConfig
//------------------------------------------------------------------------------------------------------------
bool AsConfig::Level_Has_Floor = false;
int AsConfig::Current_Timer_Tick = 0;
HWND AsConfig::Hwnd;

const AColor AsConfig::BG_Color(30, 60, 10);
const AColor AsConfig::Red_Color(255, 85, 85);
const AColor AsConfig::Blue_Color(85, 255, 255);
const AColor AsConfig::White_Color(255, 255, 255);
const AColor AsConfig::Letter_Color(White_Color, Global_Scale);
const AColor AsConfig::Laser_Color(White_Color, Global_Scale);
const AColor AsConfig::Gate_Color(White_Color, Global_Scale);
const AColor AsConfig::Unbreakable_Blue_Highlight(Blue_Color, Global_Scale);
const AColor AsConfig::Unbreakable_Red_Highlight(Red_Color, 3 * Global_Scale);
const AColor AsConfig::Teleport_Portal_Color(Blue_Color, BG_Color, Global_Scale);
const AColor AsConfig::Advertisement_Blue_Table(0, 159, 159, Global_Scale);
const AColor AsConfig::Advertisement_Red_Table(Red_Color, 2 * Global_Scale);
const AColor AsConfig::Dark_Red_Color(190, 30, 30);
const AColor AsConfig::Comet_Monster_Tail(Dark_Red_Color, Global_Scale);
const AColor AsConfig::Eye_Monster_Cornea_Color(BG_Color, White_Color, Global_Scale * 2 / 3);
const AColor AsConfig::Eye_Monster_Iris_Color(BG_Color, Blue_Color, Global_Scale * 2 / 3);
const AColor AsConfig::BG_Outline_Color(BG_Color, Global_Scale * 2 / 3);
const AColor AsConfig::Explosion_Red_Color(White_Color, Red_Color, 0);
const AColor AsConfig::Explosion_Blue_Color(White_Color, Blue_Color, 0);
const AColor AsConfig::Shadow_Color(BG_Color, Global_Scale);
const AColor AsConfig::Highlight_Color(White_Color, Global_Scale);

const AFont AsConfig::Name_Font(-48, 700, 49, L"Consolas");
const AFont AsConfig::Score_Font(-44, 700, 49, L"Consolas");
const AFont AsConfig::Logo_Corn_Font(-96, 900, 34, L"Arial Black");
const AFont AsConfig::Logo_Pop_Font(-128, 900, 34, L"Arial Black");
const AFont AsConfig::Game_Over_Font(-60, 700, 66, L"Comic Sans MS", true);

const double AsConfig::D_Global_Scale = (double)Global_Scale;
const double AsConfig::Moving_Step_Size = 1.0 / Global_Scale;
const double AsConfig::Start_Ball_Y_Pos = 184.0;
const double AsConfig::Ball_Acceleration = 1.001;
const double AsConfig::Normal_Ball_Speed = 3.0;
const double AsConfig::Min_Ball_Angle = M_PI / 8.0;
const double AsConfig::Ball_Radius = 2.0 - 0.5 / Global_Scale;
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

	return rand() * range / (RAND_MAX + 2);
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
AColor* AsTools::Get_Fading_Color(const AColor& origin_color, int step, int max_step)
{
	return Get_Fading_Color(origin_color, AsConfig::BG_Color, step, max_step);
}
//------------------------------------------------------------------------------------------------------------
AColor* AsTools::Get_Fading_Color(const AColor& origin_color, const AColor& base_color, int step, int max_step)
{
	unsigned char r, g, b;
	AColor* result_color;

	r = Get_Fading_Channel(origin_color.R, base_color.R, step, max_step);
	g = Get_Fading_Channel(origin_color.G, base_color.G, step, max_step);
	b = Get_Fading_Channel(origin_color.B, base_color.B, step, max_step);

	result_color = new AColor(r, g, b);

	return result_color;
}
//------------------------------------------------------------------------------------------------------------
bool AsTools::Reflect_On_Circle(double next_x_pos, double next_y_pos, double circle_x, double circle_y, double circle_radius, ABall_Object* ball)
{
	double dx, dy;
	double distance, two_radiuses;
	double alpha, beta, gamma;
	double related_ball_direction;
	const double pi_2 = 2.0 * M_PI;

	dx = next_x_pos - circle_x;
	dy = next_y_pos - circle_y;

	distance = sqrt(dx * dx + dy * dy);
	two_radiuses = circle_radius + AsConfig::Ball_Radius;

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




// AHit_Checker
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker::Check_Hit(double next_x_pos, double next_y_pos)
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker::Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius, double& x)
{// Tests the intersection of a horizontal line (passing from left_x to right_x through y) with a circle of radius radius

	double min_x, max_x;

	if (y > radius)
		return false;

	x = sqrt(radius * radius - y * y);

	max_x = next_x_pos + x;
	min_x = next_x_pos - x;

	if (max_x >= left_x && max_x <= right_x || min_x >= left_x && min_x <= right_x)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker::Check_Hit(RECT &rect)
{
	return false;
}
//------------------------------------------------------------------------------------------------------------




// AHit_Checker_List
//------------------------------------------------------------------------------------------------------------
AHit_Checker_List::AHit_Checker_List()
	: Hit_Checkers_Count(0), Hit_Checkers{}
{
}
//------------------------------------------------------------------------------------------------------------
void AHit_Checker_List::Add_Hit_Checker(AHit_Checker* hit_checker)
{
	Hit_Checkers.push_back(hit_checker);
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker_List::Check_Hit(double x_pos, double y_pos, ABall_Object* ball)
{
	for (auto* hit_checker : Hit_Checkers)
		if (hit_checker->Check_Hit(x_pos, y_pos, ball))
				return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker_List::Check_Hit(double x_pos, double y_pos)
{
	for (auto* hit_checker : Hit_Checkers)
		if (hit_checker->Check_Hit(x_pos, y_pos))
			return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker_List::Check_Hit(RECT& rect)
{
	for (auto* hit_checker : Hit_Checkers)
		if (hit_checker->Check_Hit(rect) )
			return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------