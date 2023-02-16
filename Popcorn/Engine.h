#pragma once

#include <Windows.h>

#include "Border.h"
#include "Level.h"
#include "Platform.h"
#include "Ball_Set.h"
#include "Monster_Set.h"

//------------------------------------------------------------------------------------------------------------
enum class EKey_Type: unsigned char
{
	Left,
	Right,
	Space
};
//------------------------------------------------------------------------------------------------------------
enum class EGame_State: unsigned char
{
	Test_Ball,

	Play_Level,
	Lost_Ball,
	Restart_Level
};
//------------------------------------------------------------------------------------------------------------
class AsInformation_Panel: public AGame_Object
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
	void Draw_String(HDC hdc, RECT &rect, const wchar_t *str, bool name);

	HFONT Logo_Corn_Font, Logo_Pop_Font, Name_Font, Score_Font;
	AColor *Shadow_Color, *Highlight_Color, *Shaded_Blue, *Dark_Red_Color;

	AFalling_Letter Letter_P, Letter_G, Letter_M;

	static const int Score_X_Pos = 208;
	static const int Score_Y_Pos = 108;
	static const int Score_Width = 110;
	static const int Score_Height = 90;
	static const int Score_Val_Offset = 22;
};
//------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	AsEngine();

	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key(EKey_Type key_type, bool key_down);
	int On_Timer();

	const int Timer_ID;

private:
	void Restart_Level();
	void Play_Level();
	void Advance_Movers();
	void Act();
	void On_Falling_Letter(AFalling_Letter* falling_letter);
	void Add_Next_Module(int &index, AGame_Object* game_object);

	EGame_State Game_State;
	double Rest_Distance;
	int Life_Count;

	AsLevel Level;
	AsPlatform Platform;
	AsBorder Border;
	AsBall_Set Ball_Set; 
	AsLaser_Beam_Set Laser_Beam_Set;
	AsMonster_Set Monster_Set;
	AsInformation_Panel Information_Panel;

	AGame_Object* Modules[AsConfig::Max_Modules_Count]; // UNO; main graphic modules
};
//------------------------------------------------------------------------------------------------------------