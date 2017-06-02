#include "StdAfx.h"
#include "Tr2MayaEulerRotationCurve.h"

#if BLUE_WITH_PYTHON

BLUE_DEFINE( Tr2MayaEulerRotationCurve );

const Be::ClassInfo* Tr2MayaEulerRotationCurve::ExposeToBlue()
{
    EXPOSURE_BEGIN( Tr2MayaEulerRotationCurve, ":jessica-icon: tree/trirotationcurve.png\n:jessica-deprecated:" )
        MAP_INTERFACE( Tr2MayaEulerRotationCurve )
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
		MAP_ATTRIBUTE( "eulerValue", m_eulerValue, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "updateQuaternion", m_updateQuaternion, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "quatValue", m_quatValue, "", Be::READWRITE )

		MAP_METHOD_AND_WRAP( "GetEulerValueAt", GetEulerValueAt, "" )
		MAP_METHOD_AND_WRAP( "GetValueAt", GetValueAt, "" )
	EXPOSURE_END()
}

#endif
