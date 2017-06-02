#include "StdAfx.h"
#include "Tr2MayaVector3Curve.h"

#if BLUE_WITH_PYTHON

BLUE_DEFINE( Tr2MayaVector3Curve );

const Be::ClassInfo* Tr2MayaVector3Curve::ExposeToBlue()
{
    EXPOSURE_BEGIN( Tr2MayaVector3Curve, ":jessica-icon: tree/trivectorcurve.png\n:jessica-deprecated:" )
        MAP_INTERFACE( Tr2MayaVector3Curve )
		MAP_INTERFACE( ITriFunction )
		MAP_INTERFACE( IInitialize )
		MAP_INTERFACE( ITriCurveLength )
		MAP_INTERFACE( INotify )
		MAP_PROPERTY( "xIndex", getXIndex, setXIndex, "" )
		MAP_PROPERTY( "yIndex", getYIndex, setYIndex, "" )
		MAP_PROPERTY( "zIndex", getZIndex, setZIndex, "" )
		MAP_ATTRIBUTE( "xIndex", m_xIndex, "", Be::PERSISTONLY )
		MAP_ATTRIBUTE( "yIndex", m_yIndex, "", Be::PERSISTONLY )
		MAP_ATTRIBUTE( "zIndex", m_zIndex, "", Be::PERSISTONLY )		
		MAP_ATTRIBUTE( "animationEngine", m_animationEngine, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "name", m_name, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "value", m_value, "", Be::READWRITE | Be::PERSIST )

		MAP_METHOD_AND_WRAP( "GetValueAt", GetValueAt , "" )
    EXPOSURE_END()
}

#endif
