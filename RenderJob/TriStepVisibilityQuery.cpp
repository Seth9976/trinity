#include "StdAfx.h"
#include "TriStepVisibilityQuery.h"

TriStepVisibilityQuery::TriStepVisibilityQuery( IRoot* lockobj )
{
}

TriStepVisibilityQuery::~TriStepVisibilityQuery(void)
{
}

// --------------------------------------------------------------------------------------
// Description:
//   Blue-exposed initializer. 
// --------------------------------------------------------------------------------------
void TriStepVisibilityQuery::py__init__( ITr2VisibilityQueryable* queryable, Tr2VisibilityResults* results )
{
	SetQueryScene( queryable );
	SetResultsObject( results );
}

TriStepResult TriStepVisibilityQuery::Execute( Be::Time realTime, Be::Time simTime, Tr2RenderContext& renderContext )
{
	if( m_queryable )
	{
		m_queryable->VisibilityQuery( m_results );
	}
	return RS_OK;
}