////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#include "StdAfx.h"
#include "Tr2ActionSetValue.h"


BLUE_DEFINE( Tr2ActionSetValue );

const Be::ClassInfo* Tr2ActionSetValue::ExposeToBlue()
{
	EXPOSURE_BEGIN( Tr2ActionSetValue, "" )
		MAP_INTERFACE( Tr2ActionSetValue )
		MAP_INTERFACE( ITr2ControllerAction )
		MAP_INTERFACE( INotify )

		MAP_ATTRIBUTE( "path", m_destination.m_path, "Path to the destination object for shared controllers", Be::READWRITE | Be::PERSIST | Be::NOTIFY )
		MAP_ATTRIBUTE( "destination", m_destination.m_object, "Destination object", Be::READWRITE | Be::PERSIST | Be::NOTIFY )
		MAP_ATTRIBUTE( "attribute", m_destination.m_attribute, "Destination attribute name", Be::READWRITE | Be::PERSIST | Be::NOTIFY )
		MAP_ATTRIBUTE( "value", m_value, "Attribute value expression", Be::READWRITE | Be::PERSIST | Be::NOTIFY )

		MAP_PROPERTY_READONLY( "isExpressionValid", IsExpressionValid, "Is \"value\" expression valid" )
		MAP_PROPERTY_READONLY( "isBindingValid", IsBindingValid, "Is destination binding valid" )

		MAP_METHOD_AND_WRAP( "GetDestination", GetDestination, "Returns destination object" )
	EXPOSURE_END()
}
