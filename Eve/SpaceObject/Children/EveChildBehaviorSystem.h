#pragma once
#ifndef EveChildBehaviorSystem_H
#define EveChildBehaviorSystem_H

#include "EveChildMesh.h"
#include "Include/ITr2InstanceData.h"
#include "Tr2DeviceResource.h"

BLUE_DECLARE( EveChildMesh ); //In future this will not be needed
BLUE_DECLARE( EveChildBehaviorSystem );
BLUE_DECLARE_INTERFACE( IBehavior );
BLUE_DECLARE_IVECTOR( IBehavior );

struct DroneAgent
{
	DroneAgent() :
		mass( 1.f ),
		position( 0, 0, 0 ),
		rotation( 0, 0, 0, 1 ),
		acceleration( 0, 0, 0 ),
		velocity( 0, 0, 0 ),
		target( 0, 0, 0 )
	{}

	float mass;
	Vector3 position;
	Quaternion rotation;
	Vector3 acceleration;
	Vector3 velocity;
	Vector3 target;
};

BLUE_CLASS( EveChildBehaviorSystem ) :
	public EveChildMesh, //In future it will not be needed
	public Tr2DeviceResource,
	public ITr2InstanceData
{
public:
	EXPOSE_TO_BLUE();

	EveChildBehaviorSystem( IRoot* lockobj = nullptr );
	~EveChildBehaviorSystem();

	/////////////////////////////////////////////////////////////////////////////////////
	// EveChildMesh
	void UpdateSyncronous( EveUpdateContext& updateContext, const EveChildUpdateParams& params );

	/////////////////////////////////////////////////////////////////////////////////////
	// EveChildBehaviorSystem
	void AddAgent();
	void SetCount( int count );

	/////////////////////////////////////////////////////////////////////////////////////
	// Tr2DeviceResource
	void ReleaseResources( TriStorage s ) {}
private:
	bool OnPrepareResources();
public:
	/////////////////////////////////////////////////////////////////////////////////////
	// ITr2InstanceData
	bool IsInstanceDataReady() const;
	unsigned int GetInstanceBufferCount() const;
	unsigned int GetInstanceBufferVertexDeclaration( unsigned int bufferIndex ) const;
	unsigned int GetInstanceBufferVertexCount( unsigned int bufferIndex ) const;
	void GetVertexBuffer( unsigned int bufferIndex, Tr2BufferAL& buffer, unsigned& stride );
	bool GetInstanceBufferBoundingBox( unsigned int bufferIndex, Vector3& minBounds, Vector3& maxBounds ) const;

private:
	/////////////////////////////////////////////////////////////////////////////////////
	// ITr2DebugRenderable
	virtual void GetDebugOptions( Tr2DebugRendererOptions& options );
	virtual void RenderDebugInfo( Tr2DebugRenderer& renderer );

	/////////////////////////////////////////////////////////////////////////////////////
	// EveChildBehaviorSystem
	void UpdateAgents( const float deltaTime );
	void UpdateBuffer( Tr2RenderContext& renderContext );
	std::vector<DroneAgent> m_agents;
	//Number of agents
	int m_count;

	//Steering behavior characteristics, this could actually go under the vehicle struct
	float m_maxVelocity;
	float m_maxForce;

	// Instance data as vertex buffer
	Tr2BufferAL m_vertexBuffer;
	// Vertex declaration handle for instance data
	uint32_t m_vertexDeclaration;
	//number of locations in memory between beginnings of successive array elements, measured in bytes or in units of the size of the array's elements.
	unsigned m_stride;
	// Number of instances
	unsigned m_vertexCount;

	//TODO: Instead of this class owning behaviors it should contain a BehaviorGroup which owns the behaviors. One BehaviorGroup for one "kind" of drone.
	//See modular traffic behavior for more info on behavior groups
	PIBehaviorVector m_behaviors;
};

TYPEDEF_BLUECLASS( EveChildBehaviorSystem );

#endif