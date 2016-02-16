////////////////////////////////////////////////////////////
//
//    Created:   Februaru 2016
//    Copyright: CCP 2016
//

#include "StdAfx.h"
#include "EveLocatorSets.h"

// locator item definition
static BlueStructureDefinition LocatorStructureDef[] =
{
	{ "position", Be::FLOAT32_3, 0 },
	{ "direction", Be::FLOAT32_4, 12 },
	{ "boneIndex", Be::INT32_1, 28 },
	{ 0 }
};

// --------------------------------------------------------------------------------
// Description:
//   Initialize data members
// --------------------------------------------------------------------------------
EveLocatorSets::EveLocatorSets( IRoot* lockobj ) :
	PARENTLOCK( m_locators )
{
	m_locators.SetStructureDefinition( LocatorStructureDef );
}

// --------------------------------------------------------------------------------
// Description:
//   Byebye
// --------------------------------------------------------------------------------
EveLocatorSets::~EveLocatorSets()
{
}

// --------------------------------------------------------------------------------
// Description:
//   Setutp from the outisde
// --------------------------------------------------------------------------------
void EveLocatorSets::Set( const char* name, const Locator* locators, size_t count )
{
	m_name = BlueSharedString( name );
	m_locators.Resize( count );
	memcpy( &m_locators[0], locators, count * sizeof( Locator ) );
}


