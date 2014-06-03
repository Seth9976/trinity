#include "StdAfx.h"
#include "TriStepSetViewport.h"
#include "TriViewport.h"
#include "Tr2Renderer.h"

TriStepSetViewport::TriStepSetViewport( IRoot* lockobj )
{
}

TriStepSetViewport::~TriStepSetViewport(void)
{
}

void TriStepSetViewport::SetViewport( TriViewport* viewport )
{
	m_viewport = viewport;
}

TriStepResult TriStepSetViewport::Execute( Be::Time realTime, Be::Time simTime, Tr2RenderContext& renderContext )
{
	if( m_viewport )
	{
		Tr2Renderer::SetViewport( *m_viewport );
	}
	else
	{
		Tr2Renderer::SetFullScreenViewport();
	}
	return RS_OK;
}