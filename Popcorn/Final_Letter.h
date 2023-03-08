#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class AFinal_Letter : public AGraphics_Object
{
public:
	AFinal_Letter(double x_pos, double y_pos, const wchar_t letter);
	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Destroy();

	double X_Pos, Y_Pos;

private:
	wchar_t Letter;
};
//------------------------------------------------------------------------------------------------------------