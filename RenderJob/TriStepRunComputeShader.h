#pragma once
#ifndef TriStepRunComputeShader_h_
#define TriStepRunComputeShader_h_


#include "TriRenderStep.h"

BLUE_DECLARE_INTERFACE( ITr2ShaderMaterial );
BLUE_DECLARE_INTERFACE( ITr2GpuBuffer );

BLUE_CLASS( TriStepRunComputeShader ) : public TriRenderStep
{
public:
	EXPOSE_TO_BLUE();
	
	TriStepRunComputeShader( IRoot* lockobj = 0 );

	void py__init__( 
		ITr2ShaderMaterial* effect, 
		Be::OptionalWithDefaultValue<unsigned, 1> groupDimX,
		Be::OptionalWithDefaultValue<unsigned, 1> groupDimY,
		Be::OptionalWithDefaultValue<unsigned, 1> groupDimZ );
	
	//RenderStep
	TriStepResult Execute( Be::Time realTime, Be::Time simTime, Tr2RenderContext& renderContext );
	
	ITr2ShaderMaterialPtr m_effect;
	unsigned m_groupDimX;
	unsigned m_groupDimY;
	unsigned m_groupDimZ;

private:
	ITr2GpuBufferPtr m_indirectionBuffer;
	uint32_t m_offsetForArgs;

	bool m_logDispatchTime;	// if set, makes it synchronous
};

TYPEDEF_BLUECLASS( TriStepRunComputeShader );

#endif