#include "StdAfx.h"
#include "EveAnimationData.h"

EveAnimation::EveAnimation( IRoot* lockobj ) :
	m_delay( 0.f ),
	m_loops( 0 ),
	m_speed( 1.f )
{}

EveAnimationCurve::EveAnimationCurve( IRoot* lockobj )
{}

EveAnimationCommand::EveAnimationCommand( IRoot* lockobj ) :
	m_command( ANIM_CMD_NONE )
{}

EveAnimationSequence::EveAnimationSequence( IRoot* lockobj ) :
	PARENTLOCK( m_curves ),
	PARENTLOCK( m_commands )
{}

	
EveTransitionSequence::EveTransitionSequence( IRoot* lockobj )
{}
