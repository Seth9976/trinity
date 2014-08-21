#include "StdAfx.h"
#include "EveAnimationSequencer.h"
#include "EveAnimationState.h"
#include "Eve/SpaceObject/EveSpaceObject2.h"

BLUE_DEFINE( EveAnimationStateSequencer );

const Be::VarChooser EveAnimationProgressChooser[] =
{
	// Name						Value								Docstring
	{ "EVE_ANIM_DONE",			BeCast( EVE_ANIM_DONE ),			"Animation state done" }, 
	{ "EVE_ANIM_INACTIVE",		BeCast( EVE_ANIM_INACTIVE ),		"Animation state inactive(no actions taken yet)" }, 
	{ "EVE_ANIM_FINALIZING",	BeCast( EVE_ANIM_FINALIZING ),		"Animation is finishing and waiting to be replaced" }, 
	{ "EVE_ANIM_RUNNING",		BeCast( EVE_ANIM_RUNNING ),			"Animation state active, i.e. running it's main animations" }, 
	{0}
};

const Be::ClassInfo* EveAnimationStateSequencer::ExposeToBlue()
{
    EXPOSURE_BEGIN( EveAnimationStateSequencer, "" )
        MAP_INTERFACE( EveAnimationStateSequencer )
        MAP_INTERFACE( IRoot )
		
		MAP_ATTRIBUTE( "owner", m_owner, "", Be::READ )
		MAP_ATTRIBUTE( "update", m_update, "", Be::READWRITE )
		MAP_ATTRIBUTE( "currentState", m_currentState, "", Be::READ )
		MAP_ATTRIBUTE( "pendingStates", m_pendingStates, "", Be::READ )
		MAP_ATTRIBUTE( "states", m_states, "", Be::READWRITE | Be::PERSIST )

		MAP_METHOD_AND_WRAP( "Clear", Clear, "" )
		MAP_METHOD_AND_WRAP( "GoToState", GoToState, "" )

		MAP_ATTRIBUTE( "defaultAnimation", m_defaultAnimation, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "extraAnimation", m_extraAnimation, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "extraAnimationTrackMask", m_extraAnimationTrackMask, "", Be::READWRITE | Be::PERSIST )

    EXPOSURE_END()
}