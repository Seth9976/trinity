#pragma once
#ifndef EveAnimationState_H
#define EveAnimationState_H

#include "BlueExposure/include/BlueTypes.h"
#include "EveAnimationData.h"

BLUE_DECLARE( EveSpaceObject2 );
BLUE_DECLARE( EveAnimationStateSequencer );

enum EveAnimationStateStartCommand {
	EVE_ANIM_START_DEFAULT,
	EVE_ANIM_START_INIT,
	EVE_ANIM_START_TRANSITION
};

struct EveAnimationStateTransition
{
	BlueSharedString name;
	BlueSharedString transitionName;
};
BLUE_DECLARE_STRUCTURE_LIST( EveAnimationStateTransition );

BLUE_CLASS( EveAnimationState ) :
	public IRoot
{
public:
	EXPOSE_TO_BLUE();
	
	EveAnimationState( IRoot* lockobj = NULL );
	~EveAnimationState();
	
	void Start( EveAnimationStateSequencerPtr owner, EveAnimationStateStartCommand mode=EVE_ANIM_START_DEFAULT );
	void Stop( EveSpaceObject2Ptr owner );
	void Update( Be::Time time, EveSpaceObject2Ptr owner );
	
	EveAnimationStateProgress GetProgress() const { return m_progress; }
	const std::string& GetName() const { return m_name; }
	const char* GetTransition( const std::string& stateName ) const;
private:
	std::string m_name;
	
	bool m_doInitialization;

	EveAnimationPtr m_animation;
	PEveAnimationCurveVector m_curves;
	PEveAnimationCommandVector m_commands;
	
	PEveAnimationCurveVector m_initCurves;
	PEveAnimationCommandVector m_initCommands;
	
	PEveAnimationStateTransitionStructureList m_transitions;

	EveAnimationStateProgress m_progress;

	float m_startTime;
	float m_animationDuration;
	float m_secondsRemaining;

	void PlayCurves( EveSpaceObject2Ptr owner );
	void ExecuteCommands( EveSpaceObject2Ptr owner );
	void PlayAnimation( EveAnimationStateSequencerPtr owner );
	void EndAnimation( EveSpaceObject2Ptr owner );
	void UpdateDuration( EveSpaceObject2Ptr owner );

	void Cleanup( EveSpaceObject2Ptr owner, Be::Time time );
};
TYPEDEF_BLUECLASS( EveAnimationState );
BLUE_DECLARE_VECTOR( EveAnimationState );

#endif