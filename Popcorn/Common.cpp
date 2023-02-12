#include "Common.h"

// AHit_Checker
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker::Check_Hit(double next_x_pos, double next_y_pos)
{
	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker::Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius, double& x)
{//This function checks the intersection of the horizontal segment (passing from left_x to right_x through y) with a circle of radius "radius"

	double min_x, max_x;
	// x*x + y*y = R*R

	// x*x = R*R - y*y

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




// AHit_Checker_List
//------------------------------------------------------------------------------------------------------------
AHit_Checker_List::AHit_Checker_List()
	: Hit_Checkers_Count(0), Hit_Checkers{}
{
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker_List::Add_Hit_Checker (AHit_Checker* hit_checker)
{
	if (Hit_Checkers_Count >= sizeof(Hit_Checkers) / sizeof(Hit_Checkers[0]))
		return false;

	Hit_Checkers[Hit_Checkers_Count++] = hit_checker;

	return true;
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker_List::Check_Hit(double x_pos, double y_pos, ABall_Object* ball)
{
	int i;

	for (i = 0; i < Hit_Checkers_Count; i++)
		if (Hit_Checkers[i]->Check_Hit(x_pos, y_pos, ball))
			return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AHit_Checker_List::Check_Hit(double x_pos, double y_pos)
{
	int i;

	for (i = 0; i < Hit_Checkers_Count; i++)
		if (Hit_Checkers[i]->Check_Hit(x_pos, y_pos))
			return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------




// AMover
//------------------------------------------------------------------------------------------------------------
AMover::~AMover()
{
}
//------------------------------------------------------------------------------------------------------------




// AGraphics_Object
//------------------------------------------------------------------------------------------------------------
AGraphics_Object::~AGraphics_Object()
{
}
//------------------------------------------------------------------------------------------------------------




// AColor
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




// AGame_Objects_Set
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Begin_Movement()
{
	int index = 0;
	AGame_Object* object;

	while (Get_Next_GameObject(index, &object) )
		object->Begin_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Finish_Movement()
{
	int index = 0;
	AGame_Object* object;

	while (Get_Next_GameObject(index, &object))
		object->Finish_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Advance(double max_speed)
{
	int index = 0;
	AGame_Object* object;

	while (Get_Next_GameObject(index, &object))
		object->Advance(max_speed);
}
//------------------------------------------------------------------------------------------------------------
double AGame_Objects_Set::Get_Speed()
{
	int index = 0;
	AGame_Object* object;
	double max_speed = 0.0, current_speed;

	while (Get_Next_GameObject(index, &object))
	{
		current_speed = object->Get_Speed();

		if (current_speed > max_speed)
			max_speed = current_speed;
	}

	return max_speed;
}
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Act()
{
	int index = 0;
	AGame_Object* object;

	while (Get_Next_GameObject(index, &object))
		object->Act();
}
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Clear(HDC hdc, RECT& paint_area)
{
	int index = 0;
	AGame_Object* object;

	while (Get_Next_GameObject(index, &object))
		object->Clear(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Draw(HDC hdc, RECT& paint_area)
{
	int index = 0;
	AGame_Object* object;

	while (Get_Next_GameObject(index, &object))
		object->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
bool AGame_Objects_Set::Is_Finished()
{
	return false; 	// Not used
}
//------------------------------------------------------------------------------------------------------------
