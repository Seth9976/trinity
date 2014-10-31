#include "StdAfx.h"
#include "EveDistanceField.h"

#include "EveCamera.h"
#include "EveUpdateContext.h"
#include "Curves/TriCurveSet.h"

EveDistanceField::EveDistanceField( IRoot* lockobj ) :
	PARENTLOCK( m_objects ),
	m_timeAdjustmentSecondsOut( 2.f ),
	m_timeAdjustmentSecondsIn( .25f ),
	m_distance( 1e6 )
{
}

void EveDistanceField::Update( const EveUpdateContext& updateContext )
{
	Vector3 posObj;

	Vector3 posRef( 0, 0, 0 );
	if( m_camera )
	{
		posRef = *m_camera->GetPosition();
	}
	float distanceNow = FLT_MAX;
	Be::Time t = updateContext.GetTime();

	for( auto oit = m_objects.begin(); oit != m_objects.end(); ++oit )
	{
		(*oit)->GetValueAt( &posObj, t );
		D3DXVec3Subtract( &posObj, &posObj, &posRef );
		distanceNow = min( distanceNow, D3DXVec3LengthSq( &posObj ) );
	}
	distanceNow = sqrt( distanceNow );

	double timeNow = TimeAsDouble( t );
	double delta = ( updateContext.GetDeltaT() ) / ( ( distanceNow > m_distance ) ? m_timeAdjustmentSecondsOut : m_timeAdjustmentSecondsIn );
	if( delta > 1 )
	{
		delta = 1;
	}

	m_distance = m_distance * ( 1 - delta ) + distanceNow * delta;
	if( m_curveSet )
	{
		if( !m_curveSet->IsPlaying() )
		{
			m_curveSet->Play();
			m_curveSet->Update( 0 );
		}
		m_curveSet->Update( m_distance );
	}
}