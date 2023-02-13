#pragma once

#include "resource.h"
#include "Engine.h"

//------------------------------------------------------------------------------------------------------------
class AsFrame_DC
{
public:
	~AsFrame_DC();
	AsFrame_DC();
	HDC Get_DC(HWND hwnd, HDC hdc);

	int Width, Height;

private:
	HBITMAP Bitmap;
	HDC DC;
};
//------------------------------------------------------------------------------------------------------------
class AsMain_Window
{
public:
	int APIENTRY Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);


private:

};
//------------------------------------------------------------------------------------------------------------