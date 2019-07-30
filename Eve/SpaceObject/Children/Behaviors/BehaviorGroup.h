#pragma once

#include "Tr2DebugRenderer.h"
#include "Eve/Volume/IEveVolume.h"
#include <functional>
#include "TriFrustum.h"

struct DroneAgent
{
	DroneAgent() :
		mass( 1.f ),
		position( 0, 0, 0 ),
		rotation( 0, 0, 0, 1 ),
		acceleration( 0, 0, 0 ),
		velocity( 0, 0, 0 ),
		target( 0, 0, 0 ),
		lifetime( 0.f ),
		xfade( 0.f ),
		id( 0 ),
		isVisible( false ),
		screenSize( 0.f )
	{}

	float mass;
	Vector3 position;
	Quaternion rotation;
	Vector3 acceleration;
	Vector3 velocity;
	Vector3 target;
	float lifetime;
	float xfade; // Crossfade between mesh and sprite. 1.0 = mesh, 0.0 = sprite
	int id;
	bool isVisible; // Don't render agents off-screen
	float screenSize;
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

	void UpdateVisibility( const TriFrustum& frustum, const Matrix& parentTransform );

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
	void ProcessLOD( DroneAgent& agent );
	void SetBlendRange( float min, float max );
	unsigned int GetVertexDeclarationHandle() const;
	unsigned GetSpriteVertexDeclarationHandle() const;
	void SetVertexFunctionReferance( const std::function<void( void )>& F );
	void GetInfoForBuffer( uint8_t* data, Matrix& parentWorldLocation );
	void CreateSpriteVertexDeclaration();

	bool m_display;
	bool IsGroupVisible();

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

	// Crossfade blend range
	float m_currentScreenSize;  // READONLY attribute to show artist what the current agent screen size
	float m_renderThreshold;	// Do not render group if all agents have a screen size below this threshold.
	float m_blendScreenSizeMin; // If mesh screen size (in pixels) is smaller than this, it will be drawn as a sprite
	float m_blendScreenSizeMax; // If mesh screen size exceeds this, it will be drawn as mesh
	float m_xfadeValue;			// Normalized 0.0 - 1.0 from m_pixelSizeMin to m_pixelSizeMax;

	void UpdateCurrentScreenSize();


	// Bounding sphere
	Vector3 m_boundingSphereCenter;
	float m_boundingSphereRadius;
};

TYPEDEF_BLUECLASS( BehaviorGroup );

