////////////////////////////////////////////////////////////
//
//    Created:   April 2020
//    Copyright: CCP 2020
//

#include "StdAfx.h"
#include "Tr2AudioStretch.h"

BLUE_DEFINE( Tr2AudioStretch );
BLUE_DEFINE_INTERFACE( ITr2Audio );
BLUE_DEFINE_INTERFACE( ITr2AudEmitter );

const Be::ClassInfo* Tr2AudioStretch::ExposeToBlue()
{
	EXPOSURE_BEGIN( Tr2AudioStretch, "Use this to add audio to lasers or other looping stretch effects." )
		MAP_INTERFACE( ITr2Audio )
		MAP_INTERFACE( ITr2DebugRenderable )
		MAP_INTERFACE( IInitialize)

		MAP_ATTRIBUTE(
			"sourceEmitter",
			m_sourceEmitter,
			"Source audio emitter, Follows the source position. The outburst event will come from this emitter.",
			Be::READWRITE | Be::PERSIST
		)
		MAP_ATTRIBUTE(
			"destinationEmitter",
			m_destEmitter,
			"Destination audio emitter, follows the destination position. The impact event will come from this emitter.",
			Be::READWRITE | Be::PERSIST
		)
		MAP_ATTRIBUTE(
			"stretchEmitter",
			m_stretchEmitter,
			"Audio emitter for the looping part of the stretch effect. It follows the camera as long as it is between the "
			"source and destination. The stretch event will come from this emitter.",
			Be::READWRITE | Be::PERSIST
		)
		MAP_ATTRIBUTE(
			"outburstEvent",
			m_outburstEvent,
			"The event to play on the outburst of the weapon firing. Comes from the source audio emitter.",
			Be::READWRITE | Be::PERSIST
		)
		MAP_ATTRIBUTE(
			"impactEvent",
			m_impactEvent,
			"The event to play on the impact of the weapon hitting the destination. Comes from the destination audio emitter.",
			Be::READWRITE | Be::PERSIST
		)
		MAP_ATTRIBUTE(
			"stretchEvent",
			m_stretchEvent,
			"The event to play on the stretch effect between the source and destination. Comes from the stretch emitter.",
			Be::READWRITE | Be::PERSIST
		)

	EXPOSURE_END()
}
