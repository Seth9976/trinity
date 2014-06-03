////////////////////////////////////////////////////////////
//
//    Created:   August 2010
//    Copyright: CCP 2010
//

#include "StdAfx.h"
#include "TriStepSetVisibilityResults.h"

TriStepSetVisibilityResults::TriStepSetVisibilityResults( IRoot* lockobj )
{
}

TriStepSetVisibilityResults::~TriStepSetVisibilityResults(void)
{
}

// --------------------------------------------------------------------------------------
// Description:
//   Blue-exposed initializer. 
// --------------------------------------------------------------------------------------
void TriStepSetVisibilityResults::py__init__( ITr2VisibilityQueryable* queryable, Tr2VisibilityResults* results )
{
	SetQueryScene( queryable );
	SetResultsObject( results );
}

TriStepResult TriStepSetVisibilityResults::Execute( Be::Time realTime, Be::Time simTime, Tr2RenderContext& renderContext )
{
	if( m_queryable )
	{
		m_queryable->SetVisibilityResults( m_results );
	}
	return RS_OK;
}