
#include "StdAfx.h"
#include "BehaviorGroup.h"
#include "Eve/EveUpdateContext.h"
#include "include/TriMath.h"
#include "IBehavior.h"
#include "Tr2InstancedMesh.h"

#include "Resources/TriGeometryRes.h"



BehaviorGroup::BehaviorGroup( IRoot* lockobj ) :
	m_vertexDeclarationHandle( Tr2EffectStateManager::UNINITIALIZED_DECLARATION ),
	m_count( 0 ),
	m_display( true ),
	m_maxVelocity( 100 ),
	m_changeBufferVertexCount( nullptr ),
	m_scale( Vector3( 1, 1, 1 ) ),
	PARENTLOCK(m_behaviors),
	PARENTLOCK( m_volumes ),
	PARENTLOCK( m_exclusionVolumes )
{
}

// --------------------------------------------------------------------------------
// Description:
//   Load the geometry resource, might be a re-load
// SeeAlso:
//   IBlueResource, TriGeometryRes
// --------------------------------------------------------------------------------
void BehaviorGroup::InitializeGeometryResource()
{
	//to make resource load correctly they must be regenerated for this instance
	m_agents.clear();
	const int t = m_count;
	m_count = 0;
	SetCount(t);
}

void BehaviorGroup::SetVertexFunctionReferance( const std::function<void(void)> &F)
{
	m_changeBufferVertexCount = F;
}

BehaviorGroup::~BehaviorGroup()
{
}

size_t BehaviorGroup::GetSize() const
{
	return m_agents.size();
}

Tr2MeshPtr BehaviorGroup::GetMesh()
{
	return m_mesh;
}

unsigned int BehaviorGroup::GetVertexDeclarationHandle() const
{
	return m_vertexDeclarationHandle;
}

void BehaviorGroup::SetGroupIndexIndicator( int index )
{
	m_groupIndex = index;
}

int BehaviorGroup::GetGroupIndexIndicator()
{
	return m_groupIndex;
}

void BehaviorGroup::AddAgent()
{
	// The function without arguments to be called from the UI
	AddAgentPrivate();
	if (m_changeBufferVertexCount)
	{
		(m_changeBufferVertexCount)();
	}
}
	

void BehaviorGroup::AddAgentPrivate()
{
	DroneAgent agent;
	agent.position = Vector3( 0, 0, 0 ); // Later we might want to find a 'smart' spawn location
	agent.id = TriRandInt( 500 ); //TODO look better into parameter
	m_agents.push_back( agent );
	m_count++;
	if (m_changeBufferVertexCount == nullptr)
	{
		return;
	}
}

void BehaviorGroup::SetCount( int count )
{
	if (count == m_count || count < 0)
	{
		return;
	}

	if ( m_count < count )
	{
		int difference = count - m_count;
		for ( int i = 0; i < difference; i++ )
		{
			AddAgentPrivate();
		}
	}
	else 
	{
		int difference =  m_count - count;
		for ( int i = 0; i < difference; i++ )
		{
			RemoveAgentPrivate();
		}
	}

	if (m_changeBufferVertexCount == nullptr)
	{
		return;
	}
	(m_changeBufferVertexCount)();
}

void BehaviorGroup::RemoveAgent()
{
	// The function without arguments to be called from the UI
	RemoveAgentPrivate();

	if (m_changeBufferVertexCount)
	{
		(m_changeBufferVertexCount)();
	}
}

// returns a vector so we can do something with the location. explosion etc
Vector3 BehaviorGroup::RemoveAgentPrivate()
{
	if ( m_agents.empty() )
	{
		return Vector3( 0, 0, 0 );
	}

	int randDrone = TriRandInt( m_count );
	DroneAgent v = m_agents[randDrone];
	m_agents[randDrone] = m_agents.back();
	m_agents.pop_back();
	m_count--;

	return v.position;
}

void BehaviorGroup::UpdateAgents(const float dt)
{
	//Calculate the behaviors
	for (auto agent = m_agents.begin(); agent != m_agents.end(); ++agent)
	{
		for (auto behavior = m_behaviors.begin(); behavior != m_behaviors.end(); ++behavior)
		{
			//Rather send a list of all agents and in each behavior loop over them and apply the behavior
			(*behavior)->CalculateBehavior( *agent, dt, *this );
		}
	}
	//Move the agents based on the behaviors
	for ( auto agent = m_agents.begin(); agent != m_agents.end(); ++agent )
	{
		agent->lifetime += dt;
		agent->velocity = ClampLength( agent->velocity + agent->acceleration * dt, m_maxVelocity );
		//agent->acceleration = Vector3( 0, 0, 0 );
		agent->position += agent->velocity * dt;

		static const Vector3 zAxis(0.f, 0.f, 1.f);
		TriQuaternionRotationArc( &agent->rotation, &zAxis, &agent->velocity );
	}
}


void BehaviorGroup::GetInfoForBuffer(uint8_t* data, Matrix& parentWorldLocation )
{
	for (auto agent = m_agents.begin(); agent != m_agents.end(); ++agent)
	{
		Matrix m = Transpose( TransformationMatrix( m_scale, agent->rotation, agent->position ) );
		memcpy(data, &m, 12 * sizeof(float));
		data += 12 * sizeof(float);
	}
}

void BehaviorGroup::CreateVertexDeclaration()
{
	if (m_mesh)
	{
		if (nullptr == m_mesh->GetGeometryResource())
		{
			return;
		}

		if ((m_mesh->GetGeometryResource())->IsGood())
		{
			TriGeometryResMeshData* meshData = m_mesh->GetGeometryResource()->GetMeshData( m_mesh->GetMeshIndex() );
			if (meshData->m_vertexDeclaration != m_cachedVD)
			{
				Tr2VertexDefinition s_agentInstancedVertex;
				Tr2EffectStateManager::GetVertexDeclarationElements( meshData->m_vertexDeclaration, s_agentInstancedVertex );

				Tr2VertexDefinition& def = s_agentInstancedVertex;
				def.Add( Tr2VertexDefinition::FLOAT32_4, Tr2VertexDefinition::TEXCOORD, 8, 1, 1 );
				def.Add( Tr2VertexDefinition::FLOAT32_4, Tr2VertexDefinition::TEXCOORD, 9, 1, 1 );
				def.Add( Tr2VertexDefinition::FLOAT32_4, Tr2VertexDefinition::TEXCOORD, 10, 1, 1 );

				// create vertex-declarartion
				m_vertexDeclarationHandle = Tr2EffectStateManager::GetVertexDeclarationHandle( s_agentInstancedVertex );

				m_cachedVD = meshData->m_vertexDeclaration;
			}
			return;
		}
		
	}
	m_cachedVD = -1;
	m_vertexDeclarationHandle = 0;
}

/////////////////////////////////////////////////////////////////////////////////////
// ITr2DebugRenderable
void BehaviorGroup::GetDebugOptions( Tr2DebugRendererOptions& options)
{
	options.insert( "Agents" );
	options.insert( "Volumes" );
	options.insert( "ExclusionVolumes" );
}

void BehaviorGroup::RenderDebugInfo( Tr2DebugRenderer& renderer, Matrix& parentWorldLocation )
{
	if ( renderer.HasOption(this, "Agents") )
	{
		for (auto agent = m_agents.begin(); agent != m_agents.end(); ++agent)
		{
			renderer.DrawSphere(this, TranslationMatrix(agent->position) * parentWorldLocation, m_scale.z * 5, 6, Tr2DebugRenderer::Wireframe, 0xff555555);
		}
	}

	if (renderer.HasOption( this, "Volumes" ))
	{
		for (auto volume = m_volumes.begin(); volume != m_volumes.end(); ++volume)
		{
			(*volume)->RenderDebugInfo( renderer, parentWorldLocation );
		}
	}

	if (renderer.HasOption( this, "ExclusionVolumes" ))
	{
		for (auto volume = m_exclusionVolumes.begin(); volume != m_exclusionVolumes.end(); ++volume)
		{
			(*volume)->RenderDebugInfo( renderer, parentWorldLocation );
		}
	}
}

