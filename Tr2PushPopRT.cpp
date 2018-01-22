#include "StdAfx.h"
#include "Tr2PushPopRT.h"
#include "Tr2Renderer.h"

Tr2PushPopRT::Tr2PushPopRT( uint32_t slot )
: m_renderContext( nullptr )
, m_slot( slot )
{
	USE_MAIN_THREAD_RENDER_CONTEXT();
	Tr2Renderer::PushRenderTarget( slot, renderContext );
}

Tr2PushPopRT::Tr2PushPopRT( const Tr2TextureAL& rt, uint32_t slot )
: m_renderContext( nullptr )
, m_slot( slot )
{
	USE_MAIN_THREAD_RENDER_CONTEXT();
	Tr2Renderer::PushRenderTarget( rt, slot, renderContext );
}

Tr2PushPopRT::Tr2PushPopRT( Tr2RenderContext& renderContext, uint32_t slot )
: m_renderContext( &renderContext )
, m_slot( slot )
{
	Tr2Renderer::PushRenderTarget( slot, renderContext );
}

Tr2PushPopRT::Tr2PushPopRT( const Tr2TextureAL& rt, Tr2RenderContext& renderContext, uint32_t slot )
: m_renderContext( &renderContext )
, m_slot( slot )
{
	Tr2Renderer::PushRenderTarget( rt, slot, renderContext );
}

Tr2PushPopRT::~Tr2PushPopRT()
{
	if( m_renderContext )
	{
		Tr2Renderer::PopRenderTarget( m_slot, *m_renderContext );
	}
	else
	{
		USE_MAIN_THREAD_RENDER_CONTEXT();
		Tr2Renderer::PopRenderTarget( m_slot, renderContext );
	}
}
