#pragma once
#ifndef _TRISTEPVISIBILITYQUERY_H_
#define _TRISTEPVISIBILITYQUERY_H_


#include "TriRenderStep.h"
#include "ITr2VisibilityQueryable.h"
#include "Tr2VisibilityResults.h"

BLUE_DECLARE( Tr2VisibilityResults );

BLUE_CLASS( TriStepVisibilityQuery ) : public TriRenderStep
{
public:
	EXPOSE_TO_BLUE();

	TriStepVisibilityQuery( IRoot* lockobj = 0 );
	~TriStepVisibilityQuery(void);

	void py__init__( ITr2VisibilityQueryable* queryable, Tr2VisibilityResults* results );

	// RenderStep
	TriStepResult Execute( Be::Time realTime, Be::Time simTime, Tr2RenderContext& renderContext );

	void SetQueryScene( ITr2VisibilityQueryable* queryable )
	{
		m_queryable = queryable;
	}
	void SetResultsObject( Tr2VisibilityResults* results )
	{
		m_results = results;
	}

private:
	ITr2VisibilityQueryablePtr m_queryable;
	Tr2VisibilityResultsPtr m_results;
};

TYPEDEF_BLUECLASS( TriStepVisibilityQuery);

#endif