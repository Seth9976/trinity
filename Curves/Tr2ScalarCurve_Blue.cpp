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
    EXPOSURE_BEGIN( Tr2ScalarCurve, "" )
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

		MAP_METHOD_AND_WRAP( "AddKey", AddKey, "AddKey( time, value )" );
		MAP_METHOD_AND_WRAP( "RemoveKey", RemoveKey, "RemoveKey( index )" );
		MAP_METHOD_AND_WRAP( "GetKeyCount", GetKeyCount, "GetKeyCount( )" );
		MAP_METHOD_AND_WRAP( "GetValueAt", GetValue, "GetValueAt( time )" );
		MAP_METHOD_AND_WRAP( "GetKeyValue", GetKeyValue, "GetKeyValue( index )" );
		MAP_METHOD_AND_WRAP( "GetKeyTime", GetKeyTime, "GetKeyTime( index )" );
		MAP_METHOD_AND_WRAP( "GetKeyLeftTangent", GetKeyLeftTangent, "GetKeyLeftTangent( index )" );
		MAP_METHOD_AND_WRAP( "GetKeyRightTangent", GetKeyRightTangent, "GetKeyRightTangent( index )" );
		MAP_METHOD_AND_WRAP( "GetKeyInterpolation", GetKeyInterpolation, "GetKeyInterpolation( index )" );
		MAP_METHOD_AND_WRAP( "SetKeyValue", SetKeyValue, "SetKeyValue( index, value )" );
		MAP_METHOD_AND_WRAP( "SetKeyTime", SetKeyTime, "SetKeyTime( index, time ). \nYou need to call Sort() afterwards, to make sure the keys are in the correct order." );
		MAP_METHOD_AND_WRAP( "SetKeyLeftTangent", SetKeyLeftTangent, "SetKeyLeftTangent( index, value )" );
		MAP_METHOD_AND_WRAP( "SetKeyRightTangent", SetKeyRightTangent, "SetKeyRightTangent( index, value )" );
		MAP_METHOD_AND_WRAP( "SetKeyInterpolation", SetKeyInterpolation, "SetKeyInterpolation( index, interpolation )" );
		MAP_METHOD_AND_WRAP( "Sort", Sort, "Sort the keys by time" );
    EXPOSURE_END()
}
