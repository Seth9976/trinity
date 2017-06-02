#include "StdAfx.h"
#include "Tr2ScalarCurve.h"

BLUE_DEFINE( Tr2ScalarCurve );
BLUE_DEFINE( Tr2ScalarKey );

const Be::VarChooser InterpolationChooser[] =
{
	// Name									Value						Docstring
	{ "TR2CURVE_CONSTANT",				BeCast( CONSTANT ),				"Constant interpolation" }, 
	{ "TR2CURVE_LINEAR",				BeCast( LINEAR ),				"Linear interpolation" },
	{ "TR2CURVE_HERMITE",				BeCast( HERMITE ),				"Hermite interpolation" },
	{ "TR2CURVE_CATMULLROM",			BeCast( CATMULLROM ),			"Catmull-Rom interpolation(Another form of Bezier)" },
	{ "TR2CURVE_SPHERICAL_LINEAR",		BeCast( SPHERICAL_LINEAR ),		"Spherical-linear interpolation" },
	{ "TR2CURVE_SPHERICAL_QUADRANGLE",	BeCast( SPHERICAL_QUADRANGLE ),	"Spherical-quadrangle interpolation" },
	{0}
};


BLUE_REGISTER_ENUM( "TR2CURVE_INTERPOLATION", Interpolation, InterpolationChooser );

Be::VarChooser ScalarInterpolationChooser[] =
{
	{ 
		"CONSTANT", 
		BeCast(CONSTANT), 
		"Performs a constant interpolation" 
	},
	{ 
		"LINEAR",   
		BeCast(LINEAR),   
		"Performs a linear interpolation" 
	},
	{ 
		"HERMITE",  
		BeCast(HERMITE),  
		"Performs a Hermite spline interpolation" 
	},
	{ 0 }
};

// --------------------------------------------------------------------------------------
// Description:
//   Builds the ClassInfo for exposing Tr2ScalarKey to Blue.
// Return Value:
//   A ClassInfo containing exposure metadata for the Tr2ScalarKey.
// --------------------------------------------------------------------------------------
const Be::ClassInfo* Tr2ScalarKey::ExposeToBlue()
{
    EXPOSURE_BEGIN( Tr2ScalarKey, "" )
        MAP_INTERFACE( Tr2ScalarKey )
			
		MAP_ATTRIBUTE( "time", m_time, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "value", m_value, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "leftTangent", m_leftTangent, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "rightTangent", m_rightTangent, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE_WITH_CHOOSER( "interpolation", m_interpolation, "", Be::READWRITE | Be::PERSIST | Be::ENUM, ScalarInterpolationChooser )

	EXPOSURE_END()
}

// --------------------------------------------------------------------------------------
// Description:
//   Builds the ClassInfo for exposing Tr2ScalarCurve to Blue.
// Return Value:
//   A ClassInfo containing exposure metadata for the Tr2ScalarCurve.
// --------------------------------------------------------------------------------------
const Be::ClassInfo* Tr2ScalarCurve::ExposeToBlue()
{
    EXPOSURE_BEGIN( Tr2ScalarCurve, ":jessica-icon: tree/triscalarcurve.png\n:jessica-deprecated:" )
        MAP_INTERFACE( Tr2ScalarCurve )
		MAP_INTERFACE( ITriFunction )
		MAP_INTERFACE( ITriScalarFunction )
        MAP_INTERFACE( IInitialize )
		MAP_INTERFACE( ITriCurveLength )

		MAP_ATTRIBUTE( "name", m_name, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "length", m_length, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "cycle", m_cycle, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "reversed", m_reversed, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "timeOffset", m_timeOffset, "An internal offset to the curve's timing", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "timeScale", m_timeScale, "An internal scaling to the curve's timing", Be::READWRITE | Be::PERSIST )

		MAP_ATTRIBUTE( "startValue", m_startValue, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "currentValue", m_currentValue, "", Be::READ )
		MAP_ATTRIBUTE( "endValue", m_endValue, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "startTangent", m_startTangent, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "endTangent", m_endTangent, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE_WITH_CHOOSER( "interpolation", m_interpolation, "", Be::READWRITE | Be::PERSIST | Be::ENUM, ScalarInterpolationChooser )
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
			GetValue, 
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
			"GetKeyLeftTangent", 
			GetKeyLeftTangent, 
			"Returns key left tangent value\n"
			":param idx: key index" );
		MAP_METHOD_AND_WRAP( 
			"GetKeyRightTangent", 
			GetKeyRightTangent, 
			"Returns key left tangent value\n"
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
			"SetKeyLeftTangent", 
			SetKeyLeftTangent, 
			"Changes key left tangent value"
			":param idx: key index\n"
			":param value: new key left tangent value" );
		MAP_METHOD_AND_WRAP( 
			"SetKeyRightTangent", 
			SetKeyRightTangent, 
			"Changes key right tangent value"
			":param idx: key index\n"
			":param value: new key left tangent value" );
		MAP_METHOD_AND_WRAP( 
			"SetKeyInterpolation", 
			SetKeyInterpolation, 
			"Changes key interpolation type\n"
			":param idx: key index\n"
			":param interpolation: new interpolation type" );
		MAP_METHOD_AND_WRAP( "Sort", Sort, "Sort the keys by time" );
    EXPOSURE_END()
}
