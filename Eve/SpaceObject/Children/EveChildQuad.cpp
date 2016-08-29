////////////////////////////////////////////////////////////
//
//    Created:   August 2016
//    Copyright: CCP 2016
//
#include "StdAfx.h"
#include "EveChildQuad.h"

#include "Tr2QuadRenderer.h"
#include "Shader/Tr2Effect.h"

#include "Tr2Renderer.h"
#include "Utilities/BoundingSphere.h"
#include "TriFrustum.h"

namespace
{
	Tr2VertexDefinition& GetQuadDefinition()
	{
		static Tr2VertexDefinition def;
		if( def.empty() )
		{
			def.Add( def.FLOAT32_1, def.TEXCOORD, 5 );

			def.Add( def.FLOAT32_3, def.POSITION, 0, 1, 1 );
			def.Add( def.FLOAT16_4, def.TEXCOORD, 0, 1, 1 );
			def.Add( def.FLOAT32_4, def.TEXCOORD, 1, 1, 1 );
		}
		return def;
	}
}

EveChildQuad::EveChildQuad( IRoot* lockobj ):
	m_worldTransform( Tr2Renderer::GetIdentityTransform() ),
	m_effectKey( 0 ),
	m_position( 0.f, 0.f, 0.f ),
	m_rotation( 0.f ),
	m_scale( 1.f, 1.f ),
	m_color( 1.f, 1.f, 1.f, 1.f ),
	m_brightness( 1.f ),
	m_display( true ),
	m_editMode( false )
{
	memset( &m_quad, 0, sizeof( m_quad ) );
}

EveChildQuad::~EveChildQuad()
{
}

bool EveChildQuad::Initialize()
{
	if( m_effect )
	{
		m_effectKey = m_effect->GetHashValue();
	}
	return true;
}

// --------------------------------------------------------------------------------
// Description:
//   Setup function to set data from outside, in this case just pass it to
//   function of base class
// --------------------------------------------------------------------------------
void EveChildQuad::Setup( const Vector3* scale, const Quaternion* rotation, const Vector3* translation, Tr2Lod lowestLodVisible )
{
}

void EveChildQuad::RegisterWithQuadRenderer( Tr2QuadRenderer& quadRenderer )
{
	if( m_effect )
	{
		quadRenderer.RegisterEffect( m_effectKey, sizeof( Quad ), 1, GetQuadDefinition(), m_effect );
	}
}

void EveChildQuad::AddQuadsToQuadRenderer( const TriFrustum& frustum, Tr2QuadRenderer& quadRenderer ) const
{
	if( m_display && m_effect )
	{
		Vector4 sphere = Vector4( 0.f, 0.f, 0.f, std::sqrt( 2.f ) * std::max( m_scale.x, m_scale.y ) );
		BoundingSphereTransform( m_worldTransform, sphere );
		if( frustum.IsSphereVisible( &sphere ) )
		{
			quadRenderer.AddQuads( m_effectKey, &m_quad, 1 );
		}
	}
}

void EveChildQuad::GetRenderables( const TriFrustum& frustum, std::vector<ITr2Renderable*>& renderables, const Matrix& parentTransform, Tr2Lod parentLod )
{
}

bool EveChildQuad::GetBoundingSphere( Vector4& sphere, BoundingSphereQuery query ) const
{
	sphere = Vector4( 0.f, 0.f, 0.f, std::sqrt( 2.f ) * std::max( m_scale.x, m_scale.y ) );
	BoundingSphereTransform( m_worldTransform, sphere );
	return true;
}

bool EveChildQuad::HasTransparentBatches()
{
	return false;
}

void EveChildQuad::GetBatches( ITriRenderBatchAccumulator* batches, TriBatchType batchType, const Tr2PerObjectData* perObjectData )
{
}

void EveChildQuad::GetShadowBatches( ITriRenderBatchAccumulator* batches, const Tr2PerObjectData* perObjectData )
{
}

float EveChildQuad::GetSortValue()
{
	Vector3 d = Tr2Renderer::GetViewPosition() - m_worldTransform.GetTranslation();
	float distance = D3DXVec3Length( &d );
	return distance;
}


void EveChildQuad::UpdateSyncronous( EveUpdateContext& updateContext, IEveSpaceObject2* spaceObjectParent, IEveSpaceObjectChild* childParent )
{
	if( m_editMode )
	{
		auto key = m_effect->GetHashValue();
		if( key != m_effectKey )
		{
			m_effectKey = key;
			RegisterWithQuadRenderer( *Tr2QuadRenderer::Instance() );
		}
	}
}

void EveChildQuad::UpdateAsyncronous( EveUpdateContext& updateContext, IEveSpaceObject2* spaceObjectParent, IEveSpaceObjectChild* childParent )
{
	Matrix parentTransform;
	if( spaceObjectParent )
	{
		spaceObjectParent->GetLocalToWorldTransform( parentTransform );
	}
	else if ( childParent )
	{
		childParent->GetLocalToWorldTransform( parentTransform );
	}
	else
	{
		return;
	}
	m_worldTransform = parentTransform;
	float scale = D3DXVec3Length( &parentTransform.GetX() ) / 3.f + D3DXVec3Length( &parentTransform.GetY() ) / 3.f + D3DXVec3Length( &parentTransform.GetY() ) / 3.f;

	m_quad.m_position = XMVector3TransformCoord( m_position, m_worldTransform );
	m_quad.m_rotation = m_rotation;
	m_quad.m_scale.x = m_scale.x * scale;
	m_quad.m_scale.y = m_scale.y * scale;
	m_quad.m_color[0] = m_color.r;
	m_quad.m_color[1] = m_color.g;
	m_quad.m_color[2] = m_color.b;
	m_quad.m_color[3] = m_color.a;
	m_quad.m_brightness = m_brightness;
}

void EveChildQuad::GetLocalToWorldTransform( Matrix& transform ) const
{
	transform = m_worldTransform;
}

Tr2PerObjectData* EveChildQuad::GetPerObjectData( ITriRenderBatchAccumulator* accumulator )
{
	return nullptr;
}