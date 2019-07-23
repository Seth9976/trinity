
#include "StdAfx.h"
#include "BehaviorGroup.h"
#include "Eve/EveUpdateContext.h"
#include "include/TriMath.h"
#include "IBehavior.h"
#include "Tr2InstancedMesh.h"

//@Leó ->camera. | might need refactor 
#include "Eve/SpaceObject/Children/TransformModifiers/EveChildModifierTransformCommon.h" 
#include "Resources/TriGeometryRes.h"



BehaviorGroup::BehaviorGroup( IRoot* lockobj ) :
	m_vertexDeclarationHandle( Tr2EffectStateManager::UNINITIALIZED_DECLARATION ),
	m_count( 0 ),
	m_display( true ),
	m_meshToggle( false ),
	m_maxVelocity( 100 ),
	m_changeBufferVertexCount( nullptr ),
	m_scale( Vector3( 1, 1, 1 ) ),
	m_spriteScale( Vector3( 10, 10, 10 ) ),
	PARENTLOCK( m_behaviors ),
	PARENTLOCK( m_volumes ),
	PARENTLOCK( m_exclusionVolumes ),
	m_blendRangeMax( 1000 ), // LOD system
	m_blendRangeMin( 500 ),
	m_blendRangeValue( 1.0 )
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

size_t BehaviorGroup::GetSize()
{
	return m_agents.size();
}

unsigned int BehaviorGroup::GetCount()
{
	return unsigned(m_count);
}

// For Artists when they are creating the sprite to easily swap between mesh's
void BehaviorGroup::ToggleMesh()
{
	m_meshToggle = !m_meshToggle;
}

Tr2MeshPtr BehaviorGroup::GetMesh() const
{
	auto mesh = m_meshToggle ? m_spriteMesh : m_mesh;
	return mesh;
}

Tr2MeshPtr BehaviorGroup::GetSpriteMesh() const
{
	return m_spriteMesh;
}

unsigned int BehaviorGroup::GetVertexDeclarationHandle() const
{
	return m_vertexDeclarationHandle;
}

unsigned int BehaviorGroup::GetSpriteVertexDeclarationHandle() const
{
	return m_spriteVertexDeclarationHandle;
}

void BehaviorGroup::SetGroupIndexIndicator( int index )
{
	m_groupIndex = index;
}

int BehaviorGroup::GetGroupIndexIndicator() const
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

float BehaviorGroup::AllTheSame()
{
	float same = -1;
	// if none of the agents need either of the meshes we let the system know
	for (auto agent = m_agents.begin(); agent != m_agents.end(); ++agent)
	{
		if (same == -1) same = (agent->cameraDistance);
		if (same != (agent->cameraDistance)) return -1;
	}
	return same;
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
	//Move the agents based on the behaviors and update LOD factor
	for ( auto agent = m_agents.begin(); agent != m_agents.end(); ++agent )
	{
		agent->lifetime += dt;
		agent->velocity = ClampLength( agent->velocity + agent->acceleration * dt, m_maxVelocity );
		//agent->acceleration = Vector3( 0, 0, 0 );
		agent->position += agent->velocity * dt;

		static const Vector3 zAxis(0.f, 0.f, 1.f);
		TriQuaternionRotationArc( &agent->rotation, &zAxis, &agent->velocity );
		ProcessLOD( *agent );
	}
}

void BehaviorGroup::ProcessLOD( DroneAgent& agent )
{
	// Calculate agent distance from camera.
	// Get camera distance
	Vector3 camPos;
	camPos = TransformCoord( camPos, Tr2Renderer::GetViewTransform() );

	Vector3 aPos = agent.position;
	Vector3 diff = Vector3( (camPos.x - aPos.x), (camPos.y - aPos.y), (camPos.z - aPos.z) );
	float dist = std::sqrt( std::pow( diff.x, 2 ) + std::pow( diff.y, 2 ) + std::pow( diff.z, 2 ) );

	// Normalize the distance so that dist <= m_blendRangeMin = 0.0 and dist >= m_blendRangeMax = 1.0
	if (dist >= m_blendRangeMax) {
		agent.cameraDistance = 1.0;
		return;
	}
	else if (dist <= m_blendRangeMin) {
		agent.cameraDistance = 0.0;
		return;
	}
	else {
		float range = m_blendRangeMax - m_blendRangeMin;
		float normDist = (dist - range) / range;
		agent.cameraDistance = max(0.0f,min( normDist, 1.0f));
	}
}

void BehaviorGroup::GetInfoForBuffer(uint8_t* data, Matrix& parentWorldLocation )
{
	for ( auto agent = m_agents.begin(); agent != m_agents.end(); ++agent )
	{
		float LOD = (*agent).cameraDistance;
		float LODmod;
		if (LOD != 1)
		{
			LODmod = (1 - LOD) *( 0.5f + (1 - LOD) * 0.5f);
			Vector3 meshScale = m_scale * Vector3( LODmod, LODmod, LODmod );

			if ( m_meshToggle )
			{
				meshScale *= m_spriteScale;
			}

			Matrix m = Transpose( TransformationMatrix( meshScale, agent->rotation, agent->position ) );
			memcpy( data, &m, 12 * sizeof( float ) );
		} 
		else
		{
			Matrix m = Transpose( TransformationMatrix( Vector3( 0, 0, 0 ), Quaternion(0, 0, 0, 0), Vector3( 0, 0, 0 ) ) );
			memcpy( data, &m, 12 * sizeof( float ) );
		}

		data += 12 * sizeof( float );

		// sprite
		if (LOD != 0)
		{
			LODmod = ( 1.0f - LOD) * (LOD * 0.3f) +  (LOD * 1.0f);
			Matrix agentMatrix = TransformationMatrix( m_scale * m_spriteScale * Vector3( LODmod, LODmod, LODmod ),
				agent->rotation, agent->position );
			agentMatrix = Billboard2D( agentMatrix );
			Matrix m2 = Transpose( agentMatrix );
			memcpy( data, &m2, 12 * sizeof( float ) );
		}
		data += 12 * sizeof( float );
	}
}

void BehaviorGroup::CreateSpriteVertexDeclaration()
{
	Tr2MeshPtr meshPtr = GetSpriteMesh();

	if ( meshPtr )
	{
		if (nullptr == meshPtr->GetGeometryResource())
		{
			return;
		}

		if ((meshPtr->GetGeometryResource())->IsGood())
		{
			TriGeometryResMeshData* meshData = meshPtr->GetGeometryResource()->GetMeshData( meshPtr->GetMeshIndex() );
			if (meshData->m_vertexDeclaration != m_cachedSVD)
			{
				Tr2VertexDefinition s_agentInstancedVertex;
				Tr2EffectStateManager::GetVertexDeclarationElements( meshData->m_vertexDeclaration, s_agentInstancedVertex );

				Tr2VertexDefinition& def = s_agentInstancedVertex;
				def.Add( Tr2VertexDefinition::FLOAT32_4, Tr2VertexDefinition::TEXCOORD, 8, 1, 1 );
				def.Add( Tr2VertexDefinition::FLOAT32_4, Tr2VertexDefinition::TEXCOORD, 9, 1, 1 );
				def.Add( Tr2VertexDefinition::FLOAT32_4, Tr2VertexDefinition::TEXCOORD, 10, 1, 1 );

				// create vertex-declarartion
				m_spriteVertexDeclarationHandle = Tr2EffectStateManager::GetVertexDeclarationHandle( s_agentInstancedVertex );

				m_cachedSVD = meshData->m_vertexDeclaration;
			}
			return;
		}
	}
	m_cachedSVD = -1;
	m_spriteVertexDeclarationHandle = 0;
}


void BehaviorGroup::CreateVertexDeclaration()
{
	Tr2MeshPtr meshPtr = GetMesh();

	if (meshPtr)
	{
		if (nullptr == meshPtr->GetGeometryResource())
		{
			return;
		}

		if ((meshPtr->GetGeometryResource())->IsGood())
		{
			TriGeometryResMeshData* meshData = meshPtr->GetGeometryResource()->GetMeshData( meshPtr->GetMeshIndex() );
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

