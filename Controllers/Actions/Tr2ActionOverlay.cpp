////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#include "StdAfx.h"
#include "Tr2ActionOverlay.h"
#include "Controllers/Tr2Controller.h"
#include "Eve/SpaceObject/EveSpaceObject2.h"
#include "Utilities/StringUtils.h"


void Tr2ActionOverlay::Start( Tr2Controller& controller )
{
	EveSpaceObject2Ptr owner = BlueCastPtr( controller.GetOwner() );
	if( !owner )
	{
		return;
	}
	auto path = m_path;
	if( owner->IsAnimated() && !StringFind( path.c_str(), "_skinned" ) )
	{
		StringInsertStubBefore( path, ".red", "_skinned" );
	}
	else if( !owner->IsAnimated() && StringFind( path.c_str(), "_skinned" ) )
	{
		StringRemove( path, "_skinned" );
	}

	BeResMan->SetUrgentResourceLoads( true );
	m_overlay = BeResMan->LoadObject<EveMeshOverlayEffect>( path.c_str() );
	BeResMan->SetUrgentResourceLoads( false );
	if( m_overlay )
	{
		owner->AddOverlayEffect( m_overlay );
	}
}

void Tr2ActionOverlay::Stop( Tr2Controller& controller )
{
	if( !m_overlay )
	{
		return;
	}
	EveSpaceObject2Ptr owner = BlueCastPtr( controller.GetOwner() );
	if( !owner )
	{
		return;
	}
	owner->RemoveOverlayEffect( m_overlay );
}
