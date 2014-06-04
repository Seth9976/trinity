#include "StdAfx.h"
#include "EveAnimationSequencer.h"
#include "EveAnimationState.h"
#include "Eve/SpaceObject/EveSpaceObject2.h"

EveAnimationStateSequencer::EveAnimationStateSequencer( IRoot* lockobj ) :
	PARENTLOCK( m_pendingStates ),
	m_update(true)
{
}

EveAnimationStateSequencer::~EveAnimationStateSequencer() 
{
}

// --------------------------------------------------------------------------------
// Description:
//   Check if this state has a transition sequence for states identified by id.
// Arguments:
//   id - The id of the state we'd transition into
// Returns:
//   boolean indicating weather this state has a transition into the id state.
// --------------------------------------------------------------------------------
void EveAnimationStateSequencer::PushState( EveAnimationState* state )
{
	if( !state )
	{
		return;
	}

	if( !m_currentState )
	{
		m_currentState = state;
		m_currentState->TransitionFrom( nullptr, m_owner );
		return;
	}

	EveAnimationStateProgress progress = m_currentState->GetProgress();
	if( progress == EVE_ANIM_EXITING )
	{
		if( m_pendingStates.GetSize() )
		{
			m_pendingStates.Clear();
		}
		m_pendingStates.Append( state );
	}
	else if( progress == EVE_ANIM_TRANSITIONING )
	{
		if( m_pendingStates.empty() )
		{
			CCP_LOGERR("EveAnimationSequencer: Current state(%s) is transitioning(into state %s) but there are no pending states.", 
				m_currentState->GetName().c_str(), m_currentState->GetTranstionStateName().c_str());
			m_pendingStates.Append( state );
		}
		else if( m_pendingStates[0]->GetName() != state->GetName() )
		{
			if( m_pendingStates.GetSize() > 1 )
			{
				m_pendingStates.Remove( 1 );
			}
			m_pendingStates.Append( state );
		}
		else if( m_pendingStates.GetSize() > 1 )
		{
			m_pendingStates.Remove( 1 );
		}
	}
	else if( m_currentState->GetName() == state->GetName() ) 
	{
		m_pendingStates.Clear();
	}
	else
	{
		if( m_pendingStates.GetSize() )
		{
			m_pendingStates.Clear();
		}
		m_pendingStates.Append( state );
	}
}


void EveAnimationStateSequencer::Update( Be::Time time )
{
	if( !m_currentState || !m_update || !m_owner )
	{
		return;
	}

	EveAnimationStateProgress progress = m_currentState->GetProgress();
	// NB: This forces us to run one 'main' loop before we transition which may
	// not be want we ultimately want.
	if( progress == EVE_ANIM_RUNNING && m_pendingStates.GetSize() )
	{
		m_currentState->TransitionTo( m_pendingStates[0], m_owner );
	}
	m_currentState->Update( time, m_owner );

	if( m_currentState->GetProgress() == EVE_ANIM_DONE && m_pendingStates.GetSize() )
	{
		m_pendingStates[0]->TransitionFrom( m_currentState, m_owner );
		m_currentState = m_pendingStates[0];
		m_pendingStates.Remove( m_pendingStates.FindKey( m_currentState ) );
		m_currentState->Update( time, m_owner );
	}
}

void EveAnimationStateSequencer::Clear()
{
	m_currentState = nullptr;
	m_pendingStates.Clear();
}