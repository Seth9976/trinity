#include "StdAfx.h"
#include "Tr2ControllerReference.h"

BLUE_DEFINE( Tr2ControllerReference );

const Be::ClassInfo* Tr2ControllerReference::ExposeToBlue()
{
	EXPOSURE_BEGIN( Tr2ControllerReference, "" )
		MAP_INTERFACE( Tr2ControllerReference )
		MAP_INTERFACE( IInitialize )
		MAP_INTERFACE( INotify )
		MAP_INTERFACE( ITr2Controller )

		MAP_ATTRIBUTE( "path", m_path, "Path to a controller", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "controller", m_controller, "Loaded controller", Be::READ )
	EXPOSURE_END()
}
