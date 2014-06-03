#include "StdAfx.h"
#include "TriStepPushRenderTarget.h"
#include "Tr2Renderer.h"


TriStepPushRenderTarget::TriStepPushRenderTarget( IRoot* lockobj )
{
}

TriStepResult TriStepPushRenderTarget::Execute( Be::Time realTime, Be::Time simTime, Tr2RenderContext& renderContext )
{
	if( m_renderTarget )
	{
		Tr2Renderer::PushRenderTarget( *m_renderTarget, renderContext );
	}
	else
	{
		Tr2Renderer::PushRenderTarget( renderContext );
	}
	return RS_OK;
}

void TriStepPushRenderTarget::py__init__( Tr2RenderTarget* rt )
{
	m_renderTarget = rt;
}
