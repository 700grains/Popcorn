#pragma once

#include "Falling_Letter.h"

//------------------------------------------------------------------------------------------------------------
enum class EScore_Event_Type : unsigned char
{
	Hit_Brick,
	Hit_Monster,
	Catch_Letter
};
//------------------------------------------------------------------------------------------------------------
class AIndicator : public AGraphics_Object
{
public:
	AIndicator(EMessage_Type message_type, int x_pos, int y_pos);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Restart();
	void Reset();

private:
	EMessage_Type Message_Type;
	bool Need_To_Send_A_Message;
	int X_Pos, Y_Pos;
	int Indicator_Lifetime;
	RECT Indicator_Rect;

	static const int Width = 12;
	static const int Inner_Width = Width - 2;
	static const int Height = 30;
	static const int Inner_Height = Height - 2;
	static const int Indicator_Timeout = AsConfig::FPS * 50; // 50 seconds
};
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

	static void Update_Score(EScore_Event_Type event_type);

	AIndicator Floor_Panel, Monsters_Panel;

private:
	void Choose_Font();
	void Draw_String(HDC hdc, RECT& rect, AString& str, bool name);
	void Show_Extra_Lives(HDC hdc);
	void Draw_Extra_Life(HDC hdc, int x, int y);

	HFONT Logo_Corn_Font, Logo_Pop_Font, Name_Font, Score_Font;
	AColor* Shadow_Color, * Highlight_Color, * Shaded_Blue, * Dark_Red_Color;

	AFalling_Letter Letter_P, Letter_G, Letter_M;

	AString Player_Name;

	static RECT Logo_Rect;
	static RECT Data_Rect; // score and lives 

	static int Score;
	static int Lives_Left;

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