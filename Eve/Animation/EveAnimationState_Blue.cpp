#include "StdAfx.h"
#include "EveAnimationState.h"
#include "SpaceObject/EveSpaceObject2.h"

const Be::VarChooser EveAnimationProgressChooser[] =
{
	// Name						Value								Docstring
	{ "EVEANIM_DONE",			BeCast( EVE_ANIM_DONE ),			"Animation state done" }, 
	{ "EVEANIM_ENTERING",		BeCast( EVE_ANIM_ENTERING ),		"Entering animation state" }, 
	{ "EVEANIM_EXITING",		BeCast( EVE_ANIM_EXITING ),			"Exiting from animation state" }, 
	{ "EVEANIM_INACTIVE",		BeCast( EVE_ANIM_INACTIVE ),		"Animation state inactive(no actions taken yet)" }, 
	{ "EVEANIM_RUNNING",		BeCast( EVE_ANIM_RUNNING ),			"Animation state active, i.e. running it's main animations" }, 
	{ "EVEANIM_TRANSITIONING",	BeCast( EVE_ANIM_TRANSITIONING ),	"Transitioning into a new state" }, 
	{0}
};

BLUE_REGISTER_ENUM( "EVE_ANIMATION_STATE_PROGRESS", EveAnimationStateProgress, EveAnimationProgressChooser );


const Be::VarChooser EveAnimationStageChooser[] =
{
	// Name							Value									Docstring
	{ "EVEANISTAGE_ENTER",			BeCast( EVE_ANISTAGE_ENTER ),			"" }, 
	{ "EVEANISTAGE_EXIT",			BeCast( EVE_ANISTAGE_EXIT ),			"" }, 
	{ "EVEANISTAGE_MAIN",			BeCast( EVE_ANISTAGE_MAIN ),			"" }, 
	{ "EVEANISTAGE_TRANSITION",		BeCast( EVE_ANISTAGE_TRANSITION ),		"" },
	{0}
};

BLUE_REGISTER_ENUM( "EVE_ANIMATION_STAGE", EveAnimationStage, EveAnimationStageChooser );

BLUE_DEFINE( EveAnimationState );

const Be::ClassInfo* EveAnimationState::ExposeToBlue()
{
    EXPOSURE_BEGIN( EveAnimationState, "" )
        MAP_INTERFACE( EveAnimationState )
        MAP_INTERFACE( IRoot )

		MAP_ATTRIBUTE( "name", m_name, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE_WITH_CHOOSER( "progress", m_progress, "", Be::READ, EveAnimationProgressChooser )
		MAP_ATTRIBUTE( "secondsLeft", m_secondsRemaining, "", Be::READ )

		MAP_ATTRIBUTE( "enter", m_enterSequence, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "main", m_mainSequence, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "exit", m_exitSequence, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "transitions", m_transitions, "", Be::READWRITE | Be::PERSIST )
    EXPOSURE_END()
}

BLUE_DEFINE( EveAnimationStateContainer );
const Be::ClassInfo* EveAnimationStateContainer::ExposeToBlue()
{
    EXPOSURE_BEGIN( EveAnimationStateContainer, "" )
        MAP_INTERFACE( EveAnimationStateContainer )

		MAP_ATTRIBUTE( "states", m_states, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "defaultAnimation", m_defaultAnimation, "", Be::READWRITE | Be::PERSIST )
    EXPOSURE_END()
}
