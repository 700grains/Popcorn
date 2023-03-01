#pragma once

//------------------------------------------------------------------------------------------------------------
class AMop_Indicators : public AGraphics_Object
{
public:
	AMop_Indicators(int x_pos, int y_pos, int time_offset);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Set_Y_Pos(int y_pos);

private:
	int X_Pos, Y_Pos;
	RECT Indicator_Rect;
	const AColor* Current_Color;
	int Time_Offset;

	static const int Width = 17;
	static const int Height = 5;
	static const int Max_Fade_Step = AsConfig::FPS * 4 / 10; // 0.4 seconds 
	static const int Normal_Timeout = AsConfig::FPS; // 1 sec

	static AColor_Fade Fading_Blue_Colors;
};
//------------------------------------------------------------------------------------------------------------