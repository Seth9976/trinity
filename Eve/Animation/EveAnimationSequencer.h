#pragma once
#ifndef EveAnimationSequencer_H
#define EveAnimationSequencer_H

#include "BlueExposure/include/BlueTypes.h"
#include "EveAnimationData.h"

BLUE_DECLARE( EveSpaceObject2 );
BLUE_DECLARE( EveAnimationState );
BLUE_DECLARE_VECTOR( EveAnimationState );

BLUE_CLASS( EveAnimationStateSequencer ) :
	public IRoot
{
public:
	EXPOSE_TO_BLUE();
	
	EveAnimationStateSequencer( IRoot* lockobj = NULL );
	~EveAnimationStateSequencer();

	void Clear();
	void Update( Be::Time time );
	void GoToState( const std::string& name );
	void Rebuild();

	const char* GetDefaultAnimation() const { return m_defaultAnimation.c_str(); }
	EveSpaceObject2Ptr GetOwner() const { return (EveSpaceObject2Ptr)m_owner; }
	void SetOwner( EveSpaceObject2Ptr owner );
private:
	std::map<std::string, EveAnimationStatePtr> m_stateMap;
	
	bool m_update;
	bool m_isTransitioning;
	bool m_useExtraAnimation;

	EveAnimationStatePtr m_currentState;
	PEveAnimationStateVector m_pendingStates;
	EveSpaceObject2Ptr m_owner;

	PEveAnimationStateVector m_states;
	BlueSharedString m_defaultAnimation;
	BlueSharedString m_extraAnimation;
	BlueSharedString m_extraAnimationTrackMask;
	
	EveAnimationStatePtr GetAnimationState( const std::string& name );
	bool CheckCompletionAndChangeStates();

};
TYPEDEF_BLUECLASS( EveAnimationStateSequencer );

#endif