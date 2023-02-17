#pragma once

#include "Config.h"
#include "Falling_Letter.h"

//------------------------------------------------------------------------------------------------------------
class AsInformation_Panel : public AGame_Object
{
public:
	~AsInformation_Panel();
	AsInformation_Panel();

	virtual void Begin_Movement() override;
	virtual void Finish_Movement() override;
	virtual void Advance(double max_speed) override;
	virtual double Get_Speed() override;

	virtual void Act() override;
	virtual void Clear(HDC hdc, RECT& paint_area) override;
	virtual void Draw(HDC hdc, RECT& paint_area) override;
	virtual bool Is_Finished() override;

	void Init();

private:
	void Choose_Font();
	void Draw_String(HDC hdc, RECT& rect, AString& str, bool name);
	void Show_Extra_Lives(HDC hdc);
	void Draw_Extra_Life(HDC hdc, int x, int y);

	HFONT Logo_Corn_Font, Logo_Pop_Font, Name_Font, Score_Font;
	AColor* Shadow_Color, * Highlight_Color, * Shaded_Blue, * Dark_Red_Color;

	AFalling_Letter Letter_P, Letter_G, Letter_M;

	AString Player_Name;

	static const int Score_X_Pos = 208;
	static const int Score_Y_Pos = 108;
	static const int Score_Width = 110;
	static const int Score_Height = 90;
	static const int Score_Val_Offset = 20;
};
//------------------------------------------------------------------------------------------------------------