////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#include "StdAfx.h"
#include "Tr2ActionPlayCurveSet.h"


BLUE_DEFINE( Tr2ActionPlayCurveSet );

const Be::ClassInfo* Tr2ActionPlayCurveSet::ExposeToBlue()
{
	EXPOSURE_BEGIN( Tr2ActionPlayCurveSet, "" )
		MAP_INTERFACE( Tr2ActionPlayCurveSet )
		MAP_INTERFACE( ITr2ControllerAction )

		MAP_ATTRIBUTE( "curveSetName", m_curveSetName, "Curve set name", Be::READWRITE | Be::PERSIST )

		MAP_ATTRIBUTE( "playRange", m_range, "Play only a specified time range of the curve set\n:jessica-group: Time Range", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "startTime", m_startTime, "Time range start time. Only applies if playRange is set.\n:jessica-group: Time Range", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "endTime", m_endTime, "Time range start time. Only applies if playRange is set.\n:jessica-group: Time Range", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "looped", m_looped, "Play time range in a loop. Only applies if playRange is set.\n:jessica-group: Time Range", Be::READWRITE | Be::PERSIST )
	EXPOSURE_END()
}
