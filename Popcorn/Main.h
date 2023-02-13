#pragma once

#include "resource.h"
#include "Engine.h"

//------------------------------------------------------------------------------------------------------------
class AsFrame_DC
{
public:
	AsFrame_DC();

	HDC Get_DC(HWND hwnd, HDC hdc);

	int Width, Height;

private:
	HBITMAP Frame_Bitmap;
	HDC Frame_DC;
};
//------------------------------------------------------------------------------------------------------------