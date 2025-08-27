#include "StdAfx.h"
#include "EveAudioObject.h"

BLUE_DEFINE( EveAudioObject );

const Be::ClassInfo* EveAudioObject::ExposeToBlue()
{
    EXPOSURE_BEGIN( EveAudioObject, "Standalone audio object for 3D positioning without Trinity asset dependency." )
		MAP_INTERFACE( IEveSpaceObject2 )
		MAP_INTERFACE( IInitialize )
		MAP_INTERFACE( IWorldPosition )
		MAP_INTERFACE( ITr2DebugRenderable )
		MAP_INTERFACE( INotify )

		MAP_ATTRIBUTE
		(
			"name",
			m_name,
			"Name of the audio object",
			Be::READWRITE | Be::PERSIST
		)

		MAP_ATTRIBUTE
		(
			"display",
			m_display,
			"Whether the object is displayed/active",
			Be::READWRITE | Be::PERSIST | Be::NOTIFY
		)
		
		MAP_ATTRIBUTE
		( 
			"mute",
			m_mute, 
			"Mute state of the audio emitter",
			Be::READWRITE | Be::NOTIFY
		)
		
		MAP_ATTRIBUTE
		(
			"scaling",
			m_scaling,
			"Local scaling of the object",
			Be::READWRITE | Be::PERSIST
		)
		
		MAP_ATTRIBUTE
		(
			"rotation",
			m_rotation,
			"Local rotation of the object",
			Be::READWRITE | Be::PERSIST
		)
		
		MAP_ATTRIBUTE
		(
			"translation",
			m_translation,
			"Local translation of the object",
			Be::READWRITE | Be::PERSIST
		)
		
		MAP_ATTRIBUTE
		(
			"ballPosition",
			m_ballPosition,
			"Function for animated position updates",
			Be::READWRITE | Be::PERSIST
		)
		
		MAP_ATTRIBUTE
		(
			"ballRotation",
			m_ballRotation,
			"Function for animated rotation updates",
			Be::READWRITE | Be::PERSIST
		)
		
		MAP_ATTRIBUTE
		(
			"audioEvent",
			m_audioEvent,
			"Audio event name to play on this emitter",
			Be::READWRITE | Be::PERSIST | Be::NOTIFY
		)

		MAP_METHOD_AND_WRAP( "GetAudioEmitter", GetAudioEmitter, "Gets the audio emitter for this object" )
		
		MAP_METHOD_AND_WRAP( "SetEmitterName", SetEmitterName, "Sets the name of this object's audio emitter" )
		
		MAP_METHOD_AND_WRAP( "SetPosition", SetPosition, "Sets the world position of this audio object" )
		
		MAP_METHOD_AND_WRAP( "SetRotation", SetRotation, "Sets the world rotation of this audio object" )
		
		MAP_METHOD_AND_WRAP( "PlayAudioEvent", PlayAudioEvent, "Plays an audio event on this object's emitter" )
		
		MAP_METHOD_AND_WRAP( "SetMute", SetMute, "Sets mute state for this object's emitter" )
		
		MAP_METHOD_AND_WRAP( "GetMute", GetMute, "Gets current mute state" )

    EXPOSURE_END();
}