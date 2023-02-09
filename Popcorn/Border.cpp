#include "Border.h"

// AsBorder
//------------------------------------------------------------------------------------------------------------
AsBorder::~AsBorder()
{
	int i;

	for (i = 0; i < AsConfig::Gates_Count; i++)
		delete Gates[i]; // !!! is it correct?
}
//------------------------------------------------------------------------------------------------------------
AsBorder::AsBorder()
	: Floor_Rect{}, Gates {}
{
	Floor_Rect.left = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
	Floor_Rect.right = (AsConfig::Max_X_Pos - 1) * AsConfig::Global_Scale;
	Floor_Rect.top = AsConfig::Floor_Y_Pos * AsConfig::Global_Scale;
	Floor_Rect.bottom = AsConfig::Max_Y_Pos * AsConfig::Global_Scale;

	// Gates
	Gates[0] = new AGate(1, 29, 0, 3);
	Gates[1] = new AGate(AsConfig::Max_X_Pos, 29, AsConfig::Level_Width - 1, 3);

	Gates[2] = new AGate(1, 77, 0, 9);
	Gates[3] = new AGate(AsConfig::Max_X_Pos, 77, AsConfig::Level_Width - 1, 9);

	Gates[4] = new AGate(1, 129);
	Gates[5] = new AGate(AsConfig::Max_X_Pos, 129);

	Gates[6] = new AGate(1, 178);
	Gates[7] = new AGate(AsConfig::Max_X_Pos, 178);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Redraw_Floor()
{
	AsTools::Invalidate_Rect(Floor_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Open_Gate(int gate_index, bool is_partially)
{
	if (gate_index != AsConfig::Gates_Count - 1 && is_partially)
		AsConfig::Throw();

	if (gate_index >= 0 && gate_index < AsConfig::Gates_Count)
		Gates[gate_index]->Open_Gate(is_partially);
	else
		AsConfig::Throw();
}
//------------------------------------------------------------------------------------------------------------
int AsBorder::Long_Open_Gate()
{
	int i;
	int gate_index;
	bool is_found = false;
	AGate* gate;

	gate_index = AsTools::Rand(AsConfig::Gates_Count);
	
	for (i = 0; i < AsConfig::Gates_Count; i++)
	{
		gate = Gates[gate_index];

		if (gate->Is_Closed())
		{
			if (gate->Level_X_Pos == -1)
			{
				is_found = true;
				break;
			}

			if (! AsLevel::Has_Brick_At(gate->Level_X_Pos, gate->Level_Y_Pos) && ! AsLevel::Has_Brick_At(gate->Level_X_Pos, gate->Level_Y_Pos + 1))
			{
				is_found = true;
				break;
			}
		}

		++gate_index;

		if (gate_index >= AsConfig::Gates_Count)
			gate_index = 0;
	}

	if (! is_found)
		AsConfig::Throw();

	Open_Gate(gate_index, false);

	return gate_index;
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Is_Gate_Opened(int gate_index)
{
	if (gate_index >= 0 && gate_index < AsConfig::Gates_Count)
		return Gates[gate_index]->Is_Opened();
	else
	{
		AsConfig::Throw();
		return false;
	}
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Is_Gate_Closed(int gate_index)
{
	if (gate_index >= 0 && gate_index < AsConfig::Gates_Count)
		return Gates[gate_index]->Is_Closed();
	else
	{
		AsConfig::Throw();
		return false;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Get_Gate_Pos(int gate_index, int& gate_x, int& gate_y)
{
	if (gate_index >= 0 && gate_index < AsConfig::Gates_Count)
		Gates[gate_index]->Get_Pos(gate_x, gate_y);
	else
		AsConfig::Throw();
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{// Correcting the position when reflected from the frame

	bool got_hit = false;

	//1. Left corner
	if (next_x_pos - ball->Radius < AsConfig::Border_X_Offset)
	{
		got_hit = true;
		ball->Reflect(false);
	}

	//2. Top corner
	if (next_y_pos - ball->Radius < AsConfig::Border_Y_Offset)
	{
		got_hit = true;
		ball->Reflect (true);
	}

	//3. Right corner
	if (next_x_pos + ball->Radius > AsConfig::Max_X_Pos)
	{
		got_hit = true;
		ball->Reflect(false);
	}

	//4. Bottom corner
	if (AsConfig::Level_Has_Floor && next_y_pos + ball->Radius > AsConfig::Floor_Y_Pos)
	{
		got_hit = true;
		ball->Reflect(true);
	}
	// We check the position below the visible border.

	if (next_y_pos + ball->Radius > (double)AsConfig::Max_Y_Pos + ball->Radius * 4.0) 
			ball->Set_State (EBall_State::Lost);

	return got_hit;
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Begin_Movement()
{
	// not yet used.
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Finish_Movement()
{
	// not yet used.
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Advance(double max_speed)
{
	// not yet used.
}
//------------------------------------------------------------------------------------------------------------
double AsBorder::Get_Speed()
{
	// not yet used.
	return 0.0;
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Act()
{
	int i;

	for (i = 0; i < AsConfig::Gates_Count; ++i)
		Gates[i]->Act();
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Clear(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;
	int i;

	// 1. Clearing the gate
	for (i = 0; i < AsConfig::Gates_Count; ++i)
		Gates[i]->Clear(hdc, paint_area);

	// 2. Clearing the floor
	if (! AsConfig::Level_Has_Floor)
		return;
	
	if (! IntersectRect(&intersection_rect, &paint_area, &Floor_Rect))
			return;

	AsTools::Rect(hdc, Floor_Rect, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw(HDC hdc, RECT& paint_area)
{// Draws a level border.

	int i;

	// 1. Left line
	for (i = 0; i < 50; i++)
		Draw_Element(hdc, paint_area, 2, 1 + i * 4, false);

	// 2. Right line
	for (i = 0; i < 50; i++)
		Draw_Element(hdc, paint_area, AsConfig::Max_X_Pos + 1, 1 + i * 4, false);

	// 3. Top line
	for (i = 0; i < 50; i++)
		Draw_Element(hdc, paint_area, 3 + i * 4, 0, true);

	// 4. if(Floor)
	if (AsConfig::Level_Has_Floor)
		Draw_Floor(hdc, paint_area);

	// 5. Gates
	for (i = 0; i < AsConfig::Gates_Count; ++i)
		Gates[i]->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Is_Finished()
{
	return false; // Not used
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, RECT& paint_area, int x, int y, bool top_border)
{// Draws a level border element
	int i;
	int gate_top_y, gate_bot_y;
	RECT intersection_rect, rect;

	rect.left = x * AsConfig::Global_Scale;
	rect.top = y * AsConfig::Global_Scale;
	rect.right = (x + 4) * AsConfig::Global_Scale;
	rect.bottom = (y + 4) * AsConfig::Global_Scale;

	if (! top_border)
	{
		for (i = 0; i < AsConfig::Gates_Count; ++i)
		{
			Gates[i]->Get_Y_Size(gate_top_y, gate_bot_y);

			if (rect.top >= gate_top_y && rect.bottom <= gate_bot_y)
				return; // The gate overlaps the border element, there is no point in drawing it.
		}
	}

	if (! IntersectRect(&intersection_rect, &paint_area, &rect))
		return;

 // The main line
	if (top_border)
		AsTools::Rect(hdc, x, y + 1, 4, 3, AsConfig::Blue_Color);
	else
		AsTools::Rect(hdc, x + 1, y, 3, 4, AsConfig::Blue_Color);

	// White border
	if (top_border)
		AsTools::Rect(hdc, x , y, 4, 1, AsConfig::White_Color);
	else
		AsTools::Rect(hdc, x, y, 1, 4, AsConfig::White_Color);

	// Perforation
	if (top_border)
		AsTools::Rect(hdc, x + 2, y + 2, 1, 1, AsConfig::BG_Color);
	else
		AsTools::Rect(hdc, x + 2, y + 1, 1, 1, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Floor(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;
	int i, strokes_count;
	int x_pos, y_pos;
	int line_len = 4 * AsConfig::Global_Scale;
	int gap_len = 2 * AsConfig::Global_Scale;
	int stroke_len = line_len + gap_len;

	if (!IntersectRect(&intersection_rect, &paint_area, &Floor_Rect) )
		return;
	
	strokes_count = (Floor_Rect.right - Floor_Rect.left + AsConfig::Global_Scale) / stroke_len;
	x_pos = Floor_Rect.left + 1;
	y_pos = Floor_Rect.top + (Floor_Rect.bottom - Floor_Rect.top) / 2;
	AsConfig::Letter_Color.Select(hdc);

	for (i = 0; i < strokes_count; i++)
	{
		MoveToEx(hdc, x_pos, y_pos, 0);
		LineTo(hdc, x_pos + line_len, y_pos);

		x_pos += stroke_len;
	}	
}
//------------------------------------------------------------------------------------------------------------
