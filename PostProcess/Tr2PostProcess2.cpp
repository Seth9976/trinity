////////////////////////////////////////////////////////////////////////////////
//
// Created:		January 2019
// Copyright:	CCP 2019
//

#include "StdAfx.h"
#include "Tr2PostProcess2.h"
#include "Tr2Renderer.h"
#include "Tr2PostProcessRenderInfo.h"
#include "Effects/Tr2PPFidelityFXEffect.h"


Tr2PostProcess2::Tr2PostProcess2( IRoot* lockobj )
{
	
}


Tr2PostProcess2::~Tr2PostProcess2()
{
}


float Tr2PostProcess2::GetMipLodBias() const
{
	if( m_fidelityFX )
	{
		// If FSR is enabled, we need to provide a mip lod bias for certain textures (the ones that use it in shaders)
		return m_fidelityFX->GetFSRMipLodBias();
	}
	return 0.0f;
}
