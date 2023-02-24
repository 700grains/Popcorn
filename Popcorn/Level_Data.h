#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class ALevel_Data
{
public:
	ALevel_Data(int level_number);
	char* Level;

	static const int Max_Level_Number = 2;

private:
	static char Test_Level[AsConfig::Level_Height][AsConfig::Level_Width];

	static char Level_01[AsConfig::Level_Height][AsConfig::Level_Width];
	static char Level_02[AsConfig::Level_Height][AsConfig::Level_Width];
};
//------------------------------------------------------------------------------------------------------------