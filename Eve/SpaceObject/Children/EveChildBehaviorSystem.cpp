#include "StdAfx.h"
#include "EveChildBehaviorSystem.h"
#include "Eve/EveUpdateContext.h"
#include "TriRenderBatch.h"
#include "Behaviors/IBehavior.h"
#include "Tr2Mesh.h"
#include "Resources/TriGeometryRes.h"

namespace
{
	class EveChildBehaviorSystemPerObjectData : public Tr2PerObjectData
	{
	public:
		virtual void SetPerObjectDataToDevice( Tr2ConstantBufferAL** buffers, unsigned constantTypeMask, Tr2RenderContext& renderContext ) const
		{
			FillAndSetConstants( *buffers[Tr2RenderContextEnum::VERTEX_SHADER],
				m_vsData, sizeof( *m_vsData ),
				Tr2RenderContextEnum::VERTEX_SHADER,
				Tr2Renderer::GetPerObjectVSStartRegister(),
				renderContext );
			FillAndSetConstants( *buffers[Tr2RenderContextEnum::PIXEL_SHADER],
				m_psData, sizeof( *m_psData ),
				Tr2RenderContextEnum::PIXEL_SHADER,
				Tr2Renderer::GetPerObjectPSStartRegister(),
				renderContext );
		}
		EveSpaceObjectPSData* m_psData;
		EveSpaceObjectVSData* m_vsData;
	};
}

// --------------------------------------------------------------------------------------
// Description
//   Render batch specialization for EveChildContainer-batches
// See Also
//   TriRenderBatch, ITr2GeometryBatch
// --------------------------------------------------------------------------------------
class TriBehaviorSystemInstancingBatch : public TriGeometryBatch
{
public:

	void SetGeometryResource( TriGeometryRes* val )
	{
		m_geometryResource = val;
	}

	// Set the ground
	void SetGround( BehaviorGroup* val )
	{
		m_ground = val;
	}

	// Set the geometry provider
	void SetBehaviorSystemReference( EveChildBehaviorSystem* val )
	{
		m_geom = val;
	}

	// Forward the SubmitGeometry call to the geometry provider
	void SubmitGeometry( Tr2RenderContext& renderContext )
	{
		if (m_geom)
		{
			auto r = GetGeometryResource();
			if (m_ground->m_display)
				m_geom->Draw( this, renderContext, static_cast<unsigned int>(m_ground->GetSize()),
					m_ground->GetVertexDeclarationHandle(), m_ground->GetGroupIndexIndicator() );
		}
	}

	// Gets the batch type name for PIX debugging
	virtual const std::string& GetBatchTypeName( void ) const
	{
		static const std::string name = "TriGroupInstancingBatch";
		return name;
	}

private:
	EveChildBehaviorSystemPtr m_geom;
	BehaviorGroupPtr m_ground;
};


EveChildBehaviorSystem::EveChildBehaviorSystem( IRoot* lockobj ) :
	m_stride( 12 * sizeof( float ) ),
	m_vertexCount( 1 ),
	m_maxVelocity( 100 ),
	m_maxForce( 50.f ),
	m_display(true),
	PARENTLOCK( m_behaviorGroups ),
	m_needToPassInVertexFunction( true )
{
	m_behaviorGroups.SetNotify( this );
	PrepareResources();
}

EveChildBehaviorSystem::~EveChildBehaviorSystem()
{
}

bool EveChildBehaviorSystem::Initialize()
{
	if (m_staticTransform)
	{
		RebuildLocalTransform();
	}

	ChangeBufferVertexCount();

	return true;
}

void EveChildBehaviorSystem::OnListModified( long event, ssize_t key, ssize_t key2, IRoot* value, const struct IList* theList )
{
	if (theList == &m_behaviorGroups)
	{
		switch (event & BELIST_EVENTMASK)
		{
		case BELIST_INSERTED:
			if (BehaviorGroupPtr handler = BlueCastPtr( value ))
			{
				std::function<void( void )> f = std::bind( &EveChildBehaviorSystem::ChangeBufferVertexCount, this );
				handler->SetVertexFunctionReferance( f );
				handler->InitializeGeometryResource();
			}
			break;
		case BELIST_REMOVED:
			if (BehaviorGroupPtr handler = BlueCastPtr( value ))
			{
				ChangeBufferVertexCount();
			}
			break;
		case BELIST_LOADFINISHED:
			if (BehaviorGroupPtr handler = BlueCastPtr( value ))
			{
				std::function<void( void )> f = std::bind( &EveChildBehaviorSystem::ChangeBufferVertexCount, this );
				handler->SetVertexFunctionReferance( f );
				handler->InitializeGeometryResource();
			}
			else m_needToPassInVertexFunction = true; //this is for when this file is loaded but the groups have yet to be loaded
			break;
		default:
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// Tr2DeviceResource
bool EveChildBehaviorSystem::OnPrepareResources()
{
	USE_MAIN_THREAD_RENDER_CONTEXT();

	auto hr = m_vertexBuffer.Create(
		m_stride,					// 12 * sizeof( float )
		m_vertexCount,				// Number of instances
		Tr2GpuUsage::VERTEX_BUFFER, // VERTEX_BUFFER
		Tr2CpuUsage::WRITE_OFTEN,	// WRITE_OFTEN
		nullptr,					// initialData
		renderContext );
	if( FAILED( hr ) )
	{
		return false;
	}

	return true;
}

// --------------------------------------------------------------------------------------
// Description:
//   Implements ITr2InstanceData interface. Returns number of instance buffers.
// Return value:
//   1 always
// --------------------------------------------------------------------------------------
unsigned int EveChildBehaviorSystem::GetInstanceBufferCount() const
{
	return 1;
}


// --------------------------------------------------------------------------------------
// Description:
//   Implements ITr2InstanceData interface. Returns number of instances in the instance 
//   buffer.
// Arguments:
//   bufferIndex - (unused) instance buffer index
// Return value:
//   Number of instances
// --------------------------------------------------------------------------------------
unsigned int EveChildBehaviorSystem::GetInstanceBufferVertexCount( unsigned int bufferIndex ) const
{
	size_t size = 0;
	for (auto it = begin( m_behaviorGroups ); it != end( m_behaviorGroups ); ++it)
	{
		size += (*it)->GetSize();
	}
	return unsigned(size);
}

// --------------------------------------------------------------------------------------
// Description:
//   Implements ITr2InstanceData interface. Returns vertex buffer with instance data.
// Arguments:
//   bufferIndex - (unused) instance buffer index
//   buffer - (out) vertex buffer containing instance data (can be null)
//   stride - (out) vertex stride for the vertex buffer
// --------------------------------------------------------------------------------------
void EveChildBehaviorSystem::GetVertexBuffer( unsigned int bufferIndex, Tr2BufferAL& buffer, unsigned& stride )
{
	buffer = m_vertexBuffer;
	stride = m_stride;
}

// A Simple function to handle cases where a behavior system is loaded along with all it's children from a single file
// The onListNotify takes care of all other cases (like when just a new behavior group is loaded)
void EveChildBehaviorSystem::PassInVertexesToBehaviorGroups()
{
	for (auto it = begin( m_behaviorGroups ); it != end( m_behaviorGroups ); ++it)
	{
		std::function<void( void )> f = std::bind( &EveChildBehaviorSystem::ChangeBufferVertexCount, this );
		(*it)->SetVertexFunctionReferance( f );
		(*it)->InitializeGeometryResource();
	}
	m_needToPassInVertexFunction = false;
	ChangeBufferVertexCount();	
}

/////////////////////////////////////////////////////////////////////////////////////
// EveChildMesh
void EveChildBehaviorSystem::UpdateSyncronous( EveUpdateContext& updateContext, const EveChildUpdateParams& params )
{
	UpdateAgents( updateContext.GetDeltaT() );

	if (m_needToPassInVertexFunction) PassInVertexesToBehaviorGroups();

	USE_MAIN_THREAD_RENDER_CONTEXT();
	UpdateBuffer( renderContext );

	for (auto it = begin( m_behaviorGroups ); it != end( m_behaviorGroups ); ++it)
	{
		(*it)->CreateVertexDeclaration();
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// EveChildBehaviorSystem
void EveChildBehaviorSystem::UpdateAgents( const float dt )
{
	for (auto it = begin( m_behaviorGroups ); it != end( m_behaviorGroups ); ++it)
	{
		(*it)->UpdateAgents( dt );
	}
}

void EveChildBehaviorSystem::UpdateBuffer( Tr2RenderContext& renderContext )
{
	m_offsets.clear();
	m_offsets.push_back( 0 );
	uint8_t *data;
	Matrix WT = EveChildTransform::m_worldTransform;
	CR_RETURN( m_vertexBuffer.MapForWriting( data, renderContext ) );
	uint8_t I = 0;
	uint8_t totalShipsSoFar = 0;
	for (auto it = begin( m_behaviorGroups ); it != end( m_behaviorGroups ); ++it)
	{
		(*it)->GetInfoForBuffer( data, WT );
		(*it)->SetGroupIndexIndicator( I ); 
		data += static_cast<unsigned int>((*it)->GetSize()) * m_stride;
		I++;
		totalShipsSoFar += static_cast<unsigned int>((*it)->GetSize());
		m_offsets.push_back( totalShipsSoFar * m_stride );
	}
	m_vertexBuffer.UnmapForWriting( renderContext );
}

void EveChildBehaviorSystem::GetBatches( ITriRenderBatchAccumulator* batches, TriBatchType batchType, const Tr2PerObjectData* perObjectData )
{
	if (!m_display)
	{
		return;
	}

	if (!m_vertexBuffer.IsValid())
	{
		return;
	}

	for (auto it = begin( m_behaviorGroups ); it != end( m_behaviorGroups ); ++it)
	{
		auto group = *it;
		auto mesh = group->GetMesh();
		
		if (mesh == nullptr)
		{
			continue;
		}
		
		if (mesh->GetGeometryResource() == nullptr)
		{
			continue;
		}

		if (!(mesh->GetGeometryResource()->IsGood()))
		{
			continue;
		}
		
		if (mesh->GetGeometryResource()->GetMeshCount() < 1)
		{
			continue;
		}

		auto areaList = mesh->GetAreas( batchType );
		for (auto srcMeshArea = areaList->begin(); srcMeshArea != areaList->end(); ++srcMeshArea)
		{
			auto a = *srcMeshArea;

			TriBehaviorSystemInstancingBatch* batch = batches->Allocate<TriBehaviorSystemInstancingBatch>();

			if (nullptr == batch)
			{
				continue;
			}
				
			batch->SetPerObjectData( perObjectData );
			batch->SetShaderMaterial( a->GetMaterialInterface());
			batch->SetMeshParameters( mesh->GetMeshIndex(), a->GetIndex(), a->GetCount(), a->IsReversed() );
			batch->SetGeometryResource( mesh->GetGeometryResource() );
			batch->SetBehaviorSystemReference( this );
			batch->SetGround( group );
			batches->Commit( batch );
		}
	}
}


bool EveChildBehaviorSystem::HasTransparentBatches()
{
	bool isTrue = false;
	for (auto it = begin( m_behaviorGroups ); it != end( m_behaviorGroups ); ++it)
	{
		auto mesh = (*it)->GetMesh();
		if (m_display && mesh)
		{
			if(!(mesh->GetAreas( TRIBATCHTYPE_TRANSPARENT )->empty())) isTrue = true;
		}
	}

	return isTrue;
}

// --------------------------------------------------------------------------------
// Description:
//   No shadows here
// --------------------------------------------------------------------------------
void EveChildBehaviorSystem::GetShadowBatches( ITriRenderBatchAccumulator* batches, const Tr2PerObjectData* perObjectData )
{
}

// --------------------------------------------------------------------------------
// Description:
//   No transparency, no sorting
// --------------------------------------------------------------------------------
float EveChildBehaviorSystem::GetSortValue()
{
	return 0.f;
}

// --------------------------------------------------------------------------------
// Description:
//   The perobject data
// --------------------------------------------------------------------------------
Tr2PerObjectData* EveChildBehaviorSystem::GetPerObjectData( ITriRenderBatchAccumulator* accumulator )
{
	EveChildBehaviorSystemPerObjectData* perObjectData = accumulator->Allocate<EveChildBehaviorSystemPerObjectData>();

	if (!perObjectData)
	{
		return nullptr;
	}

	perObjectData->m_vsData = &m_vsData;
	perObjectData->m_psData = &m_psData;
	return perObjectData;
}

// --------------------------------------------------------------------------------
// Description:
//   Setup instanced reandering and call DIP
// --------------------------------------------------------------------------------
void EveChildBehaviorSystem::Draw( TriBehaviorSystemInstancingBatch* batch, Tr2RenderContext& renderContext, int count, unsigned int vertexDecl, int groupIndex )
{
	auto geometry = batch->GetGeometryResource();
	
	if (geometry == nullptr) { return; }
	if (!(geometry->IsGood())) { return; }
	if (geometry->GetMeshCount() < 1) { return; }
	if (!m_vertexBuffer.IsValid()) { return; }
	
	const TriGeometryResMeshData* meshData = geometry->GetMeshData( 0 );
	auto areaIx = batch->GetAreaIndex();
	auto areaCount = batch->GetAreaCount();

	if (areaIx >= meshData->m_areas.size())
	{
		return;
	}

	if (areaIx + areaCount > meshData->m_areas.size())
	{
		areaCount = static_cast<unsigned int>( meshData->m_areas.size() ) - areaIx;
	}

	const TriGeometryResAreaData& area = meshData->m_areas[areaIx];

	unsigned int primCount = area.m_primitiveCount;
	for (unsigned int i = 1; i < areaCount; ++i)
	{
		const TriGeometryResAreaData& curArea = meshData->m_areas[areaIx + i];
		primCount += curArea.m_primitiveCount;
	}

	renderContext.m_esm.ApplyVertexDeclaration( vertexDecl );
	renderContext.m_esm.ApplyIndexBuffer( meshData->m_indexBuffer );
	// Stream 0: "geometry": here: our ship geometry
	renderContext.m_esm.ApplyStreamSource( 0, meshData->m_vertexBuffer, 0, meshData->m_bytesPerVertex );
	// Stream 1: instance", here: instance index
	renderContext.m_esm.ApplyStreamSource( 1, m_vertexBuffer, m_offsets[groupIndex], m_stride );

	renderContext.SetTopology( Tr2RenderContextEnum::TOP_TRIANGLES );
	renderContext.DrawIndexedInstanced( meshData->m_vertexCount, area.m_firstIndex, primCount, count );
}

/////////////////////////////////////////////////////////////////////////////////////
// ITr2DebugRenderable
void EveChildBehaviorSystem::GetDebugOptions( Tr2DebugRendererOptions& options )
{
	for (auto it = begin( m_behaviorGroups ); it != end( m_behaviorGroups ); ++it)
	{
		(*it)->GetDebugOptions( options );
	}
}

void EveChildBehaviorSystem::RenderDebugInfo( Tr2DebugRenderer& renderer )
{
	for (auto it = begin( m_behaviorGroups ); it != end( m_behaviorGroups ); ++it)
	{
		(*it)->RenderDebugInfo( renderer, EveChildTransform::m_worldTransform );
	}
}

void EveChildBehaviorSystem::ChangeBufferVertexCount( )
{
	USE_MAIN_THREAD_RENDER_CONTEXT();
	size_t temp = 0;
	
	for (auto it = begin( m_behaviorGroups ); it != end( m_behaviorGroups ); ++it)
	{
		temp += (*it)->GetSize();
	}

	unsigned int numAgents = static_cast<unsigned int>(temp);
	m_vertexCount = numAgents;
	auto b = m_vertexBuffer.Create(
		m_stride,				// 12 * sizeof( float )
		m_vertexCount,			// Number of instances
		Tr2GpuUsage::VERTEX_BUFFER, // VERTEX_BUFFER
		Tr2CpuUsage::WRITE_OFTEN,	// WRITE_OFTEN
		nullptr,
		renderContext );
}

/////////////////////////////////////////////////////////////////////////////////////
// IEveSpaceObjectChild
void EveChildBehaviorSystem::UpdateAsyncronous( EveUpdateContext& updateContext, const EveChildUpdateParams& params )
{
	Matrix localToWorldTransform;

	if (nullptr != params.childParent)
	{
		params.childParent->GetLocalToWorldTransform( localToWorldTransform );
	}
	else if (nullptr != params.spaceObjectParent)
	{
		params.spaceObjectParent->GetLocalToWorldTransform( localToWorldTransform );
		params.spaceObjectParent->GetPerObjectStructs( m_vsData, m_psData );
	}
	else
	{
		localToWorldTransform = params.localToWorldTransform;
	}
	m_vsData.worldTransformLast = Transpose( m_worldTransform );

	UpdateTransform( localToWorldTransform );

	m_vsData.worldTransform = Transpose( m_worldTransform );
	m_vsData.invWorldTransform = Inverse( m_worldTransform );
}

const char* EveChildBehaviorSystem::GetName() const 
{ 
	return m_name; 
}

void EveChildBehaviorSystem::Setup( const Vector3* scale, const Quaternion* rotation, const Vector3* translation, Tr2Lod lowestLodVisible )
{
	EveChildTransform::Setup( scale, rotation, translation, lowestLodVisible );
}

void EveChildBehaviorSystem::GetRenderables( std::vector<ITr2Renderable*>& renderables )
{
	renderables.push_back( this );
}

void EveChildBehaviorSystem::SetName( const char* name ) 
{
}

void EveChildBehaviorSystem::UpdateVisibility( const TriFrustum& frustum, const Matrix& parentTransform, Tr2Lod parentLod )
{
}

bool EveChildBehaviorSystem::GetBoundingSphere( Vector4& sphere, BoundingSphereQuery query  ) const 
{
	return true;
}

bool EveChildBehaviorSystem::GetInstanceBufferBoundingBox( unsigned int bufferIndex, Vector3& minBounds, Vector3& maxBounds ) const
{
	return false;
}

void EveChildBehaviorSystem::GetLocalToWorldTransform( Matrix& transform ) const 
{
}

void EveChildBehaviorSystem::ChangeLOD( Tr2Lod lod ) 
{
}

void EveChildBehaviorSystem::GetLights( Tr2LightManager& lightManager ) const 
{
}


