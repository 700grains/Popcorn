#include "Border.h"

// AGate
const double AGate::Max_Hole_Short_Height = 9.0;
const double AGate::Hole_Height_Short_Step = Max_Hole_Short_Height / ( (double)AsConfig::FPS / 2.0 ); // Animation will complete in 1/2 second
const double AGate::Max_Hole_Long_Height = 18.0;
const double AGate::Hole_Height_Long_Step = Max_Hole_Long_Height / ((double)AsConfig::FPS * 3.0); // Animation will complete in 3 second;
//------------------------------------------------------------------------------------------------------------
AGate::AGate(int x_pos, int y_pos)
	: Gate_State(EGate_State::Closed), Gate_Transformation(EGate_Transformation::Unknown), X_Pos(x_pos), Y_Pos(y_pos), Original_Y_Pos(y_pos), Edges_Count(5), Gate_Close_Timer(0),
	Hole_Height(0)
{
	const int scale = AsConfig::Global_Scale;

	Gate_Rect.left = X_Pos * scale;
	Gate_Rect.top = (int)Y_Pos * scale;
	Gate_Rect.right = Gate_Rect.left + Width * scale;
	Gate_Rect.bottom = Gate_Rect.top + Height * scale; 
}
//------------------------------------------------------------------------------------------------------------
void AGate::Act()
{
	bool correct_pos;

	switch (Gate_State)
	{
	case EGate_State::Closed:
		break;

	case EGate_State::Partially_Open:
		if (Act_For_Partially_Open())
			Redraw_Gate();
		break;

	case EGate_State::Fully_Open:
		if (Act_For_Fully_Open(correct_pos))
		{
			if (correct_pos)
			{
				Y_Pos = Original_Y_Pos - Hole_Height / 2;

				Gate_Rect.top = (int)round(Y_Pos * AsConfig::D_Global_Scale);
				Gate_Rect.bottom = (int)round((Y_Pos + (double)Height + Hole_Height) * AsConfig::D_Global_Scale);
			}

			Redraw_Gate();
		}
		break;

	default:
		AsConfig::Throw();
		break;
	}
}
//------------------------------------------------------------------------------------------------------------
void AGate::Clear(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Gate_Rect))
		return;

	AsConfig::Rect(hdc, Gate_Rect, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Gate_Rect))
		return;

	Clear(hdc, paint_area);

	Draw_Cup(hdc, true);
	Draw_Cup(hdc, false);
}
//------------------------------------------------------------------------------------------------------------
bool AGate::Is_Finished()
{
	return false; //!!! Have to do
}
//------------------------------------------------------------------------------------------------------------
void AGate::Open_Gate(bool is_partially)
{
	if (Gate_State != EGate_State::Closed)
		return;
	
	if (is_partially)
		Gate_State = EGate_State::Partially_Open;
	else
		Gate_State = EGate_State::Fully_Open;

	Gate_Transformation = EGate_Transformation::Init;
}
//------------------------------------------------------------------------------------------------------------
bool AGate::Is_Opened()
{
	if (Gate_State != EGate_State::Closed)
		if (Gate_Transformation == EGate_Transformation::Active)
			return true;

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AGate::Get_Y_Size(int& gate_top_y, int& gate_bot_y)
{
	gate_top_y = Gate_Rect.top;
	gate_bot_y = Gate_Rect.bottom;
}
//------------------------------------------------------------------------------------------------------------
bool AGate::Act_For_Partially_Open()
{
	switch (Gate_Transformation)
	{
	case EGate_Transformation::Init:
		if (Hole_Height < Max_Hole_Short_Height)
		{
			Hole_Height += Hole_Height_Short_Step;
			//x_pos -= Expanding_Platform_Width_Step / 2.0;
			//correct_pos = true;
		}
		else
		{
			Gate_Close_Timer = AsConfig::Current_Timer_Tick + Short_Opening_Timeout;
			Gate_Transformation = EGate_Transformation::Active;
		}
		return true;

	case EGate_Transformation::Active:
		if (AsConfig::Current_Timer_Tick >= Gate_Close_Timer)
			Gate_Transformation = EGate_Transformation::Finalize;
		break;

	case EGate_Transformation::Finalize:
		if (Hole_Height > 0.0)
		{
			Hole_Height -= Hole_Height_Short_Step;
			//x_pos += Expanding_Platform_Width_Step / 2.0;
			//correct_pos = true;
		}
		else
		{
			Gate_Transformation = EGate_Transformation::Unknown;
			//next_state = Gate_State->Set_State(EPlatform_Substate_Regular::Normal);
			Gate_State = EGate_State::Closed;
		}
		return true;

	default:
		AsConfig::Throw();
	}
	return false;
}
//------------------------------------------------------------------------------------------------------------
bool AGate::Act_For_Fully_Open(bool& correct_pos)
{
	correct_pos = false;

	switch (Gate_Transformation)
	{
	case EGate_Transformation::Init:
		if (Hole_Height < Max_Hole_Long_Height)
		{
			Hole_Height += Hole_Height_Long_Step;
			correct_pos = true;
		}
		else
		{
			Gate_Close_Timer = AsConfig::Current_Timer_Tick + Short_Opening_Timeout;
			Gate_Transformation = EGate_Transformation::Active;
		}
		return true;

	case EGate_Transformation::Active: 
		if (AsConfig::Current_Timer_Tick >= Gate_Close_Timer)
			Gate_Transformation = EGate_Transformation::Finalize;
		break;

	case EGate_Transformation::Finalize:
		if (Hole_Height > 0.0)
		{
			Hole_Height -= Hole_Height_Long_Step;
			Gate_Rect.top = (int)((Y_Pos - Hole_Height / 2) * AsConfig::D_Global_Scale);
			Gate_Rect.bottom = (int)((Y_Pos + (double)Height + Hole_Height / 2) * AsConfig::D_Global_Scale);
			correct_pos = true;
		}
		else
		{
			Gate_Transformation = EGate_Transformation::Unknown;
			//next_state = Gate_State->Set_State(EPlatform_Substate_Regular::Normal);
			Gate_State = EGate_State::Closed;
		}
		return true;

	default:
		AsConfig::Throw();
	}
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw_Cup(HDC hdc, bool is_top)
{
	RECT rect;
	HRGN region;
	XFORM xform, old_xform;
	double cup_y_offset = 5.0;

	const int x = 0, y = 0;
	const int scale = AsConfig::Global_Scale;
	const int d_scale = AsConfig::D_Global_Scale;
	const int half_scale = scale / 2; // NB! 3 / 2 = 1!

	xform.eM11 = 1.0f;
	xform.eM12 = 0.0f;
	xform.eM21 = 0.0f;

	if (is_top)
	{
		xform.eM22 = 1.0f;
		cup_y_offset = 0.0;
	}
	else
	{
		xform.eM22 = -1.0f;
		if (Gate_State == EGate_State::Fully_Open)
			cup_y_offset = ( (double)Height + Hole_Height) * d_scale - 1.0;
		else
			cup_y_offset = (double)(Height * d_scale - 1.0);

	}

	xform.eDx = (float)(X_Pos * scale);
	xform.eDy = (float)round(Y_Pos * d_scale + cup_y_offset);

	GetWorldTransform(hdc, &old_xform);
	SetWorldTransform(hdc, &xform);


	// 1. semi-circular part of the bowl
	rect.left = x * scale;
	rect.top = (y + 1) * scale;
	rect.right = rect.left + 6 * scale;
	rect.bottom = rect.top + 4 * scale;

	// 1.1 Base
	AsConfig::Blue_Color.Select(hdc);
	AsConfig::Round_Rect(hdc, rect, 3);

	// 1.2 Highlight on the left side 
	rect.left = X_Pos * scale;
	rect.right = rect.left + 3 * scale;

	if (is_top)
	{
		rect.top = (int)round( (Y_Pos + 1.0) * d_scale);
		rect.bottom = rect.top + 4 * scale;
	}
	else
	{
		rect.top = (int)round((Y_Pos - 1.0) * d_scale + cup_y_offset + 1.0);
		rect.bottom = rect.top - 4 * d_scale;
	}

	region = CreateRectRgnIndirect(&rect);
	SelectClipRgn(hdc, region);

	AsConfig::Gate_Color.Select_Pen(hdc);

	rect.left = x * scale + half_scale;
	rect.top = (y + 1) * scale + half_scale;
	rect.right = rect.left + 5 * scale + half_scale;
	rect.bottom = rect.top + 5 * scale + half_scale;

	AsConfig::Round_Rect(hdc, rect, 3);

	SelectClipRgn(hdc, 0);
	DeleteObject(region);

	// 1.3 Highlight on the bottom part of the bowl
	AsConfig::Rect(hdc, x, y + 4, 4, 1, AsConfig::White_Color);

	// 1.4 A "patch" in the lower right corner
	AsConfig::Rect(hdc, x + 4, y + 3, 2, 2, AsConfig::Blue_Color);

	// 1.5 Cutting a hole in the bowl
	AsConfig::Rect(hdc, x + 4, y + 3, 1, 1, AsConfig::BG_Color);

	// 1.5 Draw a jumper in front of the bowl
	AsConfig::Rect(hdc, x + 2, y, 2, 1, AsConfig::Blue_Color);

	// 1.6 Draw edges 
	Draw_Edges(hdc);

	SetWorldTransform(hdc, &old_xform);
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw_Edges(HDC hdc)
{
	int i;
	int count;
	double ratio = 1.0 - Hole_Height / Max_Hole_Short_Height;
	bool is_long_edge = false;

	count = (int)((double)Edges_Count * ratio);

	for (i = 0; i < count; i++)
	{
		Draw_Single_Edge(hdc, 5 + i, is_long_edge);
		is_long_edge = !is_long_edge;
	}
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw_Single_Edge(HDC hdc, int edge_y_offset, bool is_long)
{
	if (is_long)
	{
		AsConfig::Rect(hdc, 0, edge_y_offset, 4, 1, AsConfig::White_Color);
		AsConfig::Rect(hdc, 4, edge_y_offset, 2, 1, AsConfig::Blue_Color);
	}
	else
	{
		AsConfig::Rect(hdc, 1, edge_y_offset, 2, 1, AsConfig::Blue_Color);
		AsConfig::Rect(hdc, 4, edge_y_offset, 1, 1, AsConfig::Blue_Color);
	}
}
//------------------------------------------------------------------------------------------------------------
void AGate::Redraw_Gate()
{
	--Gate_Rect.top;
	++Gate_Rect.bottom;

	AsConfig::Invalidate_Rect(Gate_Rect);

	++Gate_Rect.top;
	--Gate_Rect.bottom;
}
//------------------------------------------------------------------------------------------------------------




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
	Gates[0] = new AGate(1, 29);
	Gates[1] = new AGate(AsConfig::Max_X_Pos, 29);

	Gates[2] = new AGate(1, 77);
	Gates[3] = new AGate(AsConfig::Max_X_Pos, 77);

	Gates[4] = new AGate(1, 129);
	Gates[5] = new AGate(AsConfig::Max_X_Pos, 129);

	Gates[6] = new AGate(1, 178);
	Gates[7] = new AGate(AsConfig::Max_X_Pos, 178);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Redraw_Floor()
{
	AsConfig::Invalidate_Rect(Floor_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Open_Gate(int gate_index, bool is_partially)
{
	if (gate_index < 0 || gate_index >= AsConfig::Gates_Count)
		AsConfig::Throw();

	if (gate_index != AsConfig::Gates_Count - 1 && is_partially)
		AsConfig::Throw();

	Gates[gate_index]->Open_Gate(is_partially);
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Is_Gate_Opened(int gate_index)
{
	if (gate_index < 0 || gate_index >= AsConfig::Gates_Count)
		AsConfig::Throw();

	return Gates[gate_index]->Is_Opened();
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
			ball->Set_State (EBS_Lost);

	return got_hit;
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

	AsConfig::BG_Color.Select(hdc);

	Rectangle(hdc, Floor_Rect.left, Floor_Rect.top, Floor_Rect.right - 1, Floor_Rect.bottom - 1);
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
	AsConfig::Blue_Color.Select(hdc);

	if (top_border)
		Rectangle(hdc, x * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale - 1, (y + 4) * AsConfig::Global_Scale - 1);
	else
		Rectangle(hdc, (x + 1) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale - 1, (y + 4) * AsConfig::Global_Scale - 1);

	// White border
	AsConfig::White_Color.Select(hdc);

	if (top_border)
		Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale - 1, (y + 1) * AsConfig::Global_Scale - 1);
	else
		Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 1) * AsConfig::Global_Scale - 1, (y + 4) * AsConfig::Global_Scale - 1);

	// Perforation
	AsConfig::BG_Color.Select(hdc);


	if (top_border)
		Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale - 1, (y + 3) * AsConfig::Global_Scale - 1);
	else
		Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale - 1, (y + 2) * AsConfig::Global_Scale - 1);
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