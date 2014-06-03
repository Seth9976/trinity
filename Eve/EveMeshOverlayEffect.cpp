////////////////////////////////////////////////////////////
//
//    Created:   January 2012
//    Copyright: CCP 2012
//

#include "StdAfx.h"
#include "EveMeshOverlayEffect.h"
#include "Tr2Effect.h"
#include "Curves/TriCurveSet.h"


// --------------------------------------------------------------------------------------
// Description:
//   EveMeshOverlayEffect destructor
// --------------------------------------------------------------------------------------
EveMeshOverlayEffect::~EveMeshOverlayEffect()
{
}

// --------------------------------------------------------------------------------------
// Description:
//   EveMeshOverlayEffect constructor
// --------------------------------------------------------------------------------------
EveMeshOverlayEffect::EveMeshOverlayEffect( IRoot* lockobj ):
	m_display( true ),
	m_update( true ),
	PARENTLOCK( m_opaqueEffects ),
	PARENTLOCK( m_decalEffects ),
	PARENTLOCK( m_transparentEffects ),
	PARENTLOCK( m_additiveEffects ),
	PARENTLOCK( m_distortionEffects )
{
}

// --------------------------------------------------------------------------------------
// Description:
//   IInitialize
// --------------------------------------------------------------------------------------
bool EveMeshOverlayEffect::Initialize()
{
	return true;
}

// --------------------------------------------------------------------------------------
// Description:
//   GetEffect. 
// Return Value:
//   A Tr2EffectVector of effects.
// --------------------------------------------------------------------------------------
const PTr2EffectVector& EveMeshOverlayEffect::GetEffects(TriBatchType batchType, bool& success) const
{
	if ( m_display )
	{
		if ( batchType == TRIBATCHTYPE_OPAQUE )
		{
			success = true;
			return m_opaqueEffects;
		}
		else if ( batchType == TRIBATCHTYPE_DECAL )
		{
			success = true;
			return m_decalEffects;
		}
		else if ( batchType == TRIBATCHTYPE_TRANSPARENT )
		{
			success = true;
			return m_transparentEffects;
		}
		else if ( batchType == TRIBATCHTYPE_ADDITIVE )
		{
			success = true;
			return m_additiveEffects;
		}
		else if ( batchType == TRIBATCHTYPE_DISTORTION )
		{
			success = true;
			return m_distortionEffects;
		}
	}
	success = false;
	return m_opaqueEffects;
}


// --------------------------------------------------------------------------------------
// Description:
//   Update
// --------------------------------------------------------------------------------------
void EveMeshOverlayEffect::Update( Be::Time realTime, Be::Time simTime )
{
	if( !m_update || !m_curveSet )
	{
		return;
	}

	m_curveSet->Update( realTime, simTime );
}