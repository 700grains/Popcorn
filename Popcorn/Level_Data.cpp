#include "Level_Data.h"

// ALevel_Data
char* ALevel_Data::Levels_Array[Max_Level_Number] =
{
	(char*)ALevel_Data::Level_01,
	(char*)ALevel_Data::Level_02,
	(char*)ALevel_Data::Level_03,
	(char*)ALevel_Data::Level_04,
	(char*)ALevel_Data::Level_05,
	(char*)ALevel_Data::Level_06,
	(char*)ALevel_Data::Level_07,
	(char*)ALevel_Data::Level_08,
	(char*)ALevel_Data::Level_09,
	(char*)ALevel_Data::Level_10
};
//------------------------------------------------------------------------------------------------------------
ALevel_Data::ALevel_Data(int level_number)
	: Level(nullptr), Advertisement(nullptr)
{
	if (level_number < 1 || level_number > Max_Level_Number)
		AsConfig::Throw();

	Level = Levels_Array[level_number - 1];
}
//------------------------------------------------------------------------------------------------------------
char ALevel_Data::Test_Level[AsConfig::Level_Height][AsConfig::Level_Width] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//------------------------------------------------------------------------------------------------------------
char ALevel_Data::Level_01[AsConfig::Level_Height][AsConfig::Level_Width] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//------------------------------------------------------------------------------------------------------------
char ALevel_Data::Level_02[AsConfig::Level_Height][AsConfig::Level_Width] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2,
	2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2,
	2, 1, 2, 1, 2, 2, 2, 2, 1, 2, 1, 2,
	2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2,
	2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2,
	2, 1, 2, 1, 2, 1, 1, 2, 1, 2, 1, 2,
	2, 1, 2, 1, 2, 2, 2, 2, 1, 2, 1, 2,
	2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2,
	2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//------------------------------------------------------------------------------------------------------------
char ALevel_Data::Level_03[AsConfig::Level_Height][AsConfig::Level_Width] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//------------------------------------------------------------------------------------------------------------
char ALevel_Data::Level_04[AsConfig::Level_Height][AsConfig::Level_Width] =
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
	1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
	1, 2, 1, 0, 0, 0, 0, 0, 0, 1, 2, 1,
	1, 2, 1, 0, 0, 0, 0, 0, 0, 1, 2, 1,
	1, 2, 1, 0, 0, 0, 0, 0, 0, 1, 2, 1,
	1, 2, 1, 0, 0, 0, 0, 0, 0, 1, 2, 1,
	1, 2, 1, 0, 0, 0, 0, 0, 0, 1, 2, 1,
	1, 2, 6, 6, 6, 6, 6, 6, 6, 6, 2, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//------------------------------------------------------------------------------------------------------------
char ALevel_Data::Level_05[AsConfig::Level_Height][AsConfig::Level_Width] =
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1,
	7, 7, 7, 7, 7, 4, 4, 7, 7, 7, 7, 7,
	1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1,
	2, 2, 3, 0, 0, 0, 0, 0, 0, 3, 2, 2,
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//------------------------------------------------------------------------------------------------------------
char ALevel_Data::Level_06[AsConfig::Level_Height][AsConfig::Level_Width] =
{
	0, 0, 0, 0, 0, 0, 1, 1, 1, 3, 3, 3,
	0, 0, 0, 0, 0, 1, 1, 1, 1, 3, 3, 3,
	0, 0, 0, 0, 1, 1, 1, 1, 1, 3, 3, 3,
	0, 0, 0, 1, 1, 1, 1, 1, 1, 3, 3, 3,
	0, 0, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3,
	1, 1, 1, 1, 1, 1, 1, 1, 0, 3, 3, 3,
	1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3
};
//------------------------------------------------------------------------------------------------------------
char ALevel_Data::Level_07[AsConfig::Level_Height][AsConfig::Level_Width] =
{
	4, 0, 4, 0, 4, 0, 4, 0, 4, 0, 4, 0,
	0, 4, 0, 4, 0, 4, 0, 4, 0, 4, 0, 4,
	4, 0, 4, 0, 4, 0, 4, 0, 4, 0, 4, 0,
	0, 4, 0, 4, 0, 4, 0, 4, 0, 4, 0, 4,
	4, 0, 4, 0, 4, 0, 4, 0, 4, 0, 4, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 8, 3, 8, 3, 5, 5, 3, 8, 3, 8, 3,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//------------------------------------------------------------------------------------------------------------
char ALevel_Data::Level_08[AsConfig::Level_Height][AsConfig::Level_Width] =
{
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
	0, 3, 7, 7, 7, 7, 7, 7, 3, 0, 1, 2,
	0, 3, 0, 0, 0, 0, 0, 0, 3, 0, 1, 2,
	0, 3, 0, 0, 0, 0, 0, 0, 3, 0, 1, 2,
	0, 3, 0, 0, 10, 10, 0, 0, 0, 0, 1, 2,
	0, 3, 0, 0, 10, 10, 0, 0, 0, 0, 1, 2,
	0, 3, 0, 0, 10, 10, 0, 0, 0, 0, 1, 2,
	0, 3, 0, 0, 0, 0, 0, 0, 3, 0, 1, 2,
	0, 3, 0, 0, 0, 0, 0, 0, 3, 0, 1, 2,
	0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 1, 2,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2
};
//------------------------------------------------------------------------------------------------------------
char ALevel_Data::Level_09[AsConfig::Level_Height][AsConfig::Level_Width] =
{
	1, 2, 1, 2, 1, 3, 0, 0, 0, 2, 2, 0,
	1, 2, 1, 2, 1, 3, 0, 0, 0, 2, 2, 9,
	0, 0, 0, 0, 0, 3, 0, 0, 0, 2, 2, 2,
	0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 9, 3, 0, 0, 0, 0, 0, 0,
	3, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3,
	0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 9, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1,
	5, 5, 5, 5, 5, 5, 5, 5, 3, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1
};
//------------------------------------------------------------------------------------------------------------
char ALevel_Data::Level_10[AsConfig::Level_Height][AsConfig::Level_Width] =
{
	0, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2,	0,
	0, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2,	0,
	0, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2,	0,
	0, 2, 2, 0, 2, 2, 0, 0, 0, 2, 2,	0,
	0, 2, 2, 0, 2, 2, 0, 0, 0, 2, 2,	0,
	0, 2, 2, 0, 2, 2, 0, 0, 0, 2, 2,	0,
	0, 2, 2, 0, 2, 2, 0, 8, 0, 2, 2,	0,
	0, 2, 2, 0, 2, 2, 0, 0, 0, 2, 2,	0,
	0, 2, 2, 0, 2, 2, 0, 0, 0, 2, 2, 0,
	0, 10, 10, 0, 1, 1, 0, 0, 0, 1, 1, 0,
	0, 10, 10, 0, 1, 1, 0, 0, 0, 1, 1, 0,
	0, 10, 10, 0, 1, 1, 1, 1, 1, 1, 1, 0,
	0, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2,	0,
	0, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 0
};
//------------------------------------------------------------------------------------------------------------