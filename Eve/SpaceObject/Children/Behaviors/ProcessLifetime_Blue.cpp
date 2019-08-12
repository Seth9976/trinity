#include "StdAfx.h"
#include "ProcessLifetime.h"

Be::VarChooser ProcessLifetimeTunnelGroupTypeChooser[] =
{
	{ "Exit_Tunnels", BeCast( SplineTunnelGroup::EXIT_TUNNELS ), "Tunnels Drones flock to when set to exit the scene" },
	{ "Entrance_Tunnels", BeCast( SplineTunnelGroup::ENTRANCE_TUNNELS ), "Tunnels Drones flock to when entering the scene" },
	{ "Other_Tunnels", BeCast( SplineTunnelGroup::OTHER_TUNNELS ), "pathways in the scene (hallways etc)" },
	{ 0 }
};
BLUE_REGISTER_ENUM_EX( "setTunnelType", SplineTunnelGroup::TunnelGroupType, ProcessLifetimeTunnelGroupTypeChooser, ENUM_REG_ENUM_OBJECT_ON_MODULE );


BLUE_DEFINE( ProcessLifetime );

const Be::ClassInfo* ProcessLifetime::ExposeToBlue()
{
	EXPOSURE_BEGIN( ProcessLifetime, "" )
		MAP_INTERFACE( ProcessLifetime )
		MAP_INTERFACE( IBehavior )

		MAP_ATTRIBUTE_WITH_CHOOSER( "tunnelGroupType", m_tunnelGroupType, "control when drones interact with these tunnels",
											Be::READWRITE | Be::PERSIST | Be::ENUM, ProcessLifetimeTunnelGroupTypeChooser )

		MAP_ATTRIBUTE( "firstAgentLifetime", m_firstAgentLifetime, "Current lifetime for first agent in the group", Be::READ )
		MAP_ATTRIBUTE( "returningAge", m_returningAge, "designate when agent should exit the scene", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "useEntryTunnels", m_useEntryTunnels, "should it look for and use entry_tunnels when it spawns", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "useExitTunnels", m_useExitTunnels, "should it look for and use exit_tunnels when it spawns", Be::READWRITE | Be::PERSIST )

	EXPOSURE_END()
}