#include "Platform_State.h"

// AsPlatform_State
//------------------------------------------------------------------------------------------------------------
AsPlatform_State::AsPlatform_State()
	: Current_State(EPlatform_State::Regular), Next_State(EPlatform_State::Unknown), Regular(EPlatform_Substate_Regular::Missing), Meltdown(EPlatform_Substate_Meltdown::Unknown),
	Rolling(EPlatform_Substate_Rolling::Unknown), Glue(EPlatform_Transformation::Unknown), Expanding(EPlatform_Transformation::Unknown), Laser(EPlatform_Transformation::Unknown),
	Moving(EPlatform_Moving_State::Stop)
{

}
//------------------------------------------------------------------------------------------------------------
AsPlatform_State::operator EPlatform_State() const
{
	return Current_State;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_State::operator = (EPlatform_State new_state)
{
	Current_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_State::Set_Next_State(EPlatform_State next_state)
{
	if (next_state == Current_State)
		return;

	switch (Current_State)
	{
	case EPlatform_State::Regular:
		AsConfig::Throw(); // Switching to another state from the Regular state must be explicit.
		break;

	case EPlatform_State::Meltdown:
		return; // We ignore the next state after Meltdown. the game process must be restarted

	case EPlatform_State::Rolling:
		AsConfig::Throw(); // Rolling should go to the next state by itself
		break;

	case EPlatform_State::Glue:
		Glue = EPlatform_Transformation::Finalize;
		break;

	case EPlatform_State::Expanding:
		Expanding = EPlatform_Transformation::Finalize;
		break;

	case EPlatform_State::Laser:
		Laser = EPlatform_Transformation::Finalize;
		break;

	default:
		AsConfig::Throw();
		break;
	}

	Next_State = next_state;
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform_State::Get_Next_State()
{
	return Next_State;
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform_State::Set_State(EPlatform_Substate_Regular new_regular_state)
{
	EPlatform_Transformation* transformation_state = nullptr;

	if (Current_State == EPlatform_State::Regular && Regular == new_regular_state)
		return EPlatform_State::Unknown;

	if (new_regular_state == EPlatform_Substate_Regular::Normal)
	{
		switch (Current_State)
		{
		case EPlatform_State::Glue:
			transformation_state = &Glue;
			break;

		case EPlatform_State::Expanding:
			transformation_state = &Expanding;
			break;

		case EPlatform_State::Laser:
			transformation_state = &Laser;
			break;
		}

		if (transformation_state != nullptr)
		{
			if (*transformation_state == EPlatform_Transformation::Unknown)
				return Set_Next_Or_Regular_State(new_regular_state); // State finalization finished
			else
				*transformation_state = EPlatform_Transformation::Finalize; // We start the finalization of the state

			return EPlatform_State::Unknown;
		}
	}

	Current_State = EPlatform_State::Regular;
	Regular = new_regular_state;

	return EPlatform_State::Unknown;
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform_State::Set_Next_Or_Regular_State(EPlatform_Substate_Regular new_regular_state)
{// if returned state != Unknown, we have to set this new state
	EPlatform_State next_state;

	Current_State = EPlatform_State::Regular;

	// If there is a deferred state, then we translate into it, and not into the "Regular"
	next_state = Get_Next_State();

	if (next_state != EPlatform_State::Unknown)
		Regular = new_regular_state;

	return next_state;
}
//------------------------------------------------------------------------------------------------------------