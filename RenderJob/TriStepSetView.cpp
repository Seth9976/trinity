#include "StdAfx.h"
#include "TriStepSetView.h"
#include "Tr2Renderer.h"

TriStepSetView::TriStepSetView( IRoot* lockobj )
{
}

TriStepSetView::~TriStepSetView(void)
{
}

void TriStepSetView::SetViewCameraParent( TriView* view, EveCamera* camera, ITriTransform* cameraParent )
{
	m_view = view;
	m_camera = camera;
	m_cameraParent = cameraParent;
}

TriStepResult TriStepSetView::Execute( Be::Time realTime, Be::Time simTime, Tr2RenderContext& renderContext )
{
	if( m_view )
	{
		Tr2Renderer::SetViewTransform( m_view->GetTransform() );
	}
	else if( m_camera )
	{
		m_camera->Update( simTime );
		Tr2Renderer::SetViewTransform( m_camera->GetViewMatrix()->GetTransform() );
	}
	return RS_OK;
}