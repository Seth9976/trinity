#include "StdAfx.h"
#include "KDdroneManagementTree.h"


BLUE_DEFINE( KDdroneManagementTree );

const Be::ClassInfo* KDdroneManagementTree::ExposeToBlue()
{
	EXPOSURE_BEGIN( KDdroneManagementTree, "" )
		MAP_INTERFACE( KDdroneManagementTree )
		//MAP_INTERFACE( IRoot )


		MAP_ATTRIBUTE( "freq", m_freq, ":jessica-group: Wander", Be::READWRITE | Be::PERSIST )
		
	EXPOSURE_END()
}