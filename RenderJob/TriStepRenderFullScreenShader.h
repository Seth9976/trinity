#pragma once
#ifndef _TRISTEPRENDERSHADER_H_
#define _TRISTEPRENDERSHADER_H_

#include "StdAfx.h"

#include "TriRenderStep.h"
#include "ITr2ShaderMaterial.h"

//BLUE_DECLARE( ITr2ShaderMaterialPtr );

BLUE_CLASS( TriStepRenderFullScreenShader ) : public TriRenderStep
{
public:
	EXPOSE_TO_BLUE();
	
	TriStepRenderFullScreenShader( IRoot* lockobj = 0 );
	~TriStepRenderFullScreenShader(void);

	//RenderStep
	TriStepResult Execute( Be::Time realTime, Be::Time simTime, Tr2RenderContext& renderContext );

	// Python __init__ constructor
	void py__init__( ITr2ShaderMaterial* shader );

private:
	ITr2ShaderMaterialPtr m_shader;
};

TYPEDEF_BLUECLASS( TriStepRenderFullScreenShader );

#endif