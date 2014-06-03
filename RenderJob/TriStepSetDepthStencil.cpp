#include "StdAfx.h"
#include "TriStepSetDepthStencil.h"
#include "Tr2Renderer.h"

#include "Tr2DepthStencil.h"


TriStepSetDepthStencil::TriStepSetDepthStencil( IRoot* lockobj )
{
}

TriStepSetDepthStencil::~TriStepSetDepthStencil(void)
{
}

TriStepResult TriStepSetDepthStencil::Execute( Be::Time realTime, Be::Time simTime, Tr2RenderContext& renderContext )
{
	if( m_depthStencil )
	{
		return Tr2Renderer::SetDepthStencilBuffer( *m_depthStencil, renderContext ) ? RS_OK : RS_FAILED;
	}

	return Tr2Renderer::SetDepthStencilBuffer( nullDS, renderContext ) ? RS_OK : RS_FAILED;
}