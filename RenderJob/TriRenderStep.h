#pragma once
#ifndef _TRIRENDERSTEP_H_
#define _TRIRENDERSTEP_H_



enum TriStepResult
{
	RS_OK,
	RS_FAILED,
	RS_IN_PROGRESS,
	RS_TERMINATE
};

class Tr2RenderContext;

BLUE_CLASS( TriRenderStep ) : public IRoot
{
public:
	EXPOSE_TO_BLUE();

	TriRenderStep( IRoot* lockobj = NULL );
	virtual ~TriRenderStep();

	bool IsEnabled() const;
	virtual TriStepResult Execute( Be::Time realTime, Be::Time simTime, Tr2RenderContext& renderContext ) = 0;

protected:
	std::string m_name;
	// Enabled/disabled flag: disabled steps are not executed
	bool m_enabled;
};

BLUE_DECLARE_VECTOR( TriRenderStep );
TYPEDEF_BLUECLASS( TriRenderStep );

#endif