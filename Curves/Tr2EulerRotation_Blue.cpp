#include "StdAfx.h"
#include "Tr2EulerRotation.h"

BLUE_DEFINE( Tr2EulerRotation );

// --------------------------------------------------------------------------------------
// Description:
//   Builds the ClassInfo for exposing Tr2EulerRotation to Blue.
// Return Value:
//   A ClassInfo containing exposure metadata for the Tr2EulerRotation.
// --------------------------------------------------------------------------------------
const Be::ClassInfo* Tr2EulerRotation::ExposeToBlue()
{
    EXPOSURE_BEGIN( Tr2EulerRotation, ":jessica-icon: tree/trirotationcurve.png\n:jessica-deprecated:" )
        MAP_INTERFACE( Tr2EulerRotation )
		MAP_INTERFACE( ITriFunction )
		MAP_INTERFACE( ITriCurveLength )

		MAP_ATTRIBUTE( "name", m_name, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "yawCurve", m_yawCurve, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "pitchCurve", m_pitchCurve, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "rollCurve", m_rollCurve, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "currentValue", m_currentValue, "", Be::READ )
		MAP_METHOD_AND_WRAP( 
			"GetValueAt", 
			GetValueAt, 
			"Return curve value at the specified time\n"
			":param time: input time" );
    EXPOSURE_END()
}
