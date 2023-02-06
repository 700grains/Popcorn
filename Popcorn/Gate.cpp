# include "Gate.h"

// AGate
const double AGate::Max_Hole_Short_Height = 9.0;
const double AGate::Hole_Height_Short_Step = Max_Hole_Short_Height / ((double)AsConfig::FPS / 2.0); // Animation will complete in 1/2 second
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
		if (Act_For_Open(true, correct_pos))
			Redraw_Gate();
		break;

	case EGate_State::Fully_Open:
		if (Act_For_Open(false, correct_pos))
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

	if (Gate_State == EGate_State::Fully_Open && (Gate_Transformation == EGate_Transformation::Init || Gate_Transformation == EGate_Transformation::Finalize))
		Draw_Electrical_Charge(hdc);
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
bool AGate::Act_For_Open(bool Partially_Open, bool& correct_pos)
{
	double max_hole_height, hole_height_step;
	int opening_timeout;

	correct_pos = false;

	if (Partially_Open)
	{
		opening_timeout = Short_Opening_Timeout;
		max_hole_height = Max_Hole_Short_Height;
		hole_height_step = Hole_Height_Short_Step;
	}
	else
	{
		opening_timeout = Long_Opening_Timeout;
		hole_height_step = Hole_Height_Long_Step;
		max_hole_height = Max_Hole_Long_Height;
	}

	switch (Gate_Transformation)
	{
	case EGate_Transformation::Init:
		if (Hole_Height < max_hole_height)
		{
			Hole_Height += hole_height_step;
			correct_pos = true;
		}
		else
		{
			Gate_Close_Timer = AsConfig::Current_Timer_Tick + opening_timeout;
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
			Hole_Height -= hole_height_step;
			correct_pos = true;
		}
		else
		{
			Gate_Transformation = EGate_Transformation::Unknown;
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
	const double d_scale = AsConfig::D_Global_Scale;
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
			cup_y_offset = ((double)Height + Hole_Height) * d_scale - 1.0;
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
		rect.top = (int)round((Y_Pos + 1.0) * d_scale);
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
	if (Gate_State == EGate_State::Fully_Open)
		Draw_Long_Opening_Edges(hdc);
	else
		Draw_Short_Opening_Edges(hdc);
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw_Long_Opening_Edges(HDC hdc)
{
	int i;
	double ratio = Hole_Height / Max_Hole_Long_Height;
	bool is_long_edge = false;

	if (ratio < 0.3)
	{
		for (i = 0; i < 4; i++)
		{
			Draw_Single_Edge(hdc, 5 + i, is_long_edge);
			is_long_edge = !is_long_edge;
		}
		if (ratio > 0.1)
			Draw_Red_Edge(hdc, 9, false, false);
	}
	else if (ratio < 0.5)
	{
		Draw_Single_Edge(hdc, 5, false);
		Draw_Single_Edge(hdc, 6, true);
		Draw_Single_Edge(hdc, 7, true);

		Draw_Red_Edge(hdc, 8, true, true);
		Draw_Red_Edge(hdc, 9, false, false);
	}
	else
	{
		for (i = 0; i < 2; i++)
			Draw_Single_Edge(hdc, 5 + i, true);

		Draw_Red_Edge(hdc, 7, true, false);
		Draw_Red_Edge(hdc, 8, true, true);
		Draw_Red_Edge(hdc, 9, false, false);
	}
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw_Short_Opening_Edges(HDC hdc)
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
void AGate::Draw_Red_Edge(HDC hdc, int edge_y_offset, bool is_long, bool highlight)
{
	if (is_long)
	{
		AsConfig::Rect(hdc, 0, edge_y_offset, 6, 1, AsConfig::Red_Color);
		if (highlight)
		{
			AsConfig::Rect(hdc, 1, edge_y_offset, 1, 1, AsConfig::White_Color);
		}
	}
	else
	{
		AsConfig::Rect(hdc, 1, edge_y_offset, 4, 1, AsConfig::Red_Color);
	}
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw_Electrical_Charge(HDC hdc)
{
	int i;
	int dots_count = 4;
	int electrical_field;
	int dot_x, dot_y;
	double ratio = Hole_Height / Max_Hole_Long_Height;

	if (ratio < 0.2 || ratio > 0.9)
		return; // do not draw electric discharges at the beginning and at the end of the gate opening animation

	electrical_field = (int)(Original_Y_Pos + (double)Height / 2.0 - Hole_Height / 2.0) + 1;

	for (i = 0; i < dots_count; i++)
	{
		dot_x = 1 + AsConfig::Rand(4);
		dot_y = AsConfig::Rand((int)Hole_Height - 1);

		AsConfig::Rect(hdc, X_Pos + dot_x, electrical_field + dot_y, 1, 1, AsConfig::White_Color);
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
