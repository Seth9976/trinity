#include "StdAfx.h"

#include "TriStepRenderCubeMap.h"
#include "TriViewport.h"
#include "Tr2Renderer.h"

using namespace Tr2RenderContextEnum;

TriStepRenderCubeMap::TriStepRenderCubeMap( IRoot* lockobj )
{
	// pre-calc nice positions for the sides to be displayed
	m_sidePos[0].x = 1; m_sidePos[0].y = 1;
	m_sidePos[5].x = 2; m_sidePos[5].y = 1;
	m_sidePos[1].x = 3; m_sidePos[1].y = 1;
	m_sidePos[4].x = 0; m_sidePos[4].y = 1;
	m_sidePos[2].x = 0; m_sidePos[2].y = 0;
	m_sidePos[3].x = 0; m_sidePos[3].y = 2;

	// and some names, which are depending on the handedness( pos x <-> neg x )
	if( Tr2Renderer::IsRightHanded() )
	{
		m_sideDesc[0] = "NEG X";
		m_sideDesc[1] = "POS X";
		m_sideDesc[2] = "POS Y";
		m_sideDesc[3] = "NEG Y";
		m_sideDesc[4] = "POS Z";
		m_sideDesc[5] = "NEG Z";
	}
	else
	{
		m_sideDesc[0] = "POS X";
		m_sideDesc[1] = "NEG X";
		m_sideDesc[2] = "POS Y";
		m_sideDesc[3] = "NEG Y";
		m_sideDesc[4] = "POS Z";
		m_sideDesc[5] = "NEG Z";
	}
}

TriStepRenderCubeMap::~TriStepRenderCubeMap(void)
{
}

void TriStepRenderCubeMap::py__init__( TriTextureRes* tex )
{
	m_texture = tex;
}

TriStepResult TriStepRenderCubeMap::Execute( Be::Time realTime, Be::Time simTime, Tr2RenderContext& renderContext )
{
	if( m_texture && m_texture->GetTexture() && m_texture->GetType() == Tr2RenderContextEnum::TEX_TYPE_CUBE )
	{
		// backup current viewport, we need to change it to blit to different window regions
		Tr2Renderer::PushViewport();

		// use new viewport to blit to different positions
		CTriViewport currentViewport;

		// draw all mip-levels of this cube's side
		unsigned int mipOffsetX = 0, mipOffsetY = 0;

		unsigned scale = 0;
		unsigned size = m_texture->GetWidth() / 512;
		while( size )
		{
			size /= 2;
			++scale;
		}

		for( unsigned int mip = 0; mip < m_texture->GetMipLevelCount(); ++mip )
		{
			const unsigned shift = mip + scale;

			// draw all sixe sides of cube
			for( unsigned int side = 0; side < 6; ++side )
			{
				// draw them in a nice order
				currentViewport.height = currentViewport.width = m_texture->GetWidth() >> shift;
				currentViewport.x = (m_texture->GetWidth() * m_sidePos[side].x) >> shift;
				currentViewport.y = (m_texture->GetWidth() * m_sidePos[side].y) >> shift;
				currentViewport.x += mipOffsetX;
				currentViewport.y += mipOffsetY;
				Tr2Renderer::SetViewport( currentViewport );

				// draw on screen
				Tr2Renderer::DrawCubeTexture( *m_texture->GetTexture(), (CubemapFace)side, mip );

				// debug helper text
				if( mip == 0 )
				{
					Rect rc;
                    rc.left = ( m_texture->GetWidth() * m_sidePos[side].x ) >> shift;
					rc.top = ( m_texture->GetWidth() * m_sidePos[side].y ) >> shift;
					rc.right = ( m_texture->GetWidth() * ( m_sidePos[side].x + 1 ) ) >> shift;
					rc.bottom = ( m_texture->GetWidth() * ( m_sidePos[side].y + 1 ) ) >> shift;
					Tr2Renderer::PrintfImmediate( TRI_DBG_FONT_LARGE, rc, TRI_DFS_LEFT, 0xffff0000, m_sideDesc[side].c_str() );

					rc.left++; rc.right++; rc.top++; rc.bottom++;
					Tr2Renderer::PrintfImmediate( TRI_DBG_FONT_LARGE, rc, TRI_DFS_LEFT, 0xff000000, m_sideDesc[side].c_str() );
				}
            }
			// offset to "arrange" miplevels on screen
			mipOffsetX += currentViewport.width + 2;
			mipOffsetY += 2 * currentViewport.height + 2;
		}

		// be nice and restore old viweport
		Tr2Renderer::PopViewport();
	}
	return RS_OK;
}

