#include "Tools.h"

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




