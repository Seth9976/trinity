////////////////////////////////////////////////////////////
//
//    Created:   May 2018
//    Copyright: CCP 2018
//

#include "StdAfx.h"
#include "Tr2ActionChildEffect.h"
#include "Controllers/Tr2Controller.h"
#include "Eve/SpaceObject/Children/IEveEffectChildrenOwner.h"
#include "Eve/SpaceObject/Children/IEveSpaceObjectChild.h"


Tr2ActionChildEffect::Tr2ActionChildEffect( IRoot* )
	:m_addOnStart( true ),
	m_removeOnStop( true )
{
}

void Tr2ActionChildEffect::Start( Tr2Controller& controller )
{
	IEveEffectChildrenOwnerPtr owner = BlueCastPtr( controller.GetOwner() );
	if( !owner )
	{
		return;
	}
	m_child = nullptr;
	if( !m_childName.empty() )
	{
		m_child = owner->GetEffectChildByName( m_childName.c_str() );
	}
	if( m_addOnStart && !m_child && !m_path.empty() )
	{
		m_child = BeResMan->LoadObject<IEveSpaceObjectChild>( m_path.c_str() );
		if( m_child )
		{
			if( !m_childName.empty() )
			{
				m_child->SetName( m_childName.c_str() );
			}
			owner->AddToEffectChildrenList( m_child );
		}
	}
}

void Tr2ActionChildEffect::Stop( Tr2Controller& controller )
{
	if( m_child && m_removeOnStop )
	{
		if( IEveEffectChildrenOwnerPtr owner = BlueCastPtr( controller.GetOwner() ) )
		{
			owner->RemoveFromEffectChildrenList( m_child );
		}
	}
	m_child = nullptr;
}
