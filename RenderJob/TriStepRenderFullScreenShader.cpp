#include "StdAfx.h"
#include "TriStepRenderFullScreenShader.h"
#include "Tr2Renderer.h"
#include "Tr2EffectStateManager.h"

TriStepRenderFullScreenShader::TriStepRenderFullScreenShader( IRoot* lockobj )
{

}

TriStepRenderFullScreenShader::~TriStepRenderFullScreenShader(void)
{
}

void TriStepRenderFullScreenShader::py__init__( ITr2ShaderMaterial* shader )
{
	m_shader = shader;
}

TriStepResult TriStepRenderFullScreenShader::Execute( Be::Time realTime, Be::Time simTime, Tr2RenderContext& renderContext )
{

	if( m_shader )
	{
		Tr2Renderer::DrawFullScreenWithShader( m_shader );
	}
	return RS_OK;
}