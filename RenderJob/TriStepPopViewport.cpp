#include "StdAfx.h"
#include "TriStepPopViewport.h"
#include "Tr2Renderer.h"

TriStepResult TriStepPopViewport::Execute( Be::Time realTime, Be::Time simTime, Tr2RenderContext& renderContext )
{
	Tr2Renderer::PopViewport();
	return RS_OK;
}
