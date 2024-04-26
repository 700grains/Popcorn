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




// APoint
//------------------------------------------------------------------------------------------------------------
APoint::APoint()
	: X(0), Y(0)
{
}
//------------------------------------------------------------------------------------------------------------
APoint::APoint(int x, int y)
	: X(x), Y(y)
{
}
//------------------------------------------------------------------------------------------------------------
bool APoint::operator == (const APoint& another)
{
	if (X == another.X && Y == another.Y)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void APoint::Set_As(int x, int y)
{
	X = x;
	Y = y;
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
void AString::Append(wchar_t symbol)
{
	Content.append(1, symbol);
}
//------------------------------------------------------------------------------------------------------------
void AString::Append(int value, int digits)
{
	wchar_t format[32];
	wchar_t buffer[32];

	swprintf(format, 32, L"%%.%ii", digits);

	swprintf(buffer, 32, format, value);

	Content += buffer;
}
//------------------------------------------------------------------------------------------------------------
void AString::Delete_Last_Symbol()
{
	if (Content.size() > 0)
		Content.pop_back();
}
//------------------------------------------------------------------------------------------------------------
const wchar_t* AString::Get_Content()
{
	return Content.c_str();
}
//------------------------------------------------------------------------------------------------------------
int AString::Get_Length()
{
	return (int)Content.length();
}
//------------------------------------------------------------------------------------------------------------
void AString::Clear()
{
	Content = L"";
}
//------------------------------------------------------------------------------------------------------------




// AMessage
//------------------------------------------------------------------------------------------------------------
AMessage::AMessage(EMessage_Type message_type)
	: Message_Type(message_type)
{
}
//------------------------------------------------------------------------------------------------------------




// AsMessage_Manager
std::queue <AMessage*> AsMessage_Manager::Messages_Queue;
//------------------------------------------------------------------------------------------------------------
void AsMessage_Manager::Add_Message(AMessage* message)
{
	Messages_Queue.push(message);
}
//------------------------------------------------------------------------------------------------------------
bool AsMessage_Manager::Get_Message(AMessage** message)
{
	if (Messages_Queue.size() == 0)
		return false;

	*message = Messages_Queue.front();

	Messages_Queue.pop();

	return true;
}
//------------------------------------------------------------------------------------------------------------