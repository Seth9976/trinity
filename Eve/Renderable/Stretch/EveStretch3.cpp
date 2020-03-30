////////////////////////////////////////////////////////////
//
//    Created:   March 2020
//    Copyright: CCP 2020
//

#include "StdAfx.h"

#include "EveStretch3.h"
#include "TriFloat.h"
#include "Eve/EveUpdateContext.h"
#include "Include/TriMath.h"
#include "Utilities/BoundingSphere.h"
#include "Curves/TriCurveSet.h"

#include "Lights/Tr2PointLight.h"
#include "Controllers/ITr2Controller.h"
#include "Eve/SpaceObject/Children/TransformModifiers/EveChildModifierStretch.h"
#include "Eve/SpaceObject/Children/IEveSpaceObjectChild.h"

EveStretch3::EveStretch3( IRoot* lockobj ) :
	PARENTLOCK( m_curveSets ),
	PARENTLOCK( m_controllers ),
	m_display( true ),
	m_update( true ),
	m_sourcePosition( 0.0f, 0.0f, 0.0f ),
	m_destinationPosition( 0.0f, 0.0f, 0.0f ),
	m_startTime( 0 ),
	m_destObjectScale( 1.0f )
{
	m_length.CreateInstance();
	m_moveProgression.CreateInstance();

	m_controllers.SetNotify( this );
}

bool EveStretch3::Initialize()
{
	if( m_stretchObject )
	{
		m_stretchModifier.CreateInstance();
	}
	if( m_dest && m_stretchModifier )
	{
		m_stretchModifier->SetDest( m_dest );
	}

	for( auto it = begin( m_controllers ); it != end( m_controllers ); ++it )
	{
		( *it )->Link( *GetRawRoot() );
	}

	return true;
}

void EveStretch3::RunOnComponents( std::function<void( IEveSpaceObjectChild* )> func ) const 
{
	if( m_sourceObject )
	{
		func( m_sourceObject );
	}

	if( m_destObject )
	{
		func( m_destObject );
	}

	if( m_stretchObject )
	{
		func( m_stretchObject );
	}

	if( m_moveObject )
	{
		func( m_moveObject );
	}
}

float EveStretch3::RunOnComponentsGetMax( std::function<float( IEveSpaceObjectChild* )> func ) const
{
	float ret = -1.f;
	if( m_sourceObject )
	{
		ret = max( ret, func( m_sourceObject ) );
	}

	if( m_destObject )
	{
		ret = max( ret, func( m_destObject ) );
	}

	if( m_stretchObject )
	{
		ret = max( ret, func( m_stretchObject ) );
	}

	if( m_moveObject )
	{
		ret = max( ret, func( m_moveObject ) );
	}
	return ret;
}

bool EveStretch3::OnModified( Be::Var* value )
{
	if( IsMatch( value, m_stretchObject ) )
	{
		if( m_stretchObject )
		{	
			m_stretchModifier.CreateInstance();
			if( m_dest )
			{
				m_stretchModifier->SetDest( m_dest );
			}
		}
		else
		{
			m_stretchModifier = nullptr;
		}
	}
	else if( IsMatch( value, m_dest ) && m_stretchModifier )
	{
		m_stretchModifier->SetDest( m_dest );
	}
	return true;
}

void EveStretch3::OnListModified( long event, ssize_t key, ssize_t key2, IRoot* value, const IList* list )
{
	if( list == &m_controllers && ( event & BELIST_LOADING ) == 0 )
	{
		switch( event & BELIST_EVENTMASK )
		{
		case BELIST_INSERTED:
			if( ITr2ControllerPtr controller = BlueCastPtr( value ) )
			{
				controller->Link( *GetRawRoot() );
			}
			break;
		case BELIST_REMOVED:
			if( ITr2ControllerPtr controller = BlueCastPtr( value ) )
			{
				controller->Unlink();
			}
			break;
		default:
			break;
		}
	}
}


void EveStretch3::UpdateSyncronous( EveUpdateContext& updateContext )
{
	CCP_STATS_ZONE( __FUNCTION__ );

	if( !m_update )
	{
		return;
	}

	for( auto it = begin( m_controllers ); it != end( m_controllers ); ++it )
	{
		( *it )->Update();
	}
	
	Be::Time time = updateContext.GetTime();

	if( m_source )
	{
		m_source->Update( &m_sourcePosition, time );
	}

	if( m_dest )
	{
		m_dest->Update( &m_destinationPosition, time );
	}

	Vector3 directionVec( m_sourcePosition - m_destinationPosition );
	m_length->m_value = Length( directionVec );

	EveChildUpdateParams params;
	params.spaceObjectParent = m_sourceSpaceObject == nullptr ? this : m_sourceSpaceObject;
	params.childParent = nullptr;
	params.boneCount = 0;
	params.bones = nullptr;
	params.isVisible = m_display;

	if( m_sourceObject )
	{
		m_sourceObject->UpdateSyncronous( updateContext, params );
	}

	if( m_stretchObject )
	{
		m_stretchObject->UpdateSyncronous( updateContext, params );
	}

	if( m_moveObject )
	{
		params.localToWorldTransform = TranslationMatrix( directionVec * m_moveProgression->m_value );

		m_moveObject->UpdateSyncronous( updateContext, params );
	}

	// reset the space object parent
	params.spaceObjectParent = nullptr;

	if( m_destObject )
	{
		params.localToWorldTransform = TranslationMatrix( m_destinationPosition );
		params.spaceObjectParent = m_destSpaceObject == nullptr ? this : m_destSpaceObject;
		m_destObject->UpdateSyncronous( updateContext, params );
	}
}

void EveStretch3::UpdateAsyncronous( EveUpdateContext& updateContext )
{
	if( !m_update )
	{
		return;
	}

	Be::Time time = updateContext.GetTime();
	if( m_startTime == 0 )
	{
		m_startTime = time;
	}

	for( auto curve = m_curveSets.begin(); curve != m_curveSets.end(); ++curve )
	{
		( *curve )->Update( TimeAsDouble( time - m_startTime ) );
	}

	Vector3 directionVec( m_sourcePosition - m_destinationPosition );
	m_length->m_value = Length( directionVec );

	EveChildUpdateParams params;
	params.spaceObjectParent = nullptr;
	params.childParent = nullptr;
	params.boneCount = 0; 
	params.bones = nullptr;
	params.isVisible = m_display;

	if( m_sourceObject )
	{
		auto m = TranslationMatrix( m_sourcePosition );

		params.localToWorldTransform = m;
		m_sourceObject->UpdateAsyncronous( updateContext, params );
	}
	
	if( m_stretchObject )
	{
		auto m = TranslationMatrix( m_sourcePosition );
		m_stretchModifier->SetDestPosition( m_destinationPosition );
		m = m_stretchModifier->ApplyTransform( m, 0, nullptr );
		params.localToWorldTransform = m;
		m_stretchObject->UpdateAsyncronous( updateContext, params );
	}
	params.spaceObjectParent = nullptr;

	if( m_moveObject )
	{
		Quaternion rotation( 0.0f, 0.0f, 0.0f, 1.0f );
		TriQuaternionArcFromForward( &rotation, &directionVec );

		Vector3 movedPostition = Lerp( m_sourcePosition, m_destinationPosition, m_moveProgression->m_value );
		
		params.localToWorldTransform = TransformationMatrix( Vector3( 1, 1, 1 ), rotation, movedPostition );

		m_moveObject->UpdateAsyncronous( updateContext, params );
	}
	
	if( m_destObject )
	{
		params.localToWorldTransform = ScalingMatrix( Vector3(1, 1, 1) * m_destObjectScale ) * TranslationMatrix( m_destinationPosition );

		m_destObject->UpdateAsyncronous( updateContext, params );
	}
}

void EveStretch3::Update( EveUpdateContext& updateContext )
{
	UpdateSyncronous(updateContext);
	UpdateAsyncronous(updateContext);
}


void EveStretch3::StartMoving()
{ 
}

void EveStretch3::UpdateVisibility( const TriFrustum& frustum, const Matrix& parentTransform )
{
	if( !m_display )
	{
		return;
	}

	Vector3 directionVec = Normalize( m_sourcePosition - m_destinationPosition );
	float scalingLength = m_length->m_value;

	if( m_sourceObject )
	{
		m_sourceObject->UpdateVisibility( frustum, parentTransform, TR2_LOD_HIGH );
	}
	
	if( m_destObject )
	{
		m_destObject->UpdateVisibility( frustum, parentTransform, TR2_LOD_HIGH );
	}
	
	if( m_stretchObject )
	{
		// We need to figure out a better way of doing this, maybe we need to be able to pass in 
		// an inverse modifier to update this correctly
		// Currently this will almost always be visible, because of the stretch
		// If we can not make it stretched, then we could make it invisible sooner
		m_stretchObject->UpdateVisibility( frustum, parentTransform, TR2_LOD_HIGH );
	}

	if( m_moveObject )
	{
		m_moveObject->UpdateVisibility( frustum, parentTransform, TR2_LOD_HIGH );
	}
	
}

void EveStretch3::GetRenderables( std::vector<ITr2Renderable*>& renderables)
{
	GetRenderables( renderables, nullptr );
}

void EveStretch3::GetRenderables( std::vector<ITr2Renderable*>& renderables, Tr2ImpostorManager* impostors )
{
	if( !m_display )
	{
		return;
	}

	RunOnComponents( [&renderables]( IEveSpaceObjectChild* c ) { c->GetRenderables( renderables ); } );
}

void EveStretch3::SetDisplay( bool display )
{
	m_display = display;
}

bool EveStretch3::GetBoundingSphere( Vector4& sphere, BoundingSphereQuery query ) const
{
	bool valid = false;

	Vector4 v;
	if( m_destObject )
	{
		if( m_destObject->GetBoundingSphere( v, query ) )
		{
			sphere = v;
			valid = true;
		}
	}
	if( m_sourceObject )
	{
		if( m_sourceObject->GetBoundingSphere( v, query ) )
		{
			BoundingSphereSetOrUpdate( v, sphere, valid );
			valid = true;
		}
	}
	if( m_stretchObject )
	{
		if( m_stretchObject->GetBoundingSphere( v, query ) )
		{
			BoundingSphereSetOrUpdate( v, sphere, valid );
			valid = true;
		}
	}
	if( m_moveObject )
	{
		if( m_moveObject->GetBoundingSphere( v, query ) )
		{
			BoundingSphereSetOrUpdate( v, sphere, valid );
			valid = true;
		}
	}

	return valid;
}

float EveStretch3::GetCurveDuration()
{
	float maxDuration = 0;
	for( auto it = m_curveSets.begin(); it != m_curveSets.end(); ++it )
	{
		maxDuration = std::max( maxDuration, ( *it )->GetMaxCurveDuration() / ( *it )->GetTimeScale() );
	}

	return maxDuration;
}

void EveStretch3::StartFiring( float delay )
{
	SetControllerVariable( "firingDelay", delay );
	SetControllerVariable( "isFiring", 1 );
}

void EveStretch3::StopFiring()
{
	SetControllerVariable( "isFiring", 0 );
}

void EveStretch3::SetFiringTransform( const Matrix& source, const Vector3& dest )
{
	m_destinationPosition = dest;
	m_sourcePosition = source.GetTranslation();
}

void EveStretch3::SetFiringTransform( const Vector3& source, const Vector3& dest )
{
	m_destinationPosition = dest;
	m_sourcePosition = source;
}

void EveStretch3::DisplayEndPoints( bool displaySource, bool displayDest )
{

}

void EveStretch3::GetLights( Tr2LightManager& lightManager ) const
{
	if( !m_display )
	{
		return;
	}

	RunOnComponents( [&lightManager]( IEveSpaceObjectChild* c ) { c->GetLights( lightManager ); } );
}

void EveStretch3::GetBindingRoots( std::unordered_map<std::string, IRoot*>& variables )
{
	ITr2ControllerOwner::GetBindingRoots( variables );

	variables["Source"] = m_sourceObject;
	variables["Dest"] = m_destObject;
	variables["Stretch"] = m_stretchObject;
	variables["Move"] = m_moveObject;
	variables["SourceSpaceObject"] = m_sourceSpaceObject;
	variables["DestSpaceObject"] = m_destSpaceObject;
}


void EveStretch3::SetControllerVariable( const char* name, float value )
{
	RunOnComponents( [name, value]( IEveSpaceObjectChild* c ) { c->SetControllerVariable( name, value ); } );

	for( auto it = begin( m_controllers ); it != end( m_controllers ); ++it )
	{
		( *it )->SetVariable( name, value );
	}
}


void EveStretch3::HandleControllerEvent( const char* name )
{
	RunOnComponents( [name]( IEveSpaceObjectChild* c ) { c->HandleControllerEvent( name ); } );

	for( auto it = begin( m_controllers ); it != end( m_controllers ); ++it )
	{
		( *it )->HandleEvent( name );
	}
}

void EveStretch3::StartControllers()
{
	RunOnComponents( []( IEveSpaceObjectChild* c ) { c->StartControllers(); } );

	for( auto it = begin( m_controllers ); it != end( m_controllers ); ++it )
	{
		( *it )->Start();
	}
}


void EveStretch3::RegisterWithQuadRenderer( Tr2QuadRenderer& quadRenderer )
{
	RunOnComponents( [&quadRenderer]( IEveSpaceObjectChild* c ) { c->RegisterWithQuadRenderer( quadRenderer ); } );
}

void EveStretch3::AddQuadsToQuadRenderer( const TriFrustum& frustum, Tr2QuadRenderer& quadRenderer )
{
	RunOnComponents( [&frustum, &quadRenderer]( IEveSpaceObjectChild* c ) { c->AddQuadsToQuadRenderer( frustum, quadRenderer ); } );
}

void EveStretch3::UpdateModelCenterWorldPosition( Vector3& position, Be::Time t )
{
}

void EveStretch3::GetModelCenterWorldPosition( Vector3& position ) const
{
}

bool EveStretch3::GetLocalBoundingBox( Vector3& min, Vector3& max )
{ 
	return false; 
}

void EveStretch3::GetLocalToWorldTransform( Matrix& transform ) const
{ 
}

void EveStretch3::SetDestObjectScale( float scale ) 
{
	m_destObjectScale = scale; 
}

void EveStretch3::GetDebugOptions( Tr2DebugRendererOptions& options )
{
	options.insert( "Lights" );
	RunOnComponents( [&options]( IEveSpaceObjectChild* c ) { 
		auto debugRenderable = dynamic_cast< ITr2DebugRenderable* >( c );
		if( debugRenderable )
		{
			debugRenderable->GetDebugOptions( options );
		}
	} );
}

void EveStretch3::RenderDebugInfo( ITr2DebugRenderer2& renderer )
{
	if( !m_display )
	{
		return;
	}
	RunOnComponents( [&renderer]( IEveSpaceObjectChild* c ) {
		auto debugRenderable = dynamic_cast< ITr2DebugRenderable* >( c );
		if( debugRenderable )
		{
			debugRenderable->RenderDebugInfo( renderer );
		}
	} );
}

void EveStretch3::PlayCurveSet( const std::string& name, const std::string& rangeName )
{
	if( !m_display )
	{
		return;
	}

	for( auto it = m_curveSets.begin(); it != m_curveSets.end(); it++ )
	{
		if( ( *it )->GetName() == name )
		{
			if( rangeName.empty() )
			{
				( *it )->ResetTimeRange();
				( *it )->Play();
			}
			else
			{
				( *it )->PlayTimeRange( rangeName.c_str() );
			}
		}
	}

	RunOnComponents( [name, rangeName]( IEveSpaceObjectChild* c ) {
		auto curveSetOwner = dynamic_cast< ITr2CurveSetOwner* >( c );
		if( curveSetOwner )
		{
			curveSetOwner->PlayCurveSet( name, rangeName );
		}
	} );
}

void EveStretch3::StopCurveSet( const std::string& name )
{
	if( !m_display )
	{
		return;
	}

	for( auto it = m_curveSets.begin(); it != m_curveSets.end(); it++ )
	{
		if( ( *it )->GetName() == name )
		{
			( *it )->Stop();
		}
	}

	RunOnComponents( [name]( IEveSpaceObjectChild* c ) {
		auto curveSetOwner = dynamic_cast< ITr2CurveSetOwner* >( c );
		if( curveSetOwner )
		{
			curveSetOwner->StopCurveSet( name );
		}
	} );
}

void EveStretch3::UpdateCurveSet( const std::string& name, Be::Time time )
{
	for( auto it = m_curveSets.begin(); it != m_curveSets.end(); it++ )
	{
		if( ( *it )->GetName() == name )
		{
			( *it )->Update( time, time );
		}
	}
	
	RunOnComponents( [name, time]( IEveSpaceObjectChild* c ) {
		auto curveSetOwner = dynamic_cast< ITr2CurveSetOwner* >( c );
		if( curveSetOwner )
		{
			curveSetOwner->UpdateCurveSet( name, time );
		}
	} );

}

float EveStretch3::GetCurveSetDuration( const std::string& name ) const
{
	if( !m_display )
	{
		return 0.f;
	}

	float maxDuration = 0.f;
	for( auto it = m_curveSets.begin(); it != m_curveSets.end(); it++ )
	{
		if( ( *it )->GetName() == name )
		{
			maxDuration = max( maxDuration, ( *it )->GetMaxCurveDuration() );
		}
	}
	float componentMax = RunOnComponentsGetMax( [name]( IEveSpaceObjectChild* c )-> float {
		auto curveSetOwner = dynamic_cast< ITr2CurveSetOwner* >( c );
		if( curveSetOwner )
		{
			return curveSetOwner->GetCurveSetDuration( name );
		}
		return 0.0f;
	} );

	return max(maxDuration, componentMax);
}

float EveStretch3::GetRangeDuration( const std::string& name, const std::string& rangeName ) const
{
	if( !m_display )
	{
		return 0.f;
	}

	float maxDuration = 0.f;
	for( auto it = m_curveSets.begin(); it != m_curveSets.end(); it++ )
	{
		if( ( *it )->GetName() == name )
		{
			maxDuration = max( maxDuration, ( *it )->GetRangeDuration( rangeName.c_str() ) );
		}
	}

	float componentMax = RunOnComponentsGetMax( [name, rangeName]( IEveSpaceObjectChild* c ) -> float {
		auto curveSetOwner = dynamic_cast< ITr2CurveSetOwner* >( c );
		if( curveSetOwner )
		{
			return curveSetOwner->GetRangeDuration( name, rangeName );
		}
		return 0.0f;
	} );

	return max(maxDuration, componentMax);
}
