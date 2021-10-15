#pragma once
#include <Windows.h>


//------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:
	static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);

	static const int Global_Scale = 3;
};
//------------------------------------------------------------------------------------------------------------
