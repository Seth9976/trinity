////////////////////////////////////////////////////////////
//
//    Created:   June 2019
//    Copyright: CCP 2019
//

#pragma once
#ifndef EveChildLineSet_H
#define EveChildLineSet_H

#include "IEveSpaceObjectChild.h"
#include "EveChildTransform.h"
#include "Tr2DebugRenderer.h"
#include "TriRenderBatch.h"
#include "Tr2Mesh.h"
#include "Eve/SpaceObject/EveSpaceObject2.h"
#include "Eve/UI/EveCurveLineSet.h"

class ChildLineSetInstancingBatch;

BLUE_CLASS(EveChildLineSet) :
	public IInitialize,
	public IEveSpaceObjectChild,
	public Tr2DeviceResource,
	public ITr2Renderable,
	public EveChildTransform,
	public INotify,
	public IListNotify,
	public ITr2DebugRenderable
{
public:
	EXPOSE_TO_BLUE();

	EveChildLineSet( IRoot* lockobj = NULL );
	~EveChildLineSet();

	//////////////////////////////////////////////////////////////////////////////////////
	// IInitialize
	bool Initialize() override;

	//////////////////////////////////////////////////////////////////////////////////////
	// IListNotify
	void OnListModified( long event, ssize_t key, ssize_t key2, IRoot* value, const IList* list ) override;

	//////////////////////////////////////////////////////////////////////////////////////
	// IListNotify
	bool OnModified( Be::Var* value ) override;

	//////////////////////////////////////////////////////////////////////////////////////
	// IEveSpaceObjectChild
	void SetShaderOption( const BlueSharedString& name, const BlueSharedString& value ) override;
	bool IsAlwaysOn() const override;
	void Setup( const Vector3* scale, const Quaternion* rotation, const Vector3* translation, Tr2Lod lowestLodVisible );
	
	const char* GetName() const;
	void SetName( const char* name );

	/////////////////////////////////////////////////////////////////////////////////////
	// Tr2DeviceResource
	void GetLights( Tr2LightManager& lightManager ) const {};
	void UpdateVisibility( const TriFrustum& frustum, const Matrix& parentTransform, Tr2Lod parentLod );
	void GetRenderables( std::vector<ITr2Renderable*>& renderables );
	bool GetBoundingSphere( Vector4& sphere, BoundingSphereQuery query=EVE_BOUNDS_NORMAL ) const;
	void GetLocalToWorldTransform( Matrix& transform ) const;
	void ReleaseResources( TriStorage s ) {};
	bool OnPrepareResources();
	bool HasTransparentBatches();
	float GetSortValue() { return 0.f; };
	void UpdateBuffer( Tr2RenderContext& renderContext );
	void CreateSpriteVertexDeclaration();
	Tr2MeshPtr GetMesh() const;

	void Draw(ChildLineSetInstancingBatch* batch, Tr2RenderContext& renderContext);
	
	//quad
	void RegisterWithQuadRenderer( Tr2QuadRenderer& quadRenderer ) {};
	void AddQuadsToQuadRenderer( const TriFrustum& frustum, Tr2QuadRenderer& quadRenderer ) const {};
	Tr2PerObjectData* GetPerObjectData( ITriRenderBatchAccumulator* accumulator );


	virtual void GetBatches( ITriRenderBatchAccumulator* batches, TriBatchType batchType, const Tr2PerObjectData* perObjectData );
	
	/////////////////////////////////////////////////////////////////////////////////////
	// update
	void UpdateSyncronous( EveUpdateContext& updateContext, const EveChildUpdateParams& params );
	void UpdateAsyncronous( EveUpdateContext& updateContext, const EveChildUpdateParams& params );
	void UpdateAsyncronous( EveUpdateContext& updateContext, Matrix& parentTransform );
	void ChangeLOD( Tr2Lod lod );

	
	/////////////////////////////////////////////////////////////////////////////////////
	// Debug
	void GetDebugOptions( Tr2DebugRendererOptions& options );
	void RenderDebugInfo( ITr2DebugRenderer2& renderer );

	void GetWorldVelocity( Vector3& velocity ) const;

	/////////////////////////////////////////////////////////////////////////////////////
	// EveChildLineSet
	
	float GetOwnerMaxSpeed() const;
	

	enum lineSetType { ObjectRender, LineRender, Both };
	enum lineSetObjType { Circle, BezierCurve };
	
private:

	void GenerateManagedPointsForCurve();
	void InitializeLineSet();
	void InitializeLineSetForCurves();
	void GenerateManagedPoints();

	
	BlueSharedString m_name;
	Vector3 m_worldVelocity;
	float m_ownerMaxSpeed;
	bool m_display;
	bool m_isAlwaysOn;

	// circle attributes
	float m_circleRadius;
	int m_numSegments;
	Vector3 m_circleCenter;

	Vector4 m_circleDistort;
	
	EveCurveLineSetPtr m_lineSet;
	lineSetType m_type;
	lineSetObjType m_objType;
	std::vector<Vector3> m_managedPoints;
	float m_completeness;

	//lines
	float m_lineWidth;
	Vector4 m_color1;
	Vector4 m_color2;

	//obj
	bool m_billboardObject;
	
	// Instance data as vertex buffer
	Tr2BufferAL m_vertexBuffer;
	//number of locations in memory between successive elements
	unsigned const m_stride;
	// Number of instances
	unsigned m_vertexCount;

	Tr2MeshPtr m_mesh;
	unsigned int m_vertexDeclarationHandle;
	unsigned int m_cachedSVD;
	EveSpaceObjectPSData m_psData;
	EveSpaceObjectVSData m_vsData;
	Vector3 m_objectScale;
	
	//animate the scene
	float m_animValue;
	float m_animSpeed;
	float m_scrollSpeed;
	float m_scrollSegmenting;

	// BezierCurve
	Vector3 m_point1;
	Vector3 m_point2;
	Vector3 m_bezierPoint;
	int m_curveSegments;
};

TYPEDEF_BLUECLASS( EveChildLineSet );

#endif