#pragma once
#ifndef IBehavior_h
#define IBehavior_h

BLUE_INTERFACE( IBehavior ): public IRoot
{
public:
	virtual void CalculateBehavior( DroneAgent& agent, const float deltaTime, BehaviorGroup& sys ) = 0;
	virtual void RenderDebugInfo( Tr2DebugRenderer& renderer, Vector3 agentPos ) = 0;
};

#endif
