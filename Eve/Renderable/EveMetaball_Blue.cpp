////////////////////////////////////////////////////////////
//
//    Created:   January 2014
//    Copyright: CCP 2014
//
#include "StdAfx.h"
#include "EveMetaball.h"

BLUE_DEFINE( EveMetaball );

const Be::ClassInfo* EveMetaball::ExposeToBlue()
{
    EXPOSURE_BEGIN( EveMetaball, "" )
        MAP_INTERFACE( EveMetaball )
		MAP_INTERFACE( IEveSpaceObject2 )
		MAP_INTERFACE( ITr2Renderable )
		MAP_INTERFACE( ITr2GeometryProvider )
		MAP_INTERFACE( IInitialize )

		MAP_ATTRIBUTE( "name", m_name, "The name of this metaball", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "display", m_display, "Toggle display", Be::READWRITE )

		MAP_ATTRIBUTE( "boxSize", m_boxSize, "DEBUG: sz", Be::READWRITE )

		MAP_ATTRIBUTE( "triangleCount", m_triangleCount, "Used for culling", Be::READ )

		MAP_ATTRIBUTE( "effect", m_effect, "Shader used for the rendering the surface of this metaball", Be::READWRITE | Be::PERSIST )

		MAP_ATTRIBUTE( "sourceItems", m_sourceItems, "all the spheres", Be::READWRITE | Be::PERSIST )

		MAP_ATTRIBUTE( "boundingSphere", m_boundingSphere, "Used for culling", Be::READ )
		MAP_ATTRIBUTE( "minBounds", m_minBounds, "Used for culling", Be::READ )
		MAP_ATTRIBUTE( "maxBounds", m_maxBounds, "Used for culling", Be::READ )

		MAP_ATTRIBUTE( "isolevel", m_isolevel, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "gooValue", m_gooValue, "", Be::READWRITE | Be::PERSIST )

		MAP_METHOD_AND_WRAP( "UpdateBuffers", UpdateBuffers, "rebuild internal data" )

    EXPOSURE_END()
}
