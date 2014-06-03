#include "StdAfx.h"
#include "TriCurveSet.h"
#include "TriValueBinding.h"
#include "include/ITriDuration.h"
#include "include/ITriFunction.h"
#include "include/ITriCurveLength.h"

TriCurveSet::TriCurveSet( IRoot* lockobj ) :
	PARENTLOCK( m_curves ),
	PARENTLOCK( m_bindings ),
	m_isPlaying( false ),
	m_stopOnNextFrame( false ),
	m_playOnLoad( true ),
	m_useSimTimeRebase( false ),
	m_isUsingSimTimeRebase( false ),
	m_startTime( 0.0 ),
	m_lastTime( 0.0 ),
	m_endTime( 0.0 ),
	m_scaledTime( 0.0 ),
	m_scale( 1.0f )
{
}

TriCurveSet::~TriCurveSet()
{
	if( m_isUsingSimTimeRebase )
	{
		BeOS->UnregisterForSimTimeRebase( this );
	}
}

void TriCurveSet::Update(  Be::Time realTime, Be::Time simTime  )
{
	Update(TimeAsDouble( simTime ));
}


void TriCurveSet::OnSimClockRebase( Be::Time oldTime, Be::Time newTime )
{
	double diff = TimeAsDouble( newTime - oldTime );
	m_startTime += diff;
	if( m_endTime > 0.0 )
	{
		m_endTime += diff;
	}
}


void TriCurveSet::Update( double time )
{
	CCP_STATS_ZONE( __FUNCTION__ );

	if( m_isPlaying )
	{
		double now = time - m_startTime;
		double delta = now - m_lastTime;
		m_lastTime = now;

		now = m_scaledTime + (double)m_scale * delta;
		m_scaledTime = now;

		if( (m_endTime > 0.0) && ((m_startTime + m_scaledTime) >= m_endTime) )
		{
			m_scaledTime = m_endTime - m_startTime - 0.001f;
			m_stopOnNextFrame = true;
		}

		for( ITriFunctionVector::const_iterator it = m_curves.begin(); it != m_curves.end(); ++it )
		{
			(*it)->UpdateValue( m_scaledTime );
		}

		for( ITr2ValueBindingVector::const_iterator it = m_bindings.begin(); it != m_bindings.end(); ++it )
		{
			(*it)->CopyValue();
		}
	}

	if( m_stopOnNextFrame )
	{
		if( m_callback )
		{
			m_callback.CallVoid();
			m_callback.Destroy();
		}
		m_isPlaying = false;
		m_stopOnNextFrame = false;
	}
}

bool TriCurveSet::Initialize()
{
	if( m_playOnLoad )
	{
		Play();
	}

	if( m_useSimTimeRebase )
	{
		BeOS->RegisterForSimTimeRebase( this );
		m_isUsingSimTimeRebase = true;
	}
	return true;
}

void TriCurveSet::Play()
{
	PlayFrom( 0.0 );
}

void TriCurveSet::PlayFrom( double time )
{
	m_startTime = TimeAsDouble( BeOS->GetCurrentFrameTime() );
	m_endTime = 0.0;
	m_isPlaying = true;
	m_lastTime = 0.0;
	m_scaledTime = time;

	m_callback.Destroy();
}

void TriCurveSet::Stop()
{
	m_isPlaying = false;
}

void TriCurveSet::StopOnNextFrame()
{
	m_stopOnNextFrame = true;
}

void TriCurveSet::StopAfter( double seconds )
{
	m_endTime = TimeAsDouble( BeOS->GetCurrentFrameTime() ) + seconds;
}

void TriCurveSet::StopAfterWithCallback( double seconds, const BlueScriptCallback& cb )
{
	StopAfter( seconds );
	m_callback = cb;
}

float TriCurveSet::GetTimeScale() const
{
	return m_scale;
}

float TriCurveSet::GetMaxCurveDuration() const
{
	float maxDuration = 0.0f;

	for( ITriFunctionVector::const_iterator it = m_curves.begin(); it != m_curves.end(); ++it )
	{
		float l = 0.0f;
		if( ITriDurationPtr f = BlueCastPtr( *it ) )
		{
			l = f->Length();
		}
		else if( ITriCurveLengthPtr f = BlueCastPtr( *it ) )
		{
			l = f->Length();
		}

		if( l > maxDuration )
		{
			maxDuration = l;
		}
	}

	return maxDuration;
}

bool TriCurveSet::IsPlaying()
{
	return m_isPlaying;
}

void TriCurveSet::UpdateWithCurrentTime()
{
	Update( double(BeOS->GetCurrentFrameTime()) );
}