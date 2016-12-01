#include "StdAfx.h"
#include "Tr2RenderTarget.h"

BLUE_DEFINE( Tr2RenderTarget );

const Be::ClassInfo* Tr2RenderTarget::ExposeToBlue()
{
	/////////////////////////////////////////
	// Blue class info
    EXPOSURE_BEGIN( Tr2RenderTarget, "" )

		MAP_INTERFACE( IRoot )
		MAP_INTERFACE( Tr2RenderTarget )
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
			"Returns true if the renderTarget can be textured from."
		)

		MAP_METHOD_AND_WRAP
		(
			"Create",
			Create,
			"Arguments:\n"
			"width\n"
			"height\n"
			"mipLevelCount (0 = full pyramid)\n"
			"trinity.PIXEL_FORMAT"
		)

		MAP_METHOD_AND_WRAP
		(
			"CreateMsaa",
			CreateMsaa,
			"Arguments:\n"
			"width\n"
			"height\n"
			"trinity.PIXEL_FORMAT\n"
			"msaaType\n"
			"msaaQuality"
		)

		MAP_METHOD_AND_WRAP
		(
			"CreateEx",
			CreateEx,
			"Arguments:\n"
			"width\n"
			"height\n"
			"mipLevelCount\n"
			"trinity.PIXEL_FORMAT\n"
			"msaaType\n"
			"msaaQuality\n"
			"trinity.EX_FLAG"
		)

		MAP_METHOD_AND_WRAP
		(
			"GenerateMipMaps",
			GenerateMipMaps,
			"Generate mipmaps, if any"
		)
				
		MAP_METHOD_AND_WRAP
		(
			"Resolve",
			Resolve,
			"Resolve a renderTarget (typically MSAA ) into another RT (typically non-MSAA).\n"
			"May also work for just copying non-MSAA to non-MSAA, I'm not sure :P\n"
			"Arguments:\n"
			"destination - Tr2RenderTarget"
		)

		MAP_ATTRIBUTE( "name", m_name, "", Be::READWRITE | Be::PERSIST );

		MAP_PROPERTY_READONLY( "width", GetWidth, "" );
		MAP_PROPERTY_READONLY( "height", GetHeight, "" );		
		MAP_PROPERTY_READONLY( "mipCount", GetMipCount, "" );
		MAP_PROPERTY_READONLY( "multiSampleType", GetMsaaType, "" );
		MAP_PROPERTY_READONLY( "multiSampleQuality", GetMsaaQuality, "" );

		MAP_PROPERTY_READONLY( "format", GetFormat,	"" );

		MAP_PROPERTY_READONLY( "isValid",		IsValid,	"IsValid()" );
		MAP_PROPERTY_READONLY( "isReadable",	IsReadable, "IsReadable()" );

		MAP_METHOD_AND_WRAP_OPTIONAL_ARGS( 
			"__init__", 
			py__init__, 
			6,
			"Provide no arguments, and call Create/CreateMsaa later, or provide\n" 
			"width, height, mipCount, trinity.PIXEL_FORMAT, msaaType=0, msaaQuality=0."
			)

		MAP_METHOD_AND_WRAP
		(
			"CreateUAV",
			CreateUAV,
			"[Experimental] Create Unordered Access View"
		)

		MAP_METHOD_AND_WRAP( 
			"sharedHandle",	
			GetSharedHandle, 
			"sharedHandle" 
			);

		MAP_METHOD_AND_WRAP
		(
			"HasALObject",
			HasALObject,
			"Returns True iff Tr2RenderTarget contains a reference to passed AL object ID.\n"
			"Used for debugging along with trinity.GetLiveALResources.\n"
			"Arguments:\n"
			"type - AL object type (trinity.AL_OBJECT_TYPE)\n"
			"object - AL object ID"
		)

	EXPOSURE_END()
}
