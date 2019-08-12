#include "StdAfx.h"
#include "ProcessLifetime.h"

ProcessLifetime::ProcessLifetime( IRoot* lockobj ) :
	m_tunnelGroupType( ENTRANCE_TUNNELS ),
	m_firstAgentLifetime( 0 ),
	m_returningAge( -1 ),
	m_useEntryTunnels( true ),
	m_useExitTunnels( true )
{
}

ProcessLifetime::~ProcessLifetime()
{
}


std::vector<Vector3> ProcessLifetime::CalculateBehavior(std::vector<DroneAgent>& agents, const float deltaTime,
                                                      BehaviorGroup& group, EveChildBehaviorSystem& system)
{
	auto tunnels = system.GetTunnels();

	if ( !agents.empty() )
	{
		m_firstAgentLifetime = agents.begin()->lifetime;
	}

	std::vector<Vector3> forceVectors;
	return forceVectors;
}


void ProcessLifetime::RenderDebugInfo( Tr2DebugRenderer& renderer, std::vector<DroneAgent>& agents, Matrix& parentWorldLocation )
{
	return;
}