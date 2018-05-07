////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#include "StdAfx.h"
#include "Tr2ActionPlayCurveSet.h"
#include "Controllers/Tr2Controller.h"
#include "ITr2CurveSetOwner.h"
#include "Curves/TriCurveSet.h"

#include "Eve/SpaceObject/EveSpaceObject2.h"
#include "Eve/SpaceObject/Children/IEveSpaceObjectChild.h"
#include "Eve/EveEffectRoot2.h"


Tr2ActionPlayCurveSet::Tr2ActionPlayCurveSet( IRoot* )
	:m_startTime( 0 ),
	m_endTime( 1 ),
	m_range( false ),
	m_looped( false )
{
}

void Tr2ActionPlayCurveSet::Start( Tr2Controller& controller )
{
	if( auto owner = dynamic_cast<ITr2CurveSetOwner*>( controller.GetOwner() ) )
	{
		owner->PlayCurveSet( m_curveSetName, m_range, m_startTime, m_endTime, m_looped );
	}
}

void Tr2ActionPlayCurveSet::Stop( Tr2Controller& controller )
{
	if( auto owner = dynamic_cast<ITr2CurveSetOwner*>( controller.GetOwner() ) )
	{
		owner->StopCurveSet( m_curveSetName );
	}
}
