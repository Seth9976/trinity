#include "StdAfx.h"
#include "Tr2ScalarBezierCurve.h"

BLUE_DEFINE( Tr2ScalarBezierCurve );
BLUE_DEFINE( Tr2ScalarBezierKey );

// --------------------------------------------------------------------------------------
// Description:
//   Builds the ClassInfo for exposing Tr2ScalarBezierKey to Blue.
// Return Value:
//   A ClassInfo containing exposure metadata for the Tr2ScalarBezierKey.
// --------------------------------------------------------------------------------------
const Be::ClassInfo* Tr2ScalarBezierKey::ExposeToBlue()
{
	EXPOSURE_BEGIN( Tr2ScalarBezierKey, "" )
		MAP_INTERFACE( Tr2ScalarBezierKey )

		MAP_ATTRIBUTE( "time", m_time, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "value", m_value, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "leftControlPoint", m_leftControlPoint, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "rightControlPoint", m_rightControlPoint, "", Be::READWRITE | Be::PERSIST )

		EXPOSURE_END()
}

// --------------------------------------------------------------------------------------
// Description:
//   Builds the ClassInfo for exposing Tr2ScalarBezierCurve to Blue.
// Return Value:
//   A ClassInfo containing exposure metadata for the Tr2ScalarBezierCurve.
// --------------------------------------------------------------------------------------
const Be::ClassInfo* Tr2ScalarBezierCurve::ExposeToBlue()
{
	EXPOSURE_BEGIN( Tr2ScalarBezierCurve, ":jessica-icon: tree/triscalarcurve.png\n:jessica-deprecated:" )
		MAP_INTERFACE( Tr2ScalarBezierCurve )
		MAP_INTERFACE( ITriFunction )
		MAP_INTERFACE( IInitialize )
		MAP_INTERFACE( ITriCurveLength )

		MAP_ATTRIBUTE( "name", m_name, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "length", m_length, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "cycle", m_cycle, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "reversed", m_reversed, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "startValue", m_startValue, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "currentValue", m_currentValue, "", Be::READ )
		MAP_ATTRIBUTE( "endValue", m_endValue, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "controlPointA", m_controlPointA, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "controlPointB", m_controlPointB, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "keys", m_keys, "These are the keys of the curve", Be::PERSISTONLY )

		MAP_METHOD_AND_WRAP( 
			"AddKey", 
			AddKey, 
			"Adds a new key\n"
			":param time: key time\n"
			":param value: key value"
			);
		MAP_METHOD_AND_WRAP( 
			"RemoveKey", 
			RemoveKey, 
			"Removes a key\n"
			":param idx: key index" );
		MAP_METHOD_AND_WRAP( "GetKeyCount", GetKeyCount, "Returns number of keys" );
		MAP_METHOD_AND_WRAP( 
			"GetValueAt", 
			GetValueAt, 
			"Returns curve value at a given time\n"
			":param time: input time" );
		MAP_METHOD_AND_WRAP( 
			"GetKeyValue", 
			GetKeyValue, 
			"Returns key value\n"
			":param idx: key index" );
		MAP_METHOD_AND_WRAP( 
			"GetKeyTime", 
			GetKeyTime, 
			"Returns key time\n"
			":param idx: key index" );
		MAP_METHOD_AND_WRAP( 
			"GetKeyInterpolation", 
			GetKeyInterpolation, 
			"Returns interpolation type for the key\n"
			":param idx: key index" );
		MAP_METHOD_AND_WRAP( 
			"SetKeyValue", 
			SetKeyValue, 
			"Changes key value\n"
			":param idx: key index\n"
			":param value: new key value" );
		MAP_METHOD_AND_WRAP( 
			"SetKeyTime", 
			SetKeyTime, 
			"Changes key time.\n"
			"You need to call Sort() afterwards, to make sure the keys are in the correct order.\n"
			":param idx: key index\n"
			":param time: new key time" );
		MAP_METHOD_AND_WRAP( 
			"SetKeyRightControlPoint", 
			SetKeyRightControlPoint, 
			"Changes key right control point\n"
			":param idx: key index\n"
			":param value: control point value" );
		MAP_METHOD_AND_WRAP( 
			"SetKeyLeftControlPoint", 
			SetKeyLeftControlPoint, 
			"Changes key left control point\n"
			":param idx: key index\n"
			":param value: control point value" );
		MAP_METHOD_AND_WRAP( 
			"GetKeyRightControlPoint", 
			GetKeyRightControlPoint, 
			"Returns key right control point\n"
			":param idx: key index\n" );
		MAP_METHOD_AND_WRAP( 
			"GetKeyLeftControlPoint", 
			GetKeyLeftControlPoint, 
			"Returns key left control point\n"
			":param idx: key index\n" );
		MAP_METHOD_AND_WRAP( 
			"SetKeyInterpolation", 
			SetKeyInterpolation, 
			"Changes key interpolation type\n"
			":param idx: key index\n"
			":param interpolation: new interpolation type" );
		MAP_METHOD_AND_WRAP( "Sort", Sort, "Sort the keys by time" );
	EXPOSURE_END()
}
