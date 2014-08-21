#include "StdAfx.h"
#include "EveAnimationState.h"
#include "EveAnimationSequencer.h"
#include "Eve/SpaceObject/EveSpaceObject2.h"
#include "Eve/SpaceObject/EveMobile.h"
#include "Eve/SpaceObject/EveShip2.h"
#include "Curves/TriCurveSet.h"
#include "Tr2GrannyAnimation.h"
#include "Tr2Renderer.h"

static BlueStructureDefinition EveAnimationStateTransitionStructureDef[] =
{ 
	{ "name", Be::SHAREDSTRING_1, 0 },
	{ "transitionName", Be::SHAREDSTRING_1, 8 }, 
	{0} 
};

	
// --------------------------------------------------------------------------------
// Description:
//   Initialize data members
// --------------------------------------------------------------------------------
EveAnimationState::EveAnimationState( IRoot* lockobj ) :
	PARENTLOCK( m_curves ),
	PARENTLOCK( m_commands ),
	PARENTLOCK( m_initCommands ),
	PARENTLOCK( m_initCurves ),
	PARENTLOCK( m_transitions ),
	m_progress( EVE_ANIM_INACTIVE ),
	m_animationDuration( 0.f ),
	m_startTime( 0.f ),
	m_secondsRemaining( 0.f ),
	m_doInitialization( false )
{
	m_transitions.SetStructureDefinition( EveAnimationStateTransitionStructureDef );
}

EveAnimationState::~EveAnimationState() 
{
}

// --------------------------------------------------------------------------------
// Description:
//   Update the time when animations/curves are completed for this state.
// --------------------------------------------------------------------------------
void EveAnimationState::UpdateDuration( EveSpaceObject2Ptr owner )
{
	float currentAnimationTime = Tr2Renderer::GetAnimationTime();
	m_startTime = currentAnimationTime;
	m_animationDuration = 0;
	auto ac = owner->GetAnimationController();
	if( ac && ac->IsInitialized() )
	{
		m_animationDuration = ac->GetAnimationChainCompleteTime() - currentAnimationTime;
	}

	float maxCurveDuration = 0.f;
	for( auto it = m_curves.cbegin(); it != m_curves.cend(); it++ )
	{
		maxCurveDuration = max( maxCurveDuration, owner->GetCurveSetDuration( (*it)->m_name ) );
	}

	m_animationDuration = max( m_animationDuration, maxCurveDuration );
}

// --------------------------------------------------------------------------------
// Description:
//   Deactivate this state. May take a while to wind down.
// --------------------------------------------------------------------------------
void EveAnimationState::Stop( EveSpaceObject2Ptr owner )
{
	m_progress = EVE_ANIM_FINALIZING;
	EndAnimation( owner );
}

// --------------------------------------------------------------------------------
// Description:
//   Start this state.
// Arguments:
//   owner - the owning animation sequencer
//   mode - is it a transition, does it need to run init curves etc.
// --------------------------------------------------------------------------------
void EveAnimationState::Start( EveAnimationStateSequencerPtr owner, EveAnimationStateStartCommand mode )
{
	m_doInitialization = mode == EVE_ANIM_START_INIT;

	m_startTime = Tr2Renderer::GetAnimationTime();
	m_animationDuration = 0.f;

	if( mode == EVE_ANIM_START_TRANSITION )
	{
		m_progress = EVE_ANIM_FINALIZING;
	}
	else
	{
		m_progress = EVE_ANIM_RUNNING;
	}

	PlayAnimation( owner );
	UpdateDuration( owner->GetOwner() );
}

// --------------------------------------------------------------------------------
// Description:
//   Check if this state has a transition sequence for states identified by stateName.
// Arguments:
//   stateName - The name of the state we'd transition into
// Returns:
//   name of transition state or nullptr
// --------------------------------------------------------------------------------
const char* EveAnimationState::GetTransition( const std::string& stateName ) const
{
	for( auto it = m_transitions.begin(); it != m_transitions.end(); it++ )
	{
		if( stateName == it->name.c_str() )
		{
			return it->transitionName.c_str();
		}
	}
	return nullptr;
}

// --------------------------------------------------------------------------------
// Description:
//   Play curves in the animation sequence
// --------------------------------------------------------------------------------
void EveAnimationState::PlayCurves( EveSpaceObject2Ptr owner )
{
	if( m_doInitialization )
	{	
		for( auto it = m_initCurves.cbegin(); it != m_initCurves.cend(); it++ )
		{
			owner->PlayCurveSet( (*it)->m_name );
		}
	}
	for( auto it = m_curves.cbegin(); it != m_curves.cend(); it++ )
	{
		owner->PlayCurveSet( (*it)->m_name );
	}
}

// --------------------------------------------------------------------------------
// Description:
//   Play commands in the animation sequence
// --------------------------------------------------------------------------------
void EveAnimationState::ExecuteCommands( EveSpaceObject2Ptr owner )
{
	if( m_doInitialization )
	{
		for( auto it = m_initCommands.cbegin(); it != m_initCommands.cend(); it++ )
		{
			owner->ExecuteAnimationStateCommand( (*it)->m_command, (*it)->m_data );
		}
	}
	
	for( auto it = m_commands.cbegin(); it != m_commands.cend(); it++ )
	{
		owner->ExecuteAnimationStateCommand( (*it)->m_command, (*it)->m_data );
	}
}

// --------------------------------------------------------------------------------
// Description:
//   Play animations, commands and curves in the animation sequence
// --------------------------------------------------------------------------------
void EveAnimationState::PlayAnimation( EveAnimationStateSequencerPtr owner )
{
	if( m_animation )
	{
		auto ac = owner->GetOwner()->GetAnimationController();
		bool success = ac->PlayAnimation( m_animation->m_name.c_str(), false, m_animation->m_loops, m_animation->m_delay, m_animation->m_speed, false );
		if( m_progress == EVE_ANIM_RUNNING && !success )
		{
			ac->PlayAnimation( owner->GetDefaultAnimation(), false, m_animation->m_loops, m_animation->m_delay, m_animation->m_speed, false );
		}
	}

	PlayCurves( owner->GetOwner() );
	ExecuteCommands( owner->GetOwner() );
}


// --------------------------------------------------------------------------------
// Description:
//   End current animation.
// --------------------------------------------------------------------------------
void EveAnimationState::EndAnimation( EveSpaceObject2Ptr owner )
{
	auto ac = owner->GetAnimationController();
	ac->EndAnimation();

	float currentAnimationTime = Tr2Renderer::GetAnimationTime();
	float animationDuration = 0;
	if( ac && ac->IsInitialized() )
	{
		m_animationDuration = ac->GetAnimationChainCompleteTime() - currentAnimationTime;
	}

	m_animationDuration = max( m_animationDuration, animationDuration );
}

// --------------------------------------------------------------------------------
// Description:
//   Update the state
// --------------------------------------------------------------------------------
void EveAnimationState::Update( Be::Time time, EveSpaceObject2Ptr owner )
{
	float animationDelta = Tr2Renderer::GetAnimationTimeElapsed( m_startTime );
	m_secondsRemaining = m_animationDuration - animationDelta;

	if( animationDelta < m_animationDuration )
	{
		return;
	}

	if( m_progress == EVE_ANIM_FINALIZING )
	{
		m_progress = EVE_ANIM_DONE;
		Cleanup( owner, time );
	}
}

// --------------------------------------------------------------------------------
// Description:
//   Stop animations/curves associated with this state.
// --------------------------------------------------------------------------------
void EveAnimationState::Cleanup( EveSpaceObject2Ptr owner, Be::Time time )
{
	for( auto it = m_curves.cbegin(); it != m_curves.cend(); it++ )
	{
		owner->UpdateCurveSet( (*it)->m_name, time );
		owner->StopCurveSet( (*it)->m_name );
	}
	if( m_doInitialization )
	{
		for( auto it = m_initCurves.cbegin(); it != m_initCurves.cend(); it++ )
		{
			owner->UpdateCurveSet( (*it)->m_name, time );
			owner->StopCurveSet( (*it)->m_name );
		}
	}

	m_doInitialization = false;
	m_startTime = 0.f;
	m_animationDuration = 0.f;
}


