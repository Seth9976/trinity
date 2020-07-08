#include "StdAfx.h"
#include "CollisionAvoidance.h"

CollisionAvoidance::CollisionAvoidance( IRoot* lockobj ) :
	PARENTLOCK( m_exclusionVolumes ),
	m_collisionAvoidanceScalar( 20.f ) 
{
}

CollisionAvoidance::~CollisionAvoidance()
{
}

int CollisionAvoidance::GetProcessPriority()
{
	return PROCESS_LAST;
}

std::vector<Vector3> CollisionAvoidance::CalculateBehavior( std::vector<DroneAgent>& agents, void* scratchData, const float deltaTime,
	BehaviorGroup& group, EveChildBehaviorSystem& system, const std::vector < std::vector<DroneAgent*>>& dronesInSearchRadius)
{
	std::vector<Vector3> forceVectors;
	for( auto agent = agents.begin(); agent != agents.end(); ++agent )
	{
		float force = 0.f;
		Vector3 avoidanceForce( 0.0f, 0.0f, 0.0f );
		for( auto exclusionVolume = m_exclusionVolumes.begin(); exclusionVolume != m_exclusionVolumes.end(); ++exclusionVolume )
		{
			Vector3 targetPos = ( *exclusionVolume )->GetBoundingSphere().GetXYZ();
			float targetRadius = ( *exclusionVolume )->GetBoundingSphere().w;
			Vector3 dronePos = agent->position;
			float droneRadius = group.GetBoundingSphereRadius();

			float dynamicLength = Length( agent->velocity ) / group.GetMaxVelocity();
			Vector3 ahead = (dronePos + Normalize( agent->velocity ) ) * dynamicLength;

			// 1. Get both radius and distance between drone and target
			// 2. if rad1 + rad2 is >= distance then we have collision
			// 3. if collision get direction of target to drone and move drone in that direction
			float dist = sqrt( pow( dronePos.x - targetPos.x, 2 ) + pow( dronePos.y - targetPos.y, 2 ) + pow( dronePos.z - targetPos.z, 2 ) );

			if( dist < targetRadius + droneRadius )
			{
				avoidanceForce = ahead - targetPos;
				float intensity = ( *exclusionVolume )->GetIntensity( dronePos );
				force = dist * m_collisionAvoidanceScalar;
				avoidanceForce = Normalize( avoidanceForce ) * intensity;
			}
			else
			{
				force = 0.0f;
			}
		}
		agent->acceleration += avoidanceForce * force;
		if( group.m_collectForces )
		{
			Vector3 forceOffset = Normalize( avoidanceForce ) * group.GetBoundingSphereRadius();
			forceVectors.push_back( agent->position + forceOffset );
			forceVectors.push_back( avoidanceForce  * force );
		}

	}
	return forceVectors;
}

void CollisionAvoidance::GetDebugOptions( Tr2DebugRendererOptions& options )
{
	options.insert( "ExclusionVolumes" );
}

void CollisionAvoidance::RenderDebugInfo( ITr2DebugRenderer2& renderer, std::vector<DroneAgent>& agents, Matrix& parentWorldLocation )
{
	if( renderer.HasOption( this, "ExclusionVolumes" ) )
	{
		for( auto volume = m_exclusionVolumes.begin(); volume != m_exclusionVolumes.end(); ++volume )
		{
			( *volume )->RenderDebugInfo( renderer, parentWorldLocation );
		}
	}
}