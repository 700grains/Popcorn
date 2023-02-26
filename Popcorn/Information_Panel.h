#pragma once

#include "Falling_Letter.h"
#include "Indicator.h"

//------------------------------------------------------------------------------------------------------------
enum class EScore_Event_Type : unsigned char
{
	Hit_Brick,
	Hit_Monster,
	Catch_Letter
};
//------------------------------------------------------------------------------------------------------------
class AsInformation_Panel : public AGame_Object
{
public:
	~AsInformation_Panel();
	AsInformation_Panel();

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Advance(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Init();
	void Add_Life();
	bool Remove_A_Life();

	static void Update_Score(EScore_Event_Type event_type);

	AIndicator Floor_Panel, Monsters_Panel;

private:
	void Choose_Font();
	void Draw_String(HDC hdc, RECT& rect, AString& str, bool name);
	void Show_Extra_Lives(HDC hdc);
	void Draw_Extra_Life(HDC hdc, int x, int y);

	int Lives_Left;
	HFONT Logo_Corn_Font, Logo_Pop_Font, Name_Font, Score_Font;
	AColor* Shadow_Color, * Highlight_Color, * Shaded_Blue, * Dark_Red_Color;

	AFalling_Letter Letter_P, Letter_G, Letter_M;

	AString Player_Name;

	static RECT Logo_Rect;
	static RECT Data_Rect; // score and lives 

	static int Score;

	static const int Logo_X_Pos = 212;
	static const int Logo_Y_Pos = 0;
	static const int Shade_X_Offset = 5;
	static const int Shade_Y_Offset = 6;
	
	static const int Score_X_Pos = 208;
	static const int Score_Y_Pos = 108;
	static const int Score_Width = 110;
	static const int Score_Height = 90;
	static const int Score_Val_Offset = 20;
	static const int Indicator_Y_Offset = 55;
};
//------------------------------------------------------------------------------------------------------------