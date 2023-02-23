#include "Common.h"

// AMover
//------------------------------------------------------------------------------------------------------------
AMover::~AMover()
{
}
//------------------------------------------------------------------------------------------------------------




// AGraphics_Object
//------------------------------------------------------------------------------------------------------------
AGraphics_Object::~AGraphics_Object()
{
}
//------------------------------------------------------------------------------------------------------------




// AGame_Objects_Set
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Begin_Movement()
{
	int index = 0;
	AGame_Object* object;

	while (Get_Next_Game_Object(index, &object) )
		object->Begin_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Finish_Movement()
{
	int index = 0;
	AGame_Object* object;

	while (Get_Next_Game_Object(index, &object))
		object->Finish_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Advance(double max_speed)
{
	int index = 0;
	AGame_Object* object;

	while (Get_Next_Game_Object(index, &object))
		object->Advance(max_speed);
}
//------------------------------------------------------------------------------------------------------------
double AGame_Objects_Set::Get_Speed()
{
	int index = 0;
	AGame_Object* object;
	double max_speed = 0.0, current_speed;

	while (Get_Next_Game_Object(index, &object))
	{
		current_speed = object->Get_Speed();

		if (current_speed > max_speed)
			max_speed = current_speed;
	}

	return max_speed;
}
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Act()
{
	int index = 0;
	AGame_Object* object;

	while (Get_Next_Game_Object(index, &object))
		object->Act();
}
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Clear(HDC hdc, RECT& paint_area)
{
	int index = 0;
	AGame_Object* object;

	while (Get_Next_Game_Object(index, &object))
		object->Clear(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AGame_Objects_Set::Draw(HDC hdc, RECT& paint_area)
{
	int index = 0;
	AGame_Object* object;

	while (Get_Next_Game_Object(index, &object))
		object->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
bool AGame_Objects_Set::Is_Finished()
{
	return false; 	// Not used
}
//------------------------------------------------------------------------------------------------------------




// AString
//------------------------------------------------------------------------------------------------------------
AString::~AString()
{
}
//------------------------------------------------------------------------------------------------------------
AString::AString()
{
}
//------------------------------------------------------------------------------------------------------------
AString::AString(const wchar_t* str)
	: Content(str)
{
}
//------------------------------------------------------------------------------------------------------------
void AString::Append(int value)
{
	wchar_t buffer[32];

	swprintf(buffer, 32, L"%.6i", value);

	Content += buffer;
}
//------------------------------------------------------------------------------------------------------------
const wchar_t* AString::Get_Content()
{
	return Content.c_str();
}
//------------------------------------------------------------------------------------------------------------
int AString::Get_Length()
{
	return Content.length();
}
//------------------------------------------------------------------------------------------------------------




// AMessage
//------------------------------------------------------------------------------------------------------------
AMessage::AMessage(EMessage_Type message_type)
	: Message_Type(message_type)
{
}
//------------------------------------------------------------------------------------------------------------