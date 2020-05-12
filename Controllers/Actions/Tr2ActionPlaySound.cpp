////////////////////////////////////////////////////////////
//
//    Created:   October 2018
//    Copyright: CCP 2018
//

#include "StdAfx.h"
#include "Tr2ActionPlaySound.h"
#include "Controllers/Tr2Controller.h"
#include "ITr2SoundEmitterOwner.h"
#include "Audio/ITr2AudEmitter.h"


Tr2ActionPlaySound::Tr2ActionPlaySound( IRoot* )
{
	m_bypassPrefix = false;
}

void Tr2ActionPlaySound::Start( Tr2Controller& controller )
{
	if( ITr2SoundEmitterOwnerPtr emitters = BlueCastPtr( controller.GetOwner() ) )
	{
		if( auto emitter = emitters->FindSoundEmitter( m_emitterName.c_str() ) )
		{
			emitter->SendEvent( m_soundEvent, m_bypassPrefix );
		}
	}
}
