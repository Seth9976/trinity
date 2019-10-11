////////////////////////////////////////////////////////////
//
//    Created:   June 2019
//    Copyright: CCP 2019
//

#include "StdAfx.h"
#include "Tr2CurveVector3Lerp.h"
#include "Tr2CurveScalar.h"

Tr2CurveVector3Lerp::Tr2CurveVector3Lerp( IRoot* lockobj ):
	m_curveStartTime( 1.0 ),
	m_initialValue( 0, 0, 0 ),
	m_curve( nullptr )
{
	m_currentValue = m_initialValue;
}

void Tr2CurveVector3Lerp::UpdateValue( double time )
{
	m_currentValue = GetValue( time );
}


Vector3 Tr2CurveVector3Lerp::GetValue( double time ) const
{
	Vector3 v;
	if( m_curve == nullptr )
		return v;

	if( time < m_curveStartTime && m_curveStartTime > 0.0 )
	{
		Vector3 curveStartValue;
		m_curve->GetValueAt( &curveStartValue, 0.0 );
		v = Lerp( m_initialValue, curveStartValue, (float) time / m_curveStartTime );
	}
	else
	{
		m_curve->GetValueAt( &v, time - m_curveStartTime );
	}
	return v;
}

Vector3* Tr2CurveVector3Lerp::Update( Vector3* in, Be::Time time )
{
	m_currentValue = GetValue( TimeAsDouble( time ) );
	*in = m_currentValue;
	return in;
}

Vector3* Tr2CurveVector3Lerp::Update( Vector3* in, double time )
{
	m_currentValue = GetValue( time );
	*in = m_currentValue;
	return in;
}

Vector3* Tr2CurveVector3Lerp::GetValueAt( Vector3* in, Be::Time time )
{
	return GetValueAt( in, TimeAsDouble( time ) );
}

Vector3* Tr2CurveVector3Lerp::GetValueAt( Vector3* in, double time )
{
	*in = GetValue( time );
	return in;
}

Vector3* Tr2CurveVector3Lerp::GetValueDotAt( Vector3* in, Be::Time time )
{
	return in;
}

Vector3* Tr2CurveVector3Lerp::GetValueDotAt( Vector3* in, double time )
{
	return in;
}

Vector3* Tr2CurveVector3Lerp::GetValueDoubleDotAt( Vector3* in, Be::Time time )
{
	return in;
}

Vector3* Tr2CurveVector3Lerp::GetValueDoubleDotAt( Vector3* in, double time )
{
	return in;
}

Vector3d* Tr2CurveVector3Lerp::InterpolatedPosition( Vector3d* out, Be::Time time )
{
	return out;
}