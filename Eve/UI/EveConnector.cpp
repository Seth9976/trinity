////////////////////////////////////////////////////////////
//
//    Created:   2016
//    Copyright: CCP 2016
//
#include "StdAfx.h"
#include "include/TriMath.h"
#include "EveConnector.h"
#include "EveCurveLineSet.h"

EveConnector::EveConnector( IRoot* lockobj ) :
	m_type( PointToPoint ),
	m_animationColor( 1.f, 0.f, 0.f, 1.f ),
	m_color( 0.5f, 0.5f, 0.5f, 1.f ),
	m_destPosition( 0.f, 0.f, 0.f ),
	m_sourcePosition( 0.f, 0.f, 0.f ),
	m_animationScale( 1.f ),
	m_animationSpeed( 0.f ),
	m_width( 1.f ),
	m_isAnimated( false )
{
}

EveConnector::~EveConnector()
{
}

void EveConnector::Update( EveUpdateContext& context )
{
	if( m_sourceObject )
	{
		m_sourceObject->GetValueAt( &m_sourcePosition, context.GetTime() );
	}
	
	if( m_destObject )
	{
		m_destObject->GetValueAt( &m_destPosition, context.GetTime() );
	}
}

void EveConnector::AddLine( EveCurveLineSet* lineSet )
{
	float length;
	Vector3 v;
	Vector3 n( 0, 1, 0 );

	// Currently we assume we always project onto the x,0,z plane. This may change later on, in that case using sourcePosition
	// as a point in the plane and for the relative position
	switch( m_type )
	{
	case StraightAnchor:
		TriVectorProjectOnPlane( &v, &m_destPosition, &m_sourcePosition, &n );
		AddStraightLine( lineSet, m_destPosition, v );
		break;
	case CurvedAnchor:
		TriVectorRotateToPlane( &v, &m_destPosition, &m_sourcePosition, &n );
		AddSpheredSegment( lineSet, m_destPosition, v, m_sourcePosition );
		break;
	case XZ_Circle:
		v = m_destPosition - m_sourcePosition;
		length = D3DXVec3Length( &v );
		AddCircle( lineSet, m_sourcePosition, length );
		break;
	case XZ_CircleStraight:
		TriVectorProjectOnPlane( &v, &m_destPosition, &m_sourcePosition, &n );
		v = v - m_sourcePosition;
		length = D3DXVec3Length( &v );
		AddCircle( lineSet, m_sourcePosition, length );
		break;
	case PointToPoint:
		AddStraightLine( lineSet, m_sourcePosition, m_destPosition );
		break;
	default:
		break;
	};
}

inline void EveConnector::AnimateSegment( EveCurveLineSet* lineSet, int lineID )
{
	if( m_isAnimated )
	{
		// Scale the animation by length so we get a more consistent size/speed
		// For it to look good we will need to do some falloff scaling in the shader as well
		// This doesn't quite work for circles at the moment, will have to figure out a
		// good consistent way to do this
		Vector3 d = m_sourcePosition - m_destPosition;
		float length = D3DXVec3Length( &d );
		lineSet->ChangeLineAnimation( lineID, (Vector4)m_animationColor, m_animationSpeed / length, length / m_animationScale );
	}
}

inline void EveConnector::AddCircle( EveCurveLineSet* lineSet, const Vector3& center, float radius )
{
	AddSpheredSegment( lineSet, center + Vector3( 0, 0, radius ), center + Vector3( radius, 0, 0 ), center );
	AddSpheredSegment( lineSet, center + Vector3( radius, 0, 0 ), center + Vector3( 0, 0, -radius ), center );
	AddSpheredSegment( lineSet, center + Vector3( 0, 0, -radius ), center + Vector3( -radius, 0, 0 ), center );
	AddSpheredSegment( lineSet, center + Vector3( -radius, 0, 0 ), center + Vector3( 0, 0, radius ), center );
}

inline void EveConnector::AddStraightLine( EveCurveLineSet* lineSet, const Vector3& source, const Vector3& destination )
{
	int id = lineSet->AddStraightLine( source, (Vector4)m_color, destination, (Vector4)m_color, m_width );
	AnimateSegment( lineSet, id );

}

inline void EveConnector::AddSpheredSegment( EveCurveLineSet* lineSet, const Vector3& p0, const Vector3& p1, const Vector3& center )
{
	int id = lineSet->AddSpheredLineCrt( p0, (Vector4)m_color, p1, (Vector4)m_color, center, m_width );
	AnimateSegment( lineSet, id );
}