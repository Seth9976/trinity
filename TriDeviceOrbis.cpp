////////////////////////////////////////////////////////////////////////////////
//
// Created:		September 2013
// Copyright:	CCP 2013
//

#include "StdAfx.h"
#include "TriDevice.h"

#ifdef __ORBIS__

bool TriDevice::DeviceExists()
{
	USE_MAIN_THREAD_RENDER_CONTEXT();
	return renderContext.IsValid();
}

void TriDevice::UpdateCursor()
{
}

bool TriDevice::DeviceSupportsVertexTexture()
{
	return true;
}

void TriDevice::ApplicationActivated( ApplicationActivation activated )
{
}

void TriDevice::DoShowCursor( bool show )
{
}

void TriDevice::HandleRenderTick( Be::Time timestamp )
{
	static bool isFirstFrame = true;

	USE_MAIN_THREAD_RENDER_CONTEXT();

	if( m_renderJobs )
	{
		m_renderJobs->RunUpdate( timestamp );
	}

	m_postUpdateCallbacks->Update();

	if( !isFirstFrame )
	{
		CR_RETURN( renderContext.Present() );
	}

	Render();
	isFirstFrame = false;
}

#endif