////////////////////////////////////////////////////////////
//
//    Created:   October 2015
//    Copyright: CCP 2015
//

#pragma once
#ifndef Tr2GpuUniqueEmitter_H
#define Tr2GpuUniqueEmitter_H

#include "Tr2GpuSharedEmitter.h"

// --------------------------------------------------------------------------------------
// Description:
//   Emitter class for GPU particle system. Unique emitters store persistent particle
//   parameters for each instance on GPU, so they need to be used with care. It is 
//   preferable to use shared emitters to conserve memory whenever possible. 
// See Also:
//   Tr2GpuParticleSystem, Tr2GpuUniqueEmitter
// --------------------------------------------------------------------------------------
BLUE_CLASS( Tr2GpuUniqueEmitter ): public Tr2GpuSharedEmitter
{
public:
	Tr2GpuUniqueEmitter( IRoot* lockObj = nullptr );

	EXPOSE_TO_BLUE();

	virtual void Update( const UpdateArguments& arguments );
protected:
	virtual void GenerateID();

	Vector3 m_attractorPosition;
};

TYPEDEF_BLUECLASS( Tr2GpuUniqueEmitter );

#endif