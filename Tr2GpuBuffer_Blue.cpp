////////////////////////////////////////////////////////////
//
//    Created:   February 2013
//    Copyright: CCP 2013
//

#include "StdAfx.h"
#include "Tr2GpuBuffer.h"

BLUE_DEFINE_INTERFACE( ITr2GpuBuffer );
BLUE_DEFINE( Tr2GpuBuffer );

Be::VarChooser Tr2GpuBufferCreationFlagChooser[] =
{
	{
		"CPU_WRITABLE",
		BeCast( Tr2GpuBuffer::CPU_WRITABLE ),
		"Can the buffer be locked with write-only access"
	},
	{
		"GPU_WRITABLE",
		BeCast( Tr2GpuBuffer::GPU_WRITABLE ),
		"Is the buffer used for GPU write access"
	},
	{
		"DRAW_INDIRECT",
		BeCast( Tr2GpuBuffer::DRAW_INDIRECT ),
		"Is the buffer used for indirect draw calls"
	},
	{ 0 }
};

BLUE_REGISTER_ENUM_EX( 
	"Tr2GpuBufferFlags", 
	Tr2GpuBuffer::CreationFlag, 
	Tr2GpuBufferCreationFlagChooser, 
	ENUM_REG_ENUM_OBJECT_ON_MODULE );

const Be::ClassInfo* Tr2GpuBuffer::ExposeToBlue()
{
    EXPOSURE_BEGIN( Tr2GpuBuffer, "" )

		MAP_INTERFACE( INotify )
		MAP_INTERFACE( IInitialize )
		MAP_INTERFACE( ITr2GpuBuffer )
		MAP_INTERFACE( Tr2GpuBuffer )

		MAP_METHOD_AND_WRAP_OPTIONAL_ARGS(
			"__init__",
			__init__,
			3,
			":param count: number of elements in the buffer\n"
			":param format: buffer pixel format (trinity.PIXEL_FORMAT.foo)\n"
			":param creationFlags: combination of trinity.Tr2GpuBufferFlags\n" )
		MAP_ATTRIBUTE( 
			"count", 
			m_count, 
			"Number of elements in the buffer", 
			Be::READWRITE | Be::PERSIST | Be::NOTIFY )
		MAP_ATTRIBUTE_WITH_CHOOSER(
			"format",
			m_format,
			"Buffer pixel format (trinity.PIXEL_FORMAT.foo)",
			Be::READWRITE | Be::ENUM | Be::PERSIST | Be::NOTIFY, 
			Tr2RenderContextEnum_PixelFormat_Chooser )
		MAP_ATTRIBUTE_WITH_CHOOSER( 
			"creationFlags", 
			m_creationFlags, 
			"Miscellaneous for creating a buffer", 
			Be::READWRITE | Be::PERSIST | Be::NOTIFY, 
			Tr2GpuBufferCreationFlagChooser )
		MAP_PROPERTY_READONLY( 
			"isValid",
			IsValid,
			"Is the underlying GPU object valid" )
	EXPOSURE_END()
}
