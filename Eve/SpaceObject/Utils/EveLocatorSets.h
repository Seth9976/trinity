////////////////////////////////////////////////////////////
//
//    Created:   February 2016
//    Copyright: CCP 2016
//

#pragma once
#ifndef EveLocatorSets_H
#define EveLocatorSets_H

// forwards
struct Locator
{
	Vector3 position;
	Quaternion direction;
	int boneIndex;
};
BLUE_DECLARE_STRUCTURE_LIST( Locator );

// --------------------------------------------------------------------------------
// Description:
//   A set of locatorlists, identified by names
// --------------------------------------------------------------------------------
BLUE_CLASS( EveLocatorSets ) :
	public IRoot
{
public:
	EXPOSE_TO_BLUE();

	EveLocatorSets( IRoot* lockobj = NULL );
	~EveLocatorSets();

	// access
	void Set( const char* name, const Locator* locators, size_t count );
	
private:
	// name to identify set
	BlueSharedString m_name;

	// the locators
	PLocatorStructureList m_locators;
};

TYPEDEF_BLUECLASS( EveLocatorSets );

#endif