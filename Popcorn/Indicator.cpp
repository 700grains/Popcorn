#include "Indicator.h"

// AIndicator
//------------------------------------------------------------------------------------------------------------
AIndicator::AIndicator(EMessage_Type message_type, int x_pos, int y_pos)
	: Message_Type(message_type), Need_To_Send_A_Message(false), X_Pos(x_pos), Y_Pos(y_pos), Indicator_Lifetime()
{
	const int scale = AsConfig::Global_Scale;

	Indicator_Rect.left = X_Pos * scale;
	Indicator_Rect.top = Y_Pos * scale;
	Indicator_Rect.right = Indicator_Rect.left + Width * scale;
	Indicator_Rect.bottom = Indicator_Rect.top + Height * scale;
}
//------------------------------------------------------------------------------------------------------------
void AIndicator::Act()
{
	if (!Is_Finished())
		AsTools::Invalidate_Rect(Indicator_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AIndicator::Clear(HDC hdc, RECT& paint_area)
{
	//!!! TODO!
}
//------------------------------------------------------------------------------------------------------------
void AIndicator::Draw(HDC hdc, RECT& paint_area)
{
	const int scale = AsConfig::Global_Scale;

	double ratio;
	int current_height;
	RECT rect;

	AsTools::Rect(hdc, X_Pos, Y_Pos, Width, Height, AsConfig::Teleport_Portal_Color); // border

	if (Indicator_Lifetime == 0 || Is_Finished())
		return;

	ratio = (double)(Indicator_Lifetime - AsConfig::Current_Timer_Tick) / (double)Indicator_Timeout;

	current_height = (int)((double)(Inner_Height * scale) * ratio);

	if (current_height == 0)
		return;

	rect.left = (X_Pos + 1) * scale;
	rect.top = (Y_Pos + 1) * scale + (Inner_Height * scale - current_height);
	rect.right = rect.left + Inner_Width * scale;
	rect.bottom = (Y_Pos + 1 + Inner_Height) * scale;

	AsTools::Rect(hdc, rect, AsConfig::Red_Color); // inner part of the indicator
}
//------------------------------------------------------------------------------------------------------------
bool AIndicator::Is_Finished()
{
	if (AsConfig::Current_Timer_Tick > Indicator_Lifetime)
	{
		Cancellation();
		return true;
	}
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AIndicator::Cancellation()
{
	AMessage* message;

	if (Need_To_Send_A_Message)
	{
		message = new AMessage(Message_Type);

		AsMessage_Manager::Add_Message(message);

		Need_To_Send_A_Message = false;
	}

	Reset();
}
//------------------------------------------------------------------------------------------------------------
void AIndicator::Restart()
{
	Indicator_Lifetime = AsConfig::Current_Timer_Tick + Indicator_Timeout;

	Need_To_Send_A_Message = true;
}
//------------------------------------------------------------------------------------------------------------
void AIndicator::Reset()
{
	Indicator_Lifetime = 0;
	AsTools::Invalidate_Rect(Indicator_Rect);
}
//------------------------------------------------------------------------------------------------------------