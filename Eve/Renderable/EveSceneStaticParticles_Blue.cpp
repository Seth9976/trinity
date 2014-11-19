////////////////////////////////////////////////////////////
//
//    Created:   November 2014
//    Copyright: CCP 2014
//
#include "StdAfx.h"
#include "EveSceneStaticParticles.h"

BLUE_DEFINE( EveSceneStaticParticles );

const Be::ClassInfo* EveSceneStaticParticles::ExposeToBlue()
{
    EXPOSURE_BEGIN( EveSceneStaticParticles, "" )
        MAP_INTERFACE( EveSceneStaticParticles )
		MAP_INTERFACE( IInitialize )

		MAP_ATTRIBUTE( "maxParticleCount", m_maxParticleCount, "The total particle count this module can handle", Be::READWRITE )
		MAP_ATTRIBUTE( "minSize", m_minSize, "Minimum size of particles", Be::READWRITE )
		MAP_ATTRIBUTE( "maxSize", m_maxSize, "Maximum size of particles", Be::READWRITE )
		MAP_ATTRIBUTE( "clusterParticleDensity", m_clusterParticleDensity, "How many particles per radius", Be::READWRITE )
		MAP_ATTRIBUTE( "clusterParticleDensityAdjust", m_clusterParticleDensityAdjust, "DEBUG: how much do we have to reduce", Be::READ )

		MAP_ATTRIBUTE( "transform", m_transform, "The particle system", Be::READWRITE )

		MAP_METHOD_AND_WRAP(
			"AddCluster",
			AddCluster,
			"Add a whole cluster of particles"
			"\n" 
			"\nArguments:"
			"\nposition - The center position of the cluster in global 3d space (double precision)"
			"\nradius - The radius of the cluster"
			"\ncolor1 - First color for the cluster to interpolate from"
			"\ncolor2 - Second color for the cluster to interpolate from" )

		MAP_METHOD_AND_WRAP( "ClearClusters", ClearClusters, "Remove all clusters" )
		MAP_METHOD_AND_WRAP( "Rebuild", Rebuild, "Once finished adding clusters, we need to build internal buffers etc." )

    EXPOSURE_END()
}
