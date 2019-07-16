
#include "StdAfx.h"
#include "BehaviorGroup.h"

BLUE_DEFINE( BehaviorGroup );

const Be::ClassInfo* BehaviorGroup::ExposeToBlue()
{
	EXPOSURE_BEGIN(BehaviorGroup, "")
		MAP_INTERFACE( BehaviorGroup )
		
		MAP_ATTRIBUTE( "display", m_display, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE("count", m_count, "Number of ships", Be::READ | Be::PERSIST)
		MAP_ATTRIBUTE("maxVelocity", m_maxVelocity, "", Be::READWRITE | Be::PERSIST)
		MAP_ATTRIBUTE("behaviors", m_behaviors, "What the ships do etc", Be::READ | Be::PERSIST)
		MAP_ATTRIBUTE( "volumes", m_volumes, "", Be::READ | Be::PERSIST )
		MAP_ATTRIBUTE( "exclusionVolumes", m_exclusionVolumes, "", Be::READ | Be::PERSIST )
		MAP_ATTRIBUTE( "mesh", m_mesh, "the drone mesh", Be::READWRITE | Be::PERSIST | Be::NOTIFY)
		MAP_ATTRIBUTE( "scale", m_scale, "Agent local scaling", Be::READWRITE | Be::PERSIST )

		MAP_METHOD_AND_WRAP( "AddAgent", AddAgent, "Adds a drone to the swarm \n:jessica-placement: TOOLBAR\n:jessica-icon: far-drone-alt\n" )
		MAP_METHOD_AND_WRAP( "RemoveAgent", RemoveAgent, "removes a random drone from the swarm \n:jessica-placement: TOOLBAR\n:jessica-icon: far-dumpster\n" )
		MAP_METHOD_AND_WRAP( "SetCount", SetCount, "Specify a desired number of agents for the system \n:param count: number of agents\n:jessica-placement: TOOLBAR\n:jessica-icon: far-ball-pile\n" )

	EXPOSURE_END()
}