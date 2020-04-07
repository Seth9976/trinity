////////////////////////////////////////////////////////////
//
//    Created:   April 2020
//    Copyright: CCP 2020
//
//    Description:
// 	    Provides audio functionality to EveStretch effects, specifically
//      long running stretch effects (e.g. lasers).
// 	    All creating and positioning of audio emitters is handled by the class
// 		and not the sound designer as well as when audio events are fired. The
// 		only configuration given to the sound designer is which events to fire.

#pragma once
#ifndef Tr2AudioStretch_h_
#define Tr2AudioStretch_h_

#include "ITr2Audio.h"
#include "Tr2DebugRenderer.h"
#include "ITr2AudEmitter.h"

BLUE_CLASS( Tr2AudioStretch ) :
	public IInitialize,
	public ITr2Audio,
	public ITr2DebugRenderable
{
public:
	EXPOSE_TO_BLUE();

	// IInitialize
	bool Initialize() override;

	Tr2AudioStretch( IRoot* lockobj = NULL );
	virtual ~Tr2AudioStretch();

	// Places the source and destination audio emitters at the source and
	// destination positions of the stretch effect. Places the stretch
	// emitter between those two points relative to the camera.
	void Update( Vector3& sourcePosition, Vector3& destPosition );

	unsigned int TriggerOutburstEvent();
	unsigned int TriggerImpactEvent();
	unsigned int TriggerStretchEvent();

	// debug
	void GetDebugOptions( Tr2DebugRendererOptions& options );
	void RenderDebugInfo( ITr2DebugRenderer2& renderer );
protected:
	ITr2AudEmitterPtr m_sourceEmitter;
	ITr2AudEmitterPtr m_destEmitter;
	ITr2AudEmitterPtr m_stretchEmitter;

	std::wstring m_outburstEvent;
	std::wstring m_impactEvent;
	std::wstring m_stretchEvent;
};

TYPEDEF_BLUECLASS( Tr2AudioStretch );
#endif
