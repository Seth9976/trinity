#include "StdAfx.h"
#include "Tr2ScalarExprCurve.h"

BLUE_DEFINE( Tr2ScalarExprCurve );

const Be::ClassInfo* Tr2ScalarExprCurve::ExposeToBlue()
{
    EXPOSURE_BEGIN( Tr2ScalarExprCurve, ":jessica-deprecated:" )
        MAP_INTERFACE( Tr2ScalarExprCurve )
		MAP_INTERFACE( ITriCurveLength )
		MAP_INTERFACE( ITriFunction )

		MAP_PROPERTY( "expr", GetExpression, SetExpression, "The mathematical expression to be evaluated" )		
		MAP_ATTRIBUTE( "expr", m_expression, "", Be::READWRITE| Be::PERSISTONLY )
		MAP_ATTRIBUTE( "input1", m_inputVar1, "Input variable to influence the expression", Be::READWRITE| Be::PERSIST  )
		MAP_ATTRIBUTE( "input2", m_inputVar2, "Input variable to influence the expression", Be::READWRITE| Be::PERSIST  )
		MAP_ATTRIBUTE( "input3", m_inputVar3, "Input variable to influence the expression", Be::READWRITE| Be::PERSIST  )
		MAP_ATTRIBUTE( "input4", m_inputVar4, "Input variable to influence the expression", Be::READWRITE| Be::PERSIST  )
		MAP_ATTRIBUTE( "randomMin", m_randomMin, "Random minimum value for the randomConstant variable", Be::READWRITE| Be::PERSIST  )
		MAP_ATTRIBUTE( "randomMax", m_randomMax, "Random maximum value for the randomConstant variable", Be::READWRITE| Be::PERSIST  )
		MAP_ATTRIBUTE( "sourcePositionA", m_sourcePositionA, "Source position to compute distance", Be::READWRITE| Be::PERSIST  )
		MAP_ATTRIBUTE( "sourcePositionB", m_sourcePositionB, "Source position to compute distance", Be::READWRITE| Be::PERSIST  )
		MAP_ATTRIBUTE( "updateDistance", m_updateDistance, "Should we compute the distance between the two source on each interpolation", Be::READWRITE| Be::PERSIST  )
		MAP_METHOD_AND_WRAP( "RegenRandomConstant", RegenRandomConstant, "Regenerate the randomConstant variable for the expression")
	EXPOSURE_CHAINTO(Tr2ScalarCurve)
}
