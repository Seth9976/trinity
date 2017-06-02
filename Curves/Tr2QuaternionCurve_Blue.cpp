#include "StdAfx.h"
#include "Tr2QuaternionCurve.h"

BLUE_DEFINE( Tr2QuaternionCurve );
BLUE_DEFINE( Tr2QuaternionKey );

Be::VarChooser QuaternionInterpolationChooser[] =
{
	{ 
		"CONSTANT", 
		BeCast(CONSTANT), 
		"Performs a constant interpolation" 
	},
	{ 
		"SPHERICAL_LINEAR", 
		BeCast(SPHERICAL_LINEAR), 
		"Performs a spherical linear interpolation" 
	},
	{ 0 }
};

// --------------------------------------------------------------------------------------
// Description:
//   Builds the ClassInfo for exposing Tr2QuaternionKey to Blue.
// Return Value:
//   A ClassInfo containing exposure metadata for the Tr2QuaternionKey.
// --------------------------------------------------------------------------------------
const Be::ClassInfo* Tr2QuaternionKey::ExposeToBlue()
{
	EXPOSURE_BEGIN( Tr2QuaternionKey, "" )
		MAP_INTERFACE( Tr2QuaternionKey )

		MAP_ATTRIBUTE( "time", m_time, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE_WITH_CHOOSER( "interpolation", m_interpolation, "", Be::READWRITE | Be::PERSIST | Be::ENUM, QuaternionInterpolationChooser )
		MAP_ATTRIBUTE( "value", m_value, "", Be::READWRITE | Be::PERSIST )

	EXPOSURE_END()
}

// --------------------------------------------------------------------------------------
// Description:
//   Builds the ClassInfo for exposing Tr2QuaternionCurve to Blue.
// Return Value:
//   A ClassInfo containing exposure metadata for the Tr2QuaternionCurve.
// --------------------------------------------------------------------------------------
const Be::ClassInfo* Tr2QuaternionCurve::ExposeToBlue()
{
	EXPOSURE_BEGIN( Tr2QuaternionCurve, ":jessica-icon: tree/trirotationcurve.png\n:jessica-deprecated:" )
		MAP_INTERFACE( Tr2QuaternionCurve )
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

		MAP_ATTRIBUTE_WITH_CHOOSER( "interpolation", m_interpolation, "", Be::READWRITE | Be::PERSIST | Be::ENUM, QuaternionInterpolationChooser )
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
			"SetKeyInterpolation", 
			SetKeyInterpolation, 
			"Changes key interpolation type\n"
			":param idx: key index\n"
			":param interpolation: new interpolation type" );
		MAP_METHOD_AND_WRAP( "Sort", Sort, "Sort the keys by time" );

	EXPOSURE_END()
}
