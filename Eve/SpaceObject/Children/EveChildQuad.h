////////////////////////////////////////////////////////////
//
//    Created:   August 2016
//    Copyright: CCP 2016
//
#pragma once
#ifndef EveChildQuad_H
#define EveChildQuad_H

#include "IEveSpaceObjectChild.h"
#include "ITr2Renderable.h"

BLUE_DECLARE( Tr2Effect );

BLUE_CLASS( EveChildQuad ) :
	public IEveSpaceObjectChild,
	public ITr2Renderable,
	public IInitialize
{
public:
	EXPOSE_TO_BLUE();

	EveChildQuad( IRoot* lockobj = NULL );
	~EveChildQuad();

	/////////////////////////////////////////////////////////////////////////////////////
	// IEveSpaceObjectChild
	void GetRenderables( const TriFrustum& frustum, std::vector<ITr2Renderable*>& renderables, const Matrix& parentTransform, Tr2Lod parentLod );
	bool GetBoundingSphere( Vector4& sphere, BoundingSphereQuery query=EVE_BOUNDS_NORMAL ) const;
	void RegisterWithQuadRenderer( Tr2QuadRenderer& quadRenderer );
	void AddQuadsToQuadRenderer( const TriFrustum& frustum, Tr2QuadRenderer& quadRenderer ) const;
	void UpdateSyncronous( EveUpdateContext& updateContext, IEveSpaceObject2* spaceObjectParent, IEveSpaceObjectChild* childParent );
	void UpdateAsyncronous( EveUpdateContext& updateContext, IEveSpaceObject2* spaceObjectParent, IEveSpaceObjectChild* childParent );
	void GetLocalToWorldTransform( Matrix& transform ) const;
	void ChangeLOD( Tr2Lod lod ) {};
	void PlayCurveSet( const std::string& name ) {};
	void StopCurveSet( const std::string& name ) {};
	void GetLights( Tr2LightManager& lightManager ) const {};
	float GetCurveSetDuration( const std::string& name ) const { return 0; }
	void Setup( const Vector3* scale, const Quaternion* rotation, const Vector3* translation, Tr2Lod lowestLodVisible );
	
	/////////////////////////////////////////////////////////////////////////////////////
	// ITr2Renderable
	virtual bool HasTransparentBatches();
	virtual void GetBatches( ITriRenderBatchAccumulator* batches, TriBatchType batchType, const Tr2PerObjectData* perObjectData );
	virtual void GetShadowBatches( ITriRenderBatchAccumulator* batches, const Tr2PerObjectData* perObjectData );
	virtual float GetSortValue();
	virtual Tr2PerObjectData* GetPerObjectData( ITriRenderBatchAccumulator* accumulator );
	
	/////////////////////////////////////////////////////////////////////////////////////
	// IInitialize
	virtual bool Initialize();

private:
	struct Quad
	{
		Vector3 m_position;
		D3DXFLOAT16 m_color[4];
		Vector2 m_scale;
		float m_rotation;
		float m_brightness;
	};

	Matrix m_worldTransform;

	BlueSharedString m_name;
	Tr2EffectPtr m_effect;
	unsigned m_effectKey;
	Quad m_quad;

	Vector3 m_position;
	float m_rotation;
	Vector2 m_scale;
	Color m_color;
	float m_brightness;


	bool m_display;
	// Continiously re-register effect (for editing in Jessica)
	bool m_editMode;
};

TYPEDEF_BLUECLASS( EveChildQuad );

#endif