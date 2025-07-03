#include "StdAfx.h"
#include "../include/Tr2WeakTextureAL.h"

#include TRINITY_AL_PLATFORM_INCLUDE( Tr2TextureAL )



Tr2WeakTextureAL::Tr2WeakTextureAL() :
	m_texture( Tr2TextureAL::NullTexture() )
{
}

Tr2WeakTextureAL::Tr2WeakTextureAL( Tr2TextureAL other )
{
	m_texture = other.m_texture;
}

bool Tr2WeakTextureAL::IsValid() const
{
	auto ptr = m_texture.lock();
	return ptr && ptr->IsValid();
}

Tr2ALMemoryType Tr2WeakTextureAL::GetMemoryClass()
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GetMemoryClass();
}

const Tr2BitmapDimensions& Tr2WeakTextureAL::GetDesc() const
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GetDesc();
}

const Tr2MsaaDesc& Tr2WeakTextureAL::GetMsaaDesc() const
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GetMsaaDesc();
}

Tr2GpuUsage::Type Tr2WeakTextureAL::GetGpuUsage() const
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GetGpuUsage();
}

Tr2CpuUsage::Type Tr2WeakTextureAL::GetCpuUsage() const
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GetCpuUsage();
}

uint32_t Tr2WeakTextureAL::GetWidth() const
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GetDesc().GetWidth();
}

uint32_t Tr2WeakTextureAL::GetHeight() const
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GetDesc().GetHeight();
}

uint32_t Tr2WeakTextureAL::GetDepth() const
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GetDesc().GetDepth();
}

uint32_t Tr2WeakTextureAL::GetMipCount() const
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GetDesc().GetMipCount();
}

uint32_t Tr2WeakTextureAL::GetTrueMipCount() const
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GetDesc().GetTrueMipCount();
}

Tr2RenderContextEnum::PixelFormat Tr2WeakTextureAL::GetFormat() const
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GetDesc().GetFormat();
}

Tr2RenderContextEnum::TextureType Tr2WeakTextureAL::GetType() const
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GetDesc().GetType();
}

uint32_t Tr2WeakTextureAL::GetArraySize() const
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GetDesc().GetArraySize();
}

uint32_t Tr2WeakTextureAL::GetMipSize( uint32_t mip ) const
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GetDesc().GetMipSize( mip );
}

bool Tr2WeakTextureAL::operator==( const Tr2TextureAL& other ) const
{
	return m_texture.lock() == other.m_texture;
}

bool Tr2WeakTextureAL::operator==( const Tr2WeakTextureAL& other ) const
{
	return m_texture.lock() == other.m_texture.lock();
}

ALResult Tr2WeakTextureAL::MapForReading( const Tr2TextureSubresource& region, const void*& data, uint32_t& pitch, Tr2RenderContextAL& renderContext )
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->MapForReading( region, data, pitch, renderContext );
}

void Tr2WeakTextureAL::UnmapForReading( Tr2RenderContextAL& renderContext )
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	ptr->UnmapForReading( renderContext );
}

ALResult Tr2WeakTextureAL::MapForWriting( const Tr2TextureSubresource& region, void*& data, uint32_t& pitch, Tr2RenderContextAL& renderContext )
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->MapForWriting( region, data, pitch, renderContext );
}

void Tr2WeakTextureAL::UnmapForWriting( Tr2RenderContextAL& renderContext )
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	ptr->UnmapForWriting( renderContext );
}

ALResult Tr2WeakTextureAL::UpdateSubresource( const Tr2TextureSubresource& region, const void* source, uint32_t pitch, uint32_t slicePitch, Tr2RenderContextAL& renderContext )
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->UpdateSubresource( region, source, pitch, slicePitch, renderContext );
}

ALResult Tr2WeakTextureAL::CopySubresourceRegion( const Tr2TextureSubresource& destSubresource, Tr2TextureAL& source, const Tr2TextureSubresource& sourceSubresource, Tr2RenderContextAL& renderContext )
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->CopySubresourceRegion( destSubresource, *source.m_texture, sourceSubresource, renderContext );
}

namespace
{
bool FormatIsBGR( Tr2RenderContextEnum::PixelFormat format )
{
	switch( format )
	{
	case Tr2RenderContextEnum::PIXEL_FORMAT_B8G8R8A8_UNORM:
	case Tr2RenderContextEnum::PIXEL_FORMAT_B8G8R8X8_UNORM:
	case Tr2RenderContextEnum::PIXEL_FORMAT_B8G8R8A8_UNORM_SRGB:
	case Tr2RenderContextEnum::PIXEL_FORMAT_B8G8R8X8_UNORM_SRGB:
		return true;
	default:
		return false;
	}
}
}

ALResult Tr2WeakTextureAL::GenerateMipMaps( Tr2RenderContextAL& renderContext )
{
	if( !IsValid() )
	{
		return E_INVALIDCALL;
	}
	if( GetTrueMipCount() <= 1 )
	{
		return S_OK;
	}
	if( FormatIsBGR( GetFormat() ) )
	{
		// ATM we don't support BGR formats in DirectX12 and don't seem to be using them with GenerateMipMaps
		// so we just fail here. If needed the support can be added later.
		return E_INVALIDCALL;
	}

	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GenerateMipMaps( renderContext );
}

ALResult Tr2WeakTextureAL::Resolve( Tr2TextureAL& destination, Tr2RenderContextAL& renderContext )
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->Resolve( *destination.m_texture, renderContext );
}

uintptr_t Tr2WeakTextureAL::GetSharedHandle() const
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GetSharedHandle();
}

uint32_t Tr2WeakTextureAL::GetSrvIndexInHeap( Tr2RenderContextEnum::ColorSpace colorSpace ) const
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GetSrvIndexInHeap( colorSpace );
}

uint32_t Tr2WeakTextureAL::GetUavIndexInHeap( uint32_t mip ) const
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->GetUavIndexInHeap( mip );
}

ALResult Tr2WeakTextureAL::SetName( const char* name )
{
	if( !IsValid() )
	{
		return E_INVALIDCALL;
	}
	if( !name )
	{
		return E_INVALIDARG;
	}
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr->SetName( name );
}

TrinityALImpl::Tr2TextureAL* Tr2WeakTextureAL::TrinityALImpl_GetObject() const
{
	auto ptr = m_texture.lock();
	CCP_ASSERT_M( ptr, "Tr2WeakTextureAL does not hold a valid pointer!" );
	return ptr.get();
}


namespace
{
void ConvertUsage( Tr2RenderContextEnum::BufferUsage usage, Tr2GpuUsage::Type& gpuUsage, Tr2CpuUsage::Type& cpuUsage )
{
	gpuUsage = Tr2GpuUsage::SHADER_RESOURCE;
	cpuUsage = Tr2CpuUsage::NONE;

	if( ( usage & Tr2RenderContextEnum::USAGE_IMMUTABLE ) != 0 )
	{
		cpuUsage = cpuUsage | Tr2CpuUsage::READ;
	}
	else if( ( usage & Tr2RenderContextEnum::USAGE_LOCK_FREQUENTLY ) != 0 )
	{
		cpuUsage = cpuUsage | Tr2CpuUsage::WRITE_OFTEN;
	}
	else
	{
		cpuUsage = cpuUsage | Tr2CpuUsage::WRITE;
	}
	if( ( usage & Tr2RenderContextEnum::USAGE_CPU_READ ) != 0 )
	{
		cpuUsage = cpuUsage | Tr2CpuUsage::READ;
	}
	if( ( usage & Tr2RenderContextEnum::USAGE_UNORDERED_ACCESS ) != 0 )
	{
		gpuUsage = gpuUsage | Tr2GpuUsage::UNORDERED_ACCESS;
	}
	if( ( usage & Tr2RenderContextEnum::USAGE_SHADER_RESOURCE ) != 0 )
	{
		gpuUsage = gpuUsage | Tr2GpuUsage::SHARED;
	}
}
}
