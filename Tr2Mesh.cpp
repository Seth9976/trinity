#include "StdAfx.h"
#include "Tr2Mesh.h"
#include "Shader/Tr2Effect.h"
#include "Resources/TriGeometryRes.h"
#include "Resources/Tr2LodResource.h"

BLUE_DECLARE( Tr2Effect );

Tr2Mesh::Tr2Mesh( IRoot* lockobj ) : 
	PARENTLOCK( m_lodResources ),
	m_deferGeometryLoad( false ),
	m_isLoading( false ),
	m_resourceLoadCbId( 0 ),
	m_resourcePrepCbId( 0 ),
	m_selectedLod( TR2_LOD_UNSPECIFIED )
{
}

Tr2Mesh::~Tr2Mesh()
{
	if( m_geometryResource )
	{
		m_geometryResource->RemoveNotifyTarget( this );
	}

	if( m_resourceLoadCbId )
	{
		BeResMan->CancelFromQueue( BRMQ_BACKGROUND, m_resourceLoadCbId );
		m_resourceLoadCbId = 0;
	}

	if( m_resourcePrepCbId )
	{
		BeResMan->CancelFromQueue( BRMQ_MAIN, m_resourcePrepCbId );
		m_resourcePrepCbId = 0;
	}
}


// ---------------------------------------------------------------
bool Tr2Mesh::Initialize()
{
	if( !m_deferGeometryLoad )
	{
		InitializeGeometryResource();
	}

	return true;
}

void Tr2Mesh::StaticResourceLoadFinished( void* pContext )
{
	Tr2Mesh* pThis = static_cast<Tr2Mesh*>( pContext );
	BeResMan->AddToQueue( BRMQ_MAIN, StaticResourcePrepFinished, pContext, 0, &pThis->m_resourcePrepCbId );
	pThis->m_resourceLoadCbId = 0;
}

void Tr2Mesh::StaticResourcePrepFinished( void* pContext )
{
	Tr2Mesh* pThis = static_cast<Tr2Mesh*>( pContext );
	pThis->m_resourcePrepCbId = 0;
	pThis->m_isLoading = false;
}

// ---------------------------------------------------------------
bool Tr2Mesh::OnModified( Be::Var* value )
{
	if( IsMatch( value, m_meshResPath ) )
	{
		InitializeGeometryResource();
	}
	else if( IsMatch( value, m_deferGeometryLoad ) )
	{
		if( !m_deferGeometryLoad && !m_geometryResource )
		{
			Initialize();
		}
	}

	return true;
}

void Tr2Mesh::SetGeometryRes( TriGeometryRes* res )
{
	// Remove existing callback setup if any, set new geometry resource and attach callback
	if( m_geometryResource )
	{
		m_geometryResource->RemoveNotifyTarget( this );
	}

	m_geometryResource = res;

	if( m_geometryResource )
	{
		m_geometryResource->AddNotifyTarget( this );
	}
}

// --------------------------------------------------------------------------------------
// Description:
//   Set a new geometry path from the outside. This will trigger an initialize of
//   the new geometry resource!
// Arguments:
//   path - gr2 res path
// --------------------------------------------------------------------------------------
void Tr2Mesh::SetMeshResPath( const char* path )
{
	m_meshResPath = path;

	// trigger change, this will automatically be triggered when set through python
	OnModified( (Be::Var*)&m_meshResPath );
}

void Tr2Mesh::InitializeGeometryResource()
{
	TriGeometryResPtr res;

	if( !m_meshResPath.empty() )
	{
		BeResMan->GetResource( m_meshResPath.c_str(), m_geomResourceEx.c_str(), res );
		m_isLoading = true;

		if( m_resourceLoadCbId )
		{
			BeResMan->CancelFromQueue( BRMQ_BACKGROUND, m_resourceLoadCbId );
			m_resourceLoadCbId = 0;
		}
		if( m_resourcePrepCbId )
		{
			BeResMan->CancelFromQueue( BRMQ_MAIN, m_resourcePrepCbId );
			m_resourcePrepCbId = 0;
		}

		BeResMan->AddToQueue( BRMQ_BACKGROUND, StaticResourceLoadFinished, this, IBlueCallbackMan::BCBF_FENCE, &m_resourceLoadCbId );
	}

	SetGeometryRes( res );
}

void Tr2Mesh::RebuildCachedData( BlueAsyncRes* p )
{
	if( p == m_geometryResource )
	{
		m_areBoundsValid = true;

		if( !m_geometryResource->GetBoundingBox( m_meshIndex, m_minBounds, m_maxBounds ) )
		{
			m_minBounds = Vector3( 0.0f, 0.0f, 0.0f );
			m_maxBounds = Vector3( 0.0f, 0.0f, 0.0f );
		}

		// Todo: Geometry files should have this in them - do an offline process to calculate
		// proper bounding spheres. Until then, approximate with a sphere around the bounding box
		//if( !m_geometryResource->GetBoundingSphere( m_meshIndex, m_boundingSphereCenter, m_boundingSphereRadius ) )
		//{
		//	return;
		//}
		Vector3 d = m_maxBounds - m_minBounds;
		m_boundingSphere = Vector4( ( m_minBounds + m_maxBounds ) * 0.5f, Length( d ) * 0.5f );

		m_areBoundsValid = true;
	}
}

void Tr2Mesh::ReleaseCachedData( BlueAsyncRes* p )
{
}

void Tr2Mesh::PySetGeometryRes( TriGeometryRes* geometryRes )
{
	SetMeshResPath( "" );
	SetGeometryRes( geometryRes );
}

int Tr2Mesh::GetAreasCount() const
{
	return TRIBATCHTYPE_COUNT_OF_BATCH_TYPES;
}

TriGeometryRes* Tr2Mesh::GetGeometryResource() const
{
	return m_geometryResource;
}




