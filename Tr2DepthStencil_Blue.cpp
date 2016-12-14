#include "StdAfx.h"
#include "Tr2DepthStencil.h"

BLUE_DEFINE( Tr2DepthStencil );

const Be::ClassInfo* Tr2DepthStencil::ExposeToBlue()
{
	/////////////////////////////////////////
	// Blue class info
    EXPOSURE_BEGIN( Tr2DepthStencil, "" )

		MAP_INTERFACE( IRoot )
		MAP_INTERFACE( Tr2DepthStencil )
		MAP_INTERFACE( ITr2TextureProvider )

		MAP_METHOD_AND_WRAP
		(
			"IsValid",
			IsValid,
			"No arguments"
		)

		MAP_METHOD_AND_WRAP
		(
			"IsReadable",
			IsReadable,
			"No arguments\n"
			"Returns true if the depthStencil is readable, so setting it in the\n"
			"variable store will make it act like a source texture."
		)

		MAP_METHOD_AND_WRAP
		(
			"Create",
			Create,
			":param width: buffer width\n"
			":param height: buffer height\n"
			":param format: buffer format (trinity.DEPTH_STENCIL_FORMAT)\n"
			":param msaaType: sample count\n"
			":param msaaQuality: MSAA quality"
		)

		MAP_METHOD_AND_WRAP
		(
			"CreateEx",
			CreateEx,
			":param width: buffer width\n"
			":param height: buffer height\n"
			":param format: buffer format (trinity.DEPTH_STENCIL_FORMAT)\n"
			":param msaaType: sample count\n"
			":param msaaQuality: MSAA quality"
			":param flags: trinity.EX_FLAG"
		)

		MAP_ATTRIBUTE( "name",					m_name,							"", Be::READWRITE | Be::PERSIST );

		MAP_ATTRIBUTE( "width" ,				m_depthStencil.m_width,			"", Be::READ );
		MAP_ATTRIBUTE( "height",				m_depthStencil.m_height,		"", Be::READ );		
		MAP_ATTRIBUTE( "multiSampleType",		m_depthStencil.m_msaaType,		"", Be::READ );
		MAP_ATTRIBUTE( "multiSampleQuality",	m_depthStencil.m_msaaQuality,	"", Be::READ );

		MAP_ATTRIBUTE_WITH_CHOOSER( "format", m_depthStencil.m_format,	"", Be::READ | Be::ENUM, Tr2RenderContextEnum_DepthStencilFormat_Chooser );

		MAP_PROPERTY_READONLY( "isValid",		IsValid,	"IsValid()" );
		MAP_PROPERTY_READONLY( "isReadable",	IsReadable, "IsReadable()" );

		MAP_METHOD_AND_WRAP_OPTIONAL_ARGS( 
			"__init__", 
			py__init__, 
			5,
			"Provide no arguments, and call Create later, or provide\n" 
			":param width: buffer width\n"
			":param height: buffer height\n"
			":param format: buffer format (trinity.DEPTH_STENCIL_FORMAT)\n"
			":param msaaType: sample count\n"
			":param msaaQuality: MSAA quality"
			);

		MAP_METHOD_AND_WRAP( 
			"sharedHandle",	
			GetSharedHandle, 
			"sharedHandle" 
			);

		MAP_METHOD_AND_WRAP
		(
			"HasALObject",
			HasALObject,
			"Returns True iff Tr2DepthStencil contains a reference to passed AL object ID.\n"
			"Used for debugging along with trinity.GetLiveALResources.\n"
			":param alType: AL object type (trinity.AL_OBJECT_TYPE)\n"
			":param alObject: AL object ID"
		)

	EXPOSURE_END()
}
