#pragma once
#ifndef _TRISTEPRENDERCUBEMAP_H_
#define _TRISTEPRENDERCUBEMAP_H_


#include "TriRenderStep.h"
#include "Resources/TriTextureRes.h"
#include "include/Point.h"

BLUE_CLASS( TriStepRenderCubeMap ) : public TriRenderStep
{
public:
	EXPOSE_TO_BLUE();
	
	TriStepRenderCubeMap( IRoot* lockobj = 0 );
	~TriStepRenderCubeMap(void);

	//RenderStep
	TriStepResult Execute( Be::Time realTime, Be::Time simTime, Tr2RenderContext& renderContext );

	void py__init__( TriTextureRes* tex );

private:
	TriTextureResPtr m_texture;

	// positions & desc for a nicely displayed cubemap
	Point m_sidePos[6];
	std::string m_sideDesc[6];
};

TYPEDEF_BLUECLASS( TriStepRenderCubeMap );

#endif