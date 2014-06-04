#include "StdAfx.h"
#include "EveAnimationSequencer.h"
#include "EveAnimationState.h"
#include "SpaceObject/EveSpaceObject2.h"

BLUE_DEFINE( EveAnimationStateSequencer );

const Be::ClassInfo* EveAnimationStateSequencer::ExposeToBlue()
{
    EXPOSURE_BEGIN( EveAnimationStateSequencer, "" )
        MAP_INTERFACE( EveAnimationStateSequencer )
        MAP_INTERFACE( IRoot )
		
		MAP_ATTRIBUTE( "owner", m_owner, "", Be::READWRITE )
		MAP_ATTRIBUTE( "update", m_update, "", Be::READWRITE )
		MAP_ATTRIBUTE( "currentState", m_currentState, "", Be::READ )
		MAP_ATTRIBUTE( "pendingState", m_pendingStates, "", Be::READ )

		MAP_METHOD_AND_WRAP( "Clear", Clear, "" )
		MAP_METHOD_AND_WRAP( "SetCurrentState", PushState, "" )

    EXPOSURE_END()
}