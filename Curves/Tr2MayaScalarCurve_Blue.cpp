#include "StdAfx.h"
#include "Tr2MayaScalarCurve.h"

#if BLUE_WITH_PYTHON

BLUE_DEFINE( Tr2MayaScalarCurve );

const Be::ClassInfo* Tr2MayaScalarCurve::ExposeToBlue()
{
    EXPOSURE_BEGIN( Tr2MayaScalarCurve, ":jessica-icon: tree/triscalarcurve.png\n:jessica-deprecated:" )
        MAP_INTERFACE( Tr2MayaScalarCurve )
		MAP_INTERFACE( ITriFunction )
		MAP_INTERFACE( ITriScalarFunction )
		MAP_INTERFACE( IInitialize )
		MAP_INTERFACE( ITriCurveLength )
		MAP_INTERFACE( INotify )
		MAP_PROPERTY( "index", getIndex, setIndex, "" )
		MAP_ATTRIBUTE( "index", m_index, "", Be::PERSISTONLY )		
		MAP_ATTRIBUTE( "animationEngine", m_animationEngine, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "name", m_name, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "value", m_value, "", Be::READWRITE )
    EXPOSURE_END()
}

#endif
