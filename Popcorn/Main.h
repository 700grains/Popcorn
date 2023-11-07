#pragma once

#include "resource.h"
#include "Engine.h"
#include "framework.h"

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
	AsMain_Window();

	int APIENTRY Main(HINSTANCE instance, int cmd_show);

private:
	AsEngine Engine;
	HINSTANCE Instance;
	AString Title, Window_Class_Title;
	AsFrame_DC DC;

	ATOM Register_Class();
	BOOL Init_Instance(int cmd_show);
	void On_Paint(HWND hwnd);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static AsMain_Window* Self;
	static const int MAX_LOADSTRING = 100;
};
//------------------------------------------------------------------------------------------------------------