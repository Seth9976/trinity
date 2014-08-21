#include "StdAfx.h"
#include "EveAnimationSequencer.h"
#include "EveAnimationState.h"
#include "Eve/SpaceObject/EveSpaceObject2.h"
#include "../../Tr2GrannyAnimation.h"


EveAnimationStateSequencer::EveAnimationStateSequencer( IRoot* lockobj ) :
	PARENTLOCK( m_pendingStates ),
	PARENTLOCK( m_states ),
	m_isTransitioning( false ),
	m_update( true )
{
}

EveAnimationStateSequencer::~EveAnimationStateSequencer() 
{
}

// --------------------------------------------------------------------------------
// Description:
//   Get the animation state with the name provided(or nullptr if it doesn't exist)
// --------------------------------------------------------------------------------
EveAnimationStatePtr EveAnimationStateSequencer::GetAnimationState( const std::string& name )
{
	for( auto it = m_states.begin(); it != m_states.end(); it++ )
	{
		if( (*it)->GetName() == name )
		{
			return *it;
		}
	}
	return nullptr;
}

// --------------------------------------------------------------------------------
// Description:
//   Set the owner. Triggers the extra animation.
// --------------------------------------------------------------------------------
void EveAnimationStateSequencer::SetOwner( EveSpaceObject2Ptr owner )
{
	m_owner = owner;
	if( !owner )
	{
		return;
	}
	if( !m_extraAnimation.empty() && !m_extraAnimationTrackMask.empty() )
	{
		auto ac = m_owner->GetAnimationController();
		if( ac->FindAnimationByName( m_extraAnimation.c_str() ) )
		{
			ac->AddAnimationLayerWithTrackMask( "extraLayer", m_extraAnimationTrackMask.c_str() );
			ac->PlayLayerAnimationByName( "extraLayer", m_extraAnimation.c_str(), 0, 0, 0, 1, false );
		}
	}
}

// --------------------------------------------------------------------------------
// Description:
//   Go to the state specified
// --------------------------------------------------------------------------------
void EveAnimationStateSequencer::GoToState( const std::string& name )
{
	if( !m_owner )
	{
		return;
	}

	EveAnimationStatePtr state = GetAnimationState( name );
	if( !state )
	{
		return;
	}

	if( !m_currentState || m_currentState->GetProgress() == EVE_ANIM_DONE )
	{
		m_currentState = state;
		state->Start( this, EVE_ANIM_START_INIT );
		return;
	}

	EveAnimationStateProgress progress = m_currentState->GetProgress();
	if( progress == EVE_ANIM_RUNNING )
	{
		if( state->GetName() != m_currentState->GetName() )
		{
			m_currentState->Stop( m_owner );
			m_pendingStates.Clear();
			m_pendingStates.Append( state );
		}
		else
		{
			m_pendingStates.Clear();
		}
	}
	else if( progress == EVE_ANIM_FINALIZING )
	{
		if( m_isTransitioning )
		{
			while( m_pendingStates.GetSize() > 1 )
			{
				m_pendingStates.Remove( 1 );
			}
			m_pendingStates.Append( state );
		}
		else
		{
			m_pendingStates.Clear();
			m_pendingStates.Append( state );
		}
	}
}

// --------------------------------------------------------------------------------
// Description:
//   Check if the current state is complete and swap for a new state if one is
//   available.
// --------------------------------------------------------------------------------
bool EveAnimationStateSequencer::CheckCompletionAndChangeStates()
{
	int pendingCount = m_pendingStates.GetSize();
	if( m_currentState->GetProgress() != EVE_ANIM_DONE || !pendingCount )
	{
		return false;
	}
	
	EveAnimationStatePtr lastState = m_pendingStates[pendingCount - 1];
	EveAnimationStatePtr nextState = lastState;
	const char* stateName;
	if( pendingCount == 1 )
	{
		if( !m_isTransitioning && (stateName=m_currentState->GetTransition( lastState->GetName() )) )
		{
			nextState = GetAnimationState( stateName );
			m_isTransitioning = true;
			nextState->Start( this, EVE_ANIM_START_TRANSITION );
		}
		else if( m_isTransitioning )
		{
			m_pendingStates.Clear();
			m_isTransitioning = false;
			nextState->Start( this );
		}
		else
		{
			m_pendingStates.Clear();
			m_isTransitioning = false;
			nextState->Start( this, EVE_ANIM_START_INIT );
		}
	}
	else if( m_isTransitioning )
	{
		if( stateName=m_pendingStates[0]->GetTransition( lastState->GetName() ) )
		{
			nextState = GetAnimationState( stateName );
			m_pendingStates.Clear();
			m_pendingStates.Append( lastState );
		}
		else
		{
			m_pendingStates.Clear();
		}
		nextState->Start( this );
	}
	//else{} only way get more than 1 pending is if we're transitioning, see GoToState
	m_currentState = nextState;

	return true;
}

void EveAnimationStateSequencer::Update( Be::Time time )
{
	if( !m_currentState || !m_update || !m_owner )
	{
		return;
	}

	do 
	{
		m_currentState->Update( time, m_owner );
	} while( CheckCompletionAndChangeStates() );
}

void EveAnimationStateSequencer::Clear()
{
	m_currentState = nullptr;
	m_pendingStates.Clear();
}
