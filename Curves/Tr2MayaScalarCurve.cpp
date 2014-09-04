#include "StdAfx.h"
#include "Tr2MayaScalarCurve.h"

#if BLUE_WITH_PYTHON

Tr2MayaScalarCurve::Tr2MayaScalarCurve( IRoot* lockobj ):
	m_index(-1),
	m_value(0.0f),
	m_length(0.0f),
	m_scale(1.0f)
{
}

bool Tr2MayaScalarCurve::Initialize(  )
{
	ComputeLength();
	return true;
}

// Accessors
void Tr2MayaScalarCurve::setIndex( int index )
{
	if( m_animationEngine && ( (int)m_animationEngine->getNumberOfCurves() > index ) && index > -2 )
	{
		if( index != m_index )
		{
			m_index = index;
			ComputeLength();
		}
	}
}

bool Tr2MayaScalarCurve::OnModified( Be::Var* value )
{
	if( IsMatch( value, m_animationEngine ) )
	{		
		ComputeLength();
	}

	return true;
}

void Tr2MayaScalarCurve::UpdateValue( double time )
{
	//CCP_STATS_ZONE( __FUNCTION__ );
	if( m_animationEngine )
	{	
		if( m_index > -1 )
		{
			m_value = m_animationEngine->evaluate( m_index, (float)time * m_scale );
		}	
	}
}

float Tr2MayaScalarCurve::Update( Be::Time time )
{
	return Update( TimeAsDouble( time ) );
}

float Tr2MayaScalarCurve::Update( double time )
{
	UpdateValue( time );
	return m_value;
}

float Tr2MayaScalarCurve::GetValueAt( Be::Time time )
{
	return GetValueAt( TimeAsDouble( time ) );
}

float Tr2MayaScalarCurve::GetValueAt( double time )
{
	if( m_animationEngine )
	{	
		if( m_index > -1 )
		{
			return m_animationEngine->evaluate( m_index, (float)time * m_scale );
		}	
	}
	return 0;
}

void Tr2MayaScalarCurve::ScaleTime( float s )
{
	m_scale = s;
}

void Tr2MayaScalarCurve::ComputeLength()
{
	if( m_animationEngine == NULL )
	{
		return;
	}

	if( m_animationEngine->getNumberOfCurves() == 0 )
	{
		return;
	}

	if( m_index > -1  )
	{
		m_length = m_animationEngine->getLength(m_index) * m_scale;
	}	
}

#endif
