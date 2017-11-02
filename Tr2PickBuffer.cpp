#include "StdAfx.h"
#include "Tr2PickBuffer.h"
#include "Tr2Renderer.h"

using namespace Tr2RenderContextEnum;

// ------------------------------------------------------------------------------------------------------
Tr2PickBuffer::Tr2PickBuffer( IRoot* lockobj, Tr2RenderContextEnum::PixelFormat format, int size ) :
	m_size( size ),
	m_format( format ),
	m_clearColor( 0xFFFFFFFF ),
	m_oldDepthState( 0 )
{
	m_pickTarget.SetHintLockOften();
}

// ------------------------------------------------------------------------------------------------------
Tr2PickBuffer::~Tr2PickBuffer()
{
	ReleaseResources( TRISTORAGE_ALL );
}

// ------------------------------------------------------------------------------------------------------
void Tr2PickBuffer::SetSize( int size )
{
	ReleaseResources( TRISTORAGE_ALL );
	m_size = size;
}

// ------------------------------------------------------------------------------------------------------
void Tr2PickBuffer::ReleaseResources( TriStorage s )
{
	m_pickTarget.Destroy();
	m_depthBuffer.Destroy();
}

// ------------------------------------------------------------------------------------------------------
bool Tr2PickBuffer::OnPrepareResources()
{
    int const bufferWidth( ( m_size > 0 )? m_size : Tr2Renderer::GetRenderTargetWidth() );
    int const bufferHeight( ( m_size > 0 )? m_size : Tr2Renderer::GetRenderTargetHeight() );

	// create the pixel buffer as a rendertarget
	USE_MAIN_THREAD_RENDER_CONTEXT();
	CR( m_pickTarget.Create( bufferWidth, bufferHeight, 1, m_format, Tr2MsaaDesc(), 0, EX_NONE, renderContext ) );
	CR( m_depthBuffer.Create( bufferWidth, bufferHeight, Tr2RenderContextEnum::DSFMT_D24S8, Tr2MsaaDesc(), EX_NONE, renderContext ) );

	return true;
}

// ------------------------------------------------------------------------------------------------------
bool Tr2PickBuffer::BeginRendering( float initialDepth, Tr2RenderContext& renderContext )
{
	if( !m_pickTarget.IsValid() )
	{
		// This could happen if device is lost
		return false;
	}

	Tr2Renderer::PushRenderTarget( m_pickTarget, renderContext );
	Tr2Renderer::PushDepthStencilBuffer( m_depthBuffer, renderContext );

	// buffer z state
	CR( renderContext.GetRenderState( RS_ZENABLE, &m_oldDepthState ) );	
	CR( renderContext.SetRenderState( RS_ZENABLE, TRUE ) );

	CR( renderContext.Clear( CLEARFLAGS_TARGET | CLEARFLAGS_ZBUFFER, m_clearColor, initialDepth ) );

    Tr2Renderer::SetFullScreenViewport();

	return true;
}

// ------------------------------------------------------------------------------------------------------
bool Tr2PickBuffer::EndRendering( Tr2RenderContext& renderContext )
{
	CR( renderContext.SetRenderState( RS_ZENABLE, m_oldDepthState ) );

    //D3DXSaveSurfaceToFile( "c:/test.dds", D3DXIFF_DDS, m_pickTarget, 0, 0 );
	Tr2Renderer::PopDepthStencilBuffer( renderContext );
	Tr2Renderer::PopRenderTarget( renderContext );

	return true;
}

// ------------------------------------------------------------------------------------------------------
bool Tr2PickBuffer::PrepareGetResults( void*& data, uint32_t& pitch, Tr2RenderContext& renderContext )
{
	if( !m_pickTarget.IsValid() )
	{
		// This could happen if device is lost
		return false;
	}

	HRESULT hr = m_pickTarget.Lock( 0, nullptr, data, pitch, renderContext );

	return SUCCEEDED( hr );
}

void Tr2PickBuffer::UnlockBuffer( Tr2RenderContext& renderContext )
{
	if( !m_pickTarget.IsValid() )
	{
		// This could happen if device is lost
		return;
	}
	m_pickTarget.Unlock( renderContext );
}
