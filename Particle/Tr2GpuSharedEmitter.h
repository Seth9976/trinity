////////////////////////////////////////////////////////////
//
//    Created:   October 2015
//    Copyright: CCP 2015
//

#pragma once
#ifndef Tr2GpuSharedEmitter_H
#define Tr2GpuSharedEmitter_H

#include "Tr2GpuParticleSystem.h"
#include "ITr2GenericEmitter.h"

// --------------------------------------------------------------------------------------
// Description:
//   Emitter class for GPU particle system. Shared emitters can share persistent particle
//   parameters between instances. It is preferable to use shared emitters (as opposed to 
//   unique emitters) to conserve memory whenever possible. Since shared emitter 
//   persistent parameters are shared they should not be animated. Also shared emitters
//   lack certain particle features (like attract forces).
// See Also:
//   Tr2GpuParticleSystem, Tr2GpuUniqueEmitter
// --------------------------------------------------------------------------------------
BLUE_CLASS( Tr2GpuSharedEmitter ): 
	public IInitialize,
	public INotify
{
public:
	Tr2GpuSharedEmitter( IRoot* lockObj = nullptr );

	EXPOSE_TO_BLUE();

	virtual bool Initialize();

	virtual bool OnModified( Be::Var* value );

	virtual void Update( Be::Time time, Tr2GpuParticleSystem& system, const Matrix& parentTransform, const Vector3& originShift );
protected:
	virtual void GenerateID();
	void UpdateHash();

	std::string m_name;
	Be::Time m_previousTime;
	uintptr_t m_id;
	uintptr_t m_paramsHash;
	float m_rate;
	float m_carryOver;
	float m_inheritVelocity;
	float m_emissionDensity;
	Vector3 m_position;
	Vector3 m_direction;
	Vector3 m_prevPosition;
	Vector3 m_prevVelocity;
	Vector3 m_prevDirection;
	Tr2GpuParticleSystem::Emitter m_emitter;
	Tr2GpuParticleSystem::EmitterParams m_params;
};

TYPEDEF_BLUECLASS( Tr2GpuSharedEmitter );

#endif