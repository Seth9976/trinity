////////////////////////////////////////////////////////////
//
//    Created:   August 2010
//    Copyright: CCP 2010
//

#pragma once
#ifndef TriStepSetVisibilityResults_h
#define TriStepSetVisibilityResults_h


#include "TriRenderStep.h"
#include "ITr2VisibilityQueryable.h"
#include "Tr2VisibilityResults.h"

BLUE_DECLARE( Tr2VisibilityResults );

// -------------------------------------------------------------
// Description:
//   TriStepVisibilityResults is a render step that assigns 
//   already generated visibility results to a queryable scene.
// SeeAlso:
//   TriRenderStep
// -------------------------------------------------------------
BLUE_CLASS( TriStepSetVisibilityResults ) : public TriRenderStep
{
public:
	EXPOSE_TO_BLUE();

	TriStepSetVisibilityResults( IRoot* lockobj = 0 );
	~TriStepSetVisibilityResults(void);

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

TYPEDEF_BLUECLASS( TriStepSetVisibilityResults);

#endif // TriStepSetVisibilityResults_h