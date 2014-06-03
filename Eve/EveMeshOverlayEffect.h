#pragma once
#ifndef EveMeshOverlayEffect_H
#define EveMeshOverlayEffect_H


#include "Tr2MeshArea.h"
#include "ITr2Renderable.h"

BLUE_DECLARE( Tr2Effect );
BLUE_DECLARE( TriCurveSet );
BLUE_DECLARE_VECTOR( Tr2Effect );

// --------------------------------------------------------------------------------
// Description:
//   This class holds curveSets and Tr2Effects used for overlay effects for SpaceObjects.
//   These effects render all meshes (mesh indices) in the parent's mesh.
// SeeAlso:
//   Tr2SpaceObject2
// --------------------------------------------------------------------------------
class EveMeshOverlayEffect:
	public IInitialize
{
public:
	EXPOSE_TO_BLUE();

	EveMeshOverlayEffect(IRoot* lockobj = NULL);
	~EveMeshOverlayEffect();

	bool m_display;
	bool m_update;
	std::string m_name;

	//////////////////////////////////////////////////////////////////////////////////////
	// IInitialize
	bool Initialize();
	
	const PTr2EffectVector& GetEffects( TriBatchType batchType, bool& success ) const;
	void Update( Be::Time realTime, Be::Time simTime );

	PTr2EffectVector m_opaqueEffects;
	PTr2EffectVector m_decalEffects;
	PTr2EffectVector m_transparentEffects;
	PTr2EffectVector m_additiveEffects;
	PTr2EffectVector m_distortionEffects;

private:
	TriCurveSetPtr m_curveSet;

};

TYPEDEF_BLUECLASS( EveMeshOverlayEffect );
BLUE_DECLARE_VECTOR( EveMeshOverlayEffect );

#endif // EveMeshOverlayEffect_H
