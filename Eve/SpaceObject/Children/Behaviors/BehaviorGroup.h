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
		cameraDistance( 0 ),
		id( 0 )
	{}

	float mass;
	Vector3 position;
	Quaternion rotation;
	Vector3 acceleration;
	Vector3 velocity;
	Vector3 target;
	float lifetime;
	float cameraDistance;
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
	Tr2MeshPtr GetMesh() const;
	Tr2MeshPtr GetSpriteMesh() const;

	void AddAgent();
	size_t GetSize();
	unsigned int GetCount();
	void SetMeshToggle( bool toggle );
	void RemoveAgent();
	void SetCount( int count );
	float AllTheSame();
	int GetGroupIndexIndicator() const;
	void CreateVertexDeclaration();
	void ReleaseCachedData( BlueAsyncRes* );
	void RebuildCachedData( BlueAsyncRes* );
	void SetGroupIndexIndicator( int index );
	void UpdateAgents( const float deltaTime );
	void ProcessLOD(DroneAgent& agent);
	void SetBlendRange( float min, float max );
	unsigned int GetVertexDeclarationHandle() const;
	unsigned GetSpriteVertexDeclarationHandle() const;
	void SetVertexFunctionReferance( const std::function<void( void )>& F );
	void GetInfoForBuffer( uint8_t* data, Matrix& parentWorldLocation );
	void CreateSpriteVertexDeclaration();

	bool m_display;
	float m_estimatedPixelDiameter;
	
private:
	void ToggleMesh();
	void AddAgentPrivate();
	Vector3 RemoveAgentPrivate();

	int m_count;
	Vector3 m_scale;
	Vector3 m_spriteScale;
	int m_groupIndex;
	bool m_meshToggle;
	Tr2MeshPtr m_mesh;
	Tr2MeshPtr m_spriteMesh;
	unsigned int m_cachedVD;
	unsigned int m_cachedSVD;
	BlueSharedString m_name;
	PIEveVolumeVector m_volumes;
	PIBehaviorVector m_behaviors;
	std::vector<DroneAgent> m_agents;
	PIEveVolumeVector m_exclusionVolumes;
	unsigned int m_spriteVertexDeclarationHandle;
	unsigned int m_vertexDeclarationHandle;
	std::function<void()> m_changeBufferVertexCount;
	
	//Steering behavior characteristics, this could actually go under the vehicle struct
	float m_maxVelocity;

	// Blend range
	float m_blendRangeMax; // Effectively the distance threshold
	float m_blendRangeMin; // The distance where a drone should stop having a mesh and be fully rendered as a light.
	float m_blendRangeValue; // Normalized 0.0 - 1.0 from blendRangeMin to blendRangeMax;
};

TYPEDEF_BLUECLASS( BehaviorGroup );

