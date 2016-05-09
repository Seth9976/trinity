#include "StdAfx.h"
#include "Tr2ImageIOHelpers.h"
#include "cairo.h"
#include "cairo-script-interpreter.h"

using namespace Tr2RenderContextEnum;

namespace
{

bool CreateCubeTexture(	ImageIO::HostBitmap& bitmap, Tr2TextureAL &out, 
											uint32_t &memoryUse, 
											Tr2PrimaryRenderContext& renderContext )
{
	CCP_STATS_ZONE( __FUNCTION__ );

	if( !bitmap.IsValid() )
	{
		return false;
	}

	const unsigned trueMipLevelCount = bitmap.GetTrueMipCount();
	
	std::vector<Tr2SubresourceData> initData;

	for( unsigned face = 0; face != 6 ; ++face )
	{		
		for( unsigned i = 0; i != trueMipLevelCount; ++i )
		{
			Tr2SubresourceData srd;
			srd.m_sysMem			= const_cast<char*>( bitmap.GetMipRawData( i, CubemapFace( face ) ) );
			srd.m_sysMemSlicePitch	= bitmap.GetMipSize( i );
			srd.m_sysMemPitch		= bitmap.GetMipPitch( i );	

			if( !srd.m_sysMem )
			{
				return false;
			}

			initData.push_back( srd );

			memoryUse += srd.m_sysMemSlicePitch;
		}
	}

	if( FAILED( out.CreateCube( bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetTrueMipCount(), bitmap.GetFormat(), USAGE_IMMUTABLE, &initData[0], renderContext ) ) )
	{
		return false;
	}

	return true;
}

bool CreateVolumeTexture( ImageIO::HostBitmap& bitmap, Tr2TextureAL &out, 
											uint32_t &memoryUse, 
											Tr2PrimaryRenderContext &renderContext )
{
	CCP_STATS_ZONE( __FUNCTION__ );

	if( !bitmap.IsValid() )
	{
		return false;
	}

	BeTimer create;

	const unsigned trueMipLevelCount = bitmap.GetTrueMipCount();

	std::vector<Tr2SubresourceData> initData;

	//
	// Copy the pixels into the locked D3D surface (one copy per mip)
	//
	for( unsigned i = 0; i != trueMipLevelCount; ++i )
	{
		Tr2SubresourceData srd;

		srd.m_sysMem			= const_cast<char*>( bitmap.GetMipRawData( i ) );
		srd.m_sysMemSlicePitch	= bitmap.GetMipSize( i ) / bitmap.GetMipDepth( i );
		srd.m_sysMemPitch		= bitmap.GetMipPitch( i );	

		if( !srd.m_sysMem )
		{
			return false;
		}

		//const unsigned mipDepth = std::max( ih.GetDepth() >> i, 1u );
		//srd.m_sysMemSlicePitch	/= mipDepth;

		initData.push_back( srd );

		memoryUse += bitmap.GetMipSize( i );
	}

	if( FAILED( out.CreateVolume( bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetDepth(), bitmap.GetTrueMipCount(), bitmap.GetFormat(), USAGE_IMMUTABLE, &initData[0], renderContext ) ) )
	{
		return false;
	}

	return true;
}


struct CairoData
{
	cairo_surface_t* surface;
	uint32_t width;
	uint32_t height;
	double originalWidth;
	double originalHeight;
};

cairo_surface_t* SurfaceCreate( void *closure, cairo_content_t, double width, double height, long )
{
	auto data = static_cast<CairoData*>( closure );
	if( data->surface )
	{
		double xscale = data->width / data->originalWidth;
		double yscale = data->height / data->originalHeight;
		auto surface = cairo_image_surface_create( CAIRO_FORMAT_ARGB32, int( width * xscale ), int( height * yscale ) );
		cairo_surface_set_device_scale( surface, xscale, yscale );
		return surface;
	}
	else
	{
		data->originalWidth = width;
		data->originalHeight = height;
		if( data->width == 0 && data->height == 0 )
		{
			data->width = uint32_t( width + 0.5f );
			data->height = uint32_t( height + 0.5f );
		}
		else if( data->width == 0 )
		{
			data->width = uint32_t( width * data->height / height + 0.5f );
		}
		else if( data->height == 0 )
		{
			data->height = uint32_t( width * data->width / height + 0.5f );
		}
		data->surface = cairo_image_surface_create( CAIRO_FORMAT_ARGB32, data->width, data->height );
		double xscale = data->width / data->originalWidth;
		double yscale = data->height / data->originalHeight;
		cairo_surface_set_device_scale( data->surface, xscale, yscale );
		return cairo_surface_reference( data->surface );
	}
}

bool ProtectedRasterize( const char* script, size_t length, CairoData* data )
{
	cairo_script_interpreter_hooks_t hooks = {
		data,
		&SurfaceCreate,
	};

	bool success;

#ifdef _MSC_VER
	__try
#endif
	{
		auto interpreter = cairo_script_interpreter_create();
		cairo_script_interpreter_install_hooks( interpreter, &hooks );
		auto result = cairo_script_interpreter_feed_string( interpreter, script, int( length ) );
		success = result == CAIRO_STATUS_SUCCESS;
		cairo_script_interpreter_finish( interpreter );
		cairo_script_interpreter_destroy( interpreter );
	}
#ifdef _MSC_VER
	__except( EXCEPTION_EXECUTE_HANDLER )
	{ 
		CCP_LOGERR( "Exception caught while rasterizing cairo script" );
	}
#endif
	return success;
}

const BlueAsyncRes::QueryArgument* FindFirstQueryArgumentByName( const BlueAsyncRes::QueryArguments& arguments, const wchar_t* name )
{
	for( auto it = std::begin( arguments ); it != std::end( arguments ); ++it )
	{
		if( it->first == name )
		{
			return &( *it );
		}
	}
	return nullptr;
}

}

namespace Tr2ImageIOHelpers
{

bool Create2DTexture(	ImageIO::HostBitmap& bitmap, Tr2TextureAL &out, 
										uint32_t &memoryUse, 
										Tr2PrimaryRenderContext &renderContext, 
										Tr2RenderContextEnum::BufferUsage usage ) 
{
	CCP_STATS_ZONE( __FUNCTION__ );

	if( !bitmap.IsValid() )
	{
		return false;
	}

	BeTimer create;

	const unsigned trueMipLevelCount = bitmap.GetTrueMipCount();

	std::vector<Tr2SubresourceData> initData( trueMipLevelCount * bitmap.GetArraySize() );

	for( unsigned j = 0; j < bitmap.GetArraySize(); ++j )
	{
		for( unsigned i = 0; i != trueMipLevelCount; ++i )
		{
			Tr2SubresourceData& srd = initData[i + j * trueMipLevelCount];

			srd.m_sysMem			= const_cast<char*>( bitmap.GetMipRawData( i, j ) );
			srd.m_sysMemSlicePitch	= bitmap.GetMipSize( i );
			srd.m_sysMemPitch		= bitmap.GetMipPitch( i );	

			if( !srd.m_sysMem )
			{
				return false;
			}

			memoryUse += srd.m_sysMemSlicePitch;
		}
	}

	if( bitmap.GetArraySize() > 1 )
	{
		if( FAILED( out.Create2DArray( 
			bitmap.GetWidth(), 
			bitmap.GetHeight(), 
			bitmap.GetTrueMipCount(), 
			bitmap.GetArraySize(), 
			bitmap.GetFormat(), 
			usage, 
			&initData[0], 
			renderContext ) ) )
		{
			return false;
		}
	}
	else
	{
		if( FAILED( out.Create2D(	bitmap.GetWidth(), 
									bitmap.GetHeight(), 
									bitmap.GetTrueMipCount(), 
									bitmap.GetFormat(), 
									usage, 
									&initData[0], 
									renderContext ) ) )
		{
			return false;
		}
	}

	return true;
}

bool CreateTexture(	ImageIO::HostBitmap& bitmap, Tr2TextureAL &out, 
										uint32_t &memoryUse, 
										Tr2PrimaryRenderContext &renderContext, 
										Tr2RenderContextEnum::BufferUsage usage )
{
	if( !bitmap.IsValid() )
	{
		return false;
	}

	if( bitmap.GetType() == TEX_TYPE_CUBE )
	{
		return CreateCubeTexture( bitmap, out, memoryUse, renderContext );
	}
	else if( bitmap.GetType() == TEX_TYPE_3D )
	{
		return CreateVolumeTexture( bitmap, out, memoryUse, renderContext );
	}
	else
	{
		return Create2DTexture( bitmap, out, memoryUse, renderContext, usage );
	}
	

	return true;
}

bool CopyToTexture( ImageIO::HostBitmap& bitmap, Tr2TextureAL& texture, unsigned int x, unsigned int y, unsigned int margin, Tr2RenderContext& renderContext )
{
	CCP_STATS_ZONE( __FUNCTION__ );

	if( !bitmap.IsValid() )
	{
		return false;
	}

	if( texture.GetFormat() != bitmap.GetFormat() )
	{
		CCP_LOGERR( "Tr2ImageHandler::CopyToTexture - formats don't match" );
		return false;
	}

	if( x >= texture.GetWidth() || y >= texture.GetHeight() )
	{
		CCP_LOGERR( "Tr2ImageHandler::CopyToTexture - out of bounds" );
		return false;
	}

	if( x + bitmap.GetWidth() > texture.GetWidth() || y + bitmap.GetHeight() > texture.GetHeight() )
	{
		CCP_LOGERR( "Tr2ImageHandler::CopyToTexture - out of bounds" );
		return false;
	}

	if( !margin )
	{
		const auto result = texture.UpdateSubresource( x, y, x + bitmap.GetWidth(), y + bitmap.GetHeight(), bitmap.GetRawData(), bitmap.GetPitch(), renderContext );
		if( FAILED( result ) )
		{
			CCP_LOGERR( "Tr2ImageHandler::CopyToTexture - UpdateSubresource failed [no margin]: %08x", result );
		}
		return SUCCEEDED(result);
	}

	// Can't expect the Hal to support updating a subresource with automatic replication of border pixels, so do this ourselves in a chunk
	// of temporary memory, then send that off to the backend.

	std::vector<unsigned char> pixels;
	unsigned pitch = 0;
	AddMargin( bitmap.GetFormat(), reinterpret_cast<const uint8_t*>( bitmap.GetRawData() ), bitmap.GetWidth(), bitmap.GetHeight(), margin, pixels, pitch );

	const auto result = texture.UpdateSubresource( x, y, x + bitmap.GetWidth() + 2 * margin, y + bitmap.GetHeight() + 2 * margin, &pixels[0], pitch, renderContext );
	if( FAILED( result ) )
	{
		CCP_LOGERR( "Tr2ImageHandler::CopyToTexture - UpdateSubresource failed [margin]: %08x", result );
	}
	return SUCCEEDED(result);
}


void AddMargin(	const Tr2RenderContextEnum::PixelFormat format,
				const unsigned char* source,
				const unsigned width, const unsigned height,
				const unsigned margin,
				std::vector<unsigned char> &output, 
				unsigned &outputPitch )
{
	const unsigned char* src = source;

	if( IsCompressedFormat( format ) )
	{
		const unsigned blockByteSize = Tr2RenderContextEnum::GetBlockByteSize( format );
		const unsigned blockPixelSize = 4;
		CCP_ASSERT( blockByteSize != 0 && margin % blockPixelSize == 0 );
		const unsigned blockMargin	= margin	/ blockPixelSize;
		const unsigned blocksX		= width		/ blockPixelSize;
		const unsigned blocksY		= height	/ blockPixelSize;

		//technically the block contents need to be mirrored in the margin to get nice filtering,
		// but that's somewhat fiddly.
		//textures which are intended to be used in a tiled fashion could also use the block from
		// the opposite edge.
		//maybe we need a usage hint here to inform this decision.
		
		outputPitch = ( blocksX + 2 * blockMargin ) * blockByteSize;
		output.resize( ( blockMargin + blocksY + blockMargin ) * outputPitch );
		unsigned char* dst = &output[0];

		//top margin
		for( unsigned i = 0; i < blockMargin; ++i )
		{
			memcpy( dst + blockMargin * blockByteSize, src, blocksX * blockByteSize );
			dst += outputPitch;
		}

		// Have to copy one line at a time since the target area is not linearly laid out.
		
		for( unsigned line = 0; line < height; line += blockPixelSize )
		{
			//left margin
			for( unsigned i = 0; i != blockMargin; ++i ) 
			{
				memcpy( dst + i * blockByteSize, src, blockByteSize );
			}
			
			memcpy( dst + blockMargin * blockByteSize, src, blocksX * blockByteSize );
			
			//right margin
			for( unsigned i = 0; i != blockMargin; ++i ) 
			{
				memcpy( dst + (blocksX+blockMargin+i) * blockByteSize, src + (blocksX-1) * blockByteSize, blockByteSize );
			}

			if( line < height-1 )
			{
				src += blocksX * blockByteSize;
			}
		}

		//bottom margin
		for( unsigned i = 0; i != blockMargin; ++i )
		{
			memcpy( dst + blockMargin * blockByteSize, src, blocksX * blockByteSize );
			dst += outputPitch;
		}

		CCP_ASSERT( dst == &output[0] + output.size() );
	}
	else
	{
		// Align pixels to bytes.
		const unsigned byteCount = GetBytesPerPixel( format );

		// Align srcPitch to 4 bytes.
		unsigned int srcPitch = 4 * ((width * byteCount + 3) / 4);

		outputPitch = ( width + 2 * margin ) * byteCount;

		output.resize( ( height + 2 * margin ) * outputPitch );
		unsigned char* dst = &output[0];

		//top margin
		for( unsigned i = 0; i != margin; ++i )
		{
			//note that we don't touch the corners - may want to stick debug colours in these texels,
			// at least for margins > 1. Hmm.
			memcpy( dst + margin * byteCount, src, width * byteCount );
			dst += outputPitch;
		}

		// Have to copy one line at a time since the target area is not linearly laid out.
		for( unsigned line = 0; line != height; ++line )
		{
			//left margin
			for( unsigned i = 0; i != margin; ++i ) 
			{
				memcpy( dst + i * byteCount, src, byteCount );
			}

			memcpy( dst + margin * byteCount, src, width * byteCount );
			
			//right margin
			for( unsigned i = 0; i != margin; ++i ) 
			{
				memcpy( dst + ( width + margin + i ) * byteCount, src + ( width-1 ) * byteCount, byteCount );
			}

			if( line < height-1 )
			{
				src += srcPitch;
			}
			dst += outputPitch;
		}

		//bottom margin
		for( unsigned i = 0; i != margin; ++i )
		{
			memcpy( dst + margin * byteCount, src, width * byteCount );
			dst += outputPitch;
		}

		CCP_ASSERT( dst == &output[0] + output.size() );
	}
}

bool IsCairoScriptPath( const wchar_t* path )
{
	auto length = wcslen( path );
    auto ext = path + length - 4;
	return length > 4 && ext[0] == L'.' &&
        ( ext[1] == L'e' || ext[1] == L'E' ) &&
        ( ext[2] == L'c' || ext[2] == L'C' ) &&
        ( ext[3] == L's' || ext[3] == L'S' );
}

bool RasterizeCairoScript( const char* script, size_t length, uint32_t width, uint32_t height, ImageIO::HostBitmap& bitmap )
{
	CCP_STATS_ZONE( __FUNCTION__ );

	std::unique_ptr<CairoData> data( new CairoData );
	data->surface = nullptr;
	data->width = width;
	data->height = height;
	data->originalWidth = width;
	data->originalHeight = height;

	bool success;
	{
		CCP_STATS_ZONE( "Cairo rasterizing" );
		success = ProtectedRasterize( script, length, data.get() );
	}
	if( !data->surface )
	{
		success = false;
	}

	if( success )
	{
		if( bitmap.Create( data->width, data->height, 1, Tr2RenderContextEnum::PIXEL_FORMAT_B8G8R8A8_UNORM ) )
		{
			cairo_surface_flush( data->surface );
			auto src = cairo_image_surface_get_data( data->surface );
			auto srcStride = cairo_image_surface_get_stride( data->surface );
			auto dest = reinterpret_cast<uint8_t*>( bitmap.GetRawData() );
			auto destStride = bitmap.GetPitch();

			// we need to convert cairo's premultiplied alpha image to non-premultiplied alpha
			for( uint32_t j = 0; j < data->height; ++j )
			{
				for( uint32_t i = 0; i < data->width; ++i )
				{
					float a = float( src[3] ) / 255.f;
					if( a )
					{
						dest[0] = uint8_t( src[0] / a );
						dest[1] = uint8_t( src[1] / a );
						dest[2] = uint8_t( src[2] / a );
					}
					else
					{
						dest[0] = src[0];
						dest[1] = src[1];
						dest[2] = src[2];
					}
					dest[3] = src[3];
					dest += 4;
					src += 4;
				}
				src += srcStride - data->width * 4;
				dest += destStride - data->width * 4;
			}
		}
		else
		{
			success = false;
		}
	}
	if( data->surface )
	{
		cairo_surface_destroy( data->surface );
	}
	return success;
}

ImageIO::Result RasterizeCairoScript( IBlueStream* stream, const BlueAsyncRes::QueryArguments& arguments, ImageIO::HostBitmap& bitmap )
{
	if( !stream )
	{
		return ImageIO::Result( ImageIO::Result::INVALID_DATA );
	}

	uint32_t width = 0;
	uint32_t height = 0;

	auto widthArgument = FindFirstQueryArgumentByName( arguments, L"width" );
	if( widthArgument )
	{
		width = uint32_t( wcstol( widthArgument->second.c_str(), nullptr, 10 ) );
	}

	auto heightArgument = FindFirstQueryArgumentByName( arguments, L"height" );
	if( heightArgument )
	{
		height = uint32_t( wcstol( heightArgument->second.c_str(), nullptr, 10 ) );
	}

	CcpMallocBuffer script( "RasterizeCairoScript", stream->GetSize() );
	if( !script.size() )
	{
		CCP_LOGERR( "RasterizeCairoScript: out of memory when reading stream of size %" CCP_SIZET_FORMAT, size_t( stream->GetSize() ) );
		return ImageIO::Result( ImageIO::Result::OUT_OF_MEMORY );
	}
	if( stream->Read( script.get(), script.size() ) != script.size() )
	{
		CCP_LOGERR( "RasterizeCairoScript: failed to read file" );
		return ImageIO::Result( ImageIO::Result::READ_FAILURE );
	}

	if( !RasterizeCairoScript( script.get(), script.size(), width, height, bitmap ) )
	{
		return ImageIO::Result( ImageIO::Result::INVALID_DATA );
	}
	return ImageIO::Result( ImageIO::Result::OK );
}

}
