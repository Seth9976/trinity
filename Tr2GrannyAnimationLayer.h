#pragma once
#ifndef Tr2GrannyAnimationLayer_h
#define Tr2GrannyAnimationLayer_h

#include "granny.h"
#include "ITr2AnimationUpdater.h"
#include "GrannyBoneOffset.h"
#include "blue/include/BlueAsyncRes.h"

class Tr2GrannyAnimation;


class Tr2GrannyAnimationLayer
{
public:
	Tr2GrannyAnimationLayer();
	Tr2GrannyAnimationLayer( float defaultBoneWeight );

	bool PlayAnimation( const Tr2GrannyAnimation* grannyAnimation, const char* animName, bool replace, int loopCount, float delay, float speed, bool clearWhenDone );
	void QueueAnimation( const char* animName, bool replace, int loopCount, float delay, float speed, bool clearWhenDone );
	void EndAnimation();
	void ClearAnimations();
	float GetAnimationChainCompleteTime();
	float GetAnimationRemainingTime();
	void InitializeAnimationLayer( const Tr2GrannyAnimation* grannyAnimation );
	void ConsumeAnimationQueue( const Tr2GrannyAnimation* grannyAnimation );
	void Cleanup();
	
	void SampleAnimation( float animationTime, granny_local_pose* resultPose );
	void SampleAnimation( float animationTime, granny_local_pose* compositePose, granny_local_pose* resultPose );
	void AddBone( const Tr2GrannyAnimation* grannyAnimation, const char* name );
	void RemoveBone( const Tr2GrannyAnimation* grannyAnimation, const char* name );
	void ExtractTrackMask( const Tr2GrannyAnimation* grannyAnimation, const char* name );
	
	std::string m_name;
	
	granny_model_instance* m_modelInstance;
private:
	struct AnimationRequest
	{
		std::string m_animationName;
		bool m_replace;
		bool m_clearWhenDone;
		int m_loopCount;
		float m_start;
		float m_speed;
	};
	std::vector<AnimationRequest> m_animationQueue;

	granny_track_mask* m_trackMask;
	std::vector<std::string> m_bones;
	granny_control* m_activeControl;

	int m_boneCount;
	float m_defaultBoneWeight;

	const char* m_trackMaskName;
};

#endif //Tr2GrannyAnimationLayer_h
