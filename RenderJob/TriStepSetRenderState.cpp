#include "StdAfx.h"
#include "TriStepSetRenderState.h"
#include "Tr2Renderer.h"

TriStepSetRenderState::TriStepSetRenderState( IRoot* lockobj )
{
}

TriStepResult TriStepSetRenderState::Execute( Be::Time realTime, Be::Time simTime, Tr2RenderContext& renderContext )
{
	renderContext.SetRenderState( m_state, m_value );
	return RS_OK;
}

void TriStepSetRenderState::SetStateAndValue( unsigned int state, unsigned int value )
{
	m_state = static_cast<Tr2RenderContextEnum::RenderState>( state );
	m_value = value;
}