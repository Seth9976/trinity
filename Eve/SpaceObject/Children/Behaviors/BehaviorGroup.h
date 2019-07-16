#pragma once

#include "Tr2DebugRenderer.h"
#include "Eve/Volume/IEveVolume.h"
#include <functional>

struct DroneAgent
{
	DroneAgent() :
		mass(1.f),
		position(0, 0, 0),
		rotation(0, 0, 0, 1),
		acceleration(0, 0, 0),
		velocity(0, 0, 0),
		target(0, 0, 0),
		lifetime( 0.f ),
		id( 0 )
	{}

	float mass;
	Vector3 position;
	Quaternion rotation;
	Vector3 acceleration;
	Vector3 velocity;
	Vector3 target;
	float lifetime;
	int id;
};

struct ITr2Renderable;

BLUE_DECLARE( BehaviorGroup );
BLUE_DECLARE( Tr2Mesh );
BLUE_DECLARE( TriGeometryRes );
BLUE_DECLARE_INTERFACE( IBehavior );
BLUE_DECLARE_IVECTOR( IBehavior );
BLUE_DECLARE_INTERFACE( IEveVolume );
BLUE_DECLARE_IVECTOR( IEveVolume );


BLUE_CLASS( BehaviorGroup ) :
	public IRoot
{
public:
	EXPOSE_TO_BLUE();
	BehaviorGroup( IRoot* lockobj = nullptr );
	~BehaviorGroup();

	/////////////////////////////////////////////////////////////////////////////////////
	// ITr2DebugRenderable
	virtual void GetDebugOptions( Tr2DebugRendererOptions& options );
	virtual void RenderDebugInfo( Tr2DebugRenderer& renderer, Matrix& parentWorldLocation );
	
	// geom res
	void InitializeGeometryResource();
	Tr2MeshPtr GetMesh();

	void AddAgent();
	size_t GetSize() const;
	void RemoveAgent();
	void SetCount( int count );
	int GetGroupIndexIndicator();
	void CreateVertexDeclaration();
	void ReleaseCachedData( BlueAsyncRes* );
	void RebuildCachedData( BlueAsyncRes* );
	void SetGroupIndexIndicator( int index );
	void UpdateAgents( const float deltaTime );
	unsigned int GetVertexDeclarationHandle() const;
	void SetVertexFunctionReferance( const std::function<void( void )>& F );
	void GetInfoForBuffer( uint8_t* data, Matrix& parentWorldLocation );
	
	bool m_display;
	float m_estimatedPixelDiameter;
	
private:
	void AddAgentPrivate();
	Vector3 RemoveAgentPrivate();

	int m_count;
	Vector3 m_scale;
	int m_groupIndex;
	Tr2MeshPtr m_mesh;
	unsigned int m_cachedVD;
	BlueSharedString m_name;
	PIEveVolumeVector m_volumes;
	PIBehaviorVector m_behaviors;
	std::vector<DroneAgent> m_agents;
	PIEveVolumeVector m_exclusionVolumes;
	unsigned int m_vertexDeclarationHandle;
	std::function<void()> m_changeBufferVertexCount;
	
	//Steering behavior characteristics, this could actually go under the vehicle struct
	float m_maxVelocity;
};

TYPEDEF_BLUECLASS( BehaviorGroup );

