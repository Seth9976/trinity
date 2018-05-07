////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#pragma once

#include "ITr2ControllerAction.h"


BLUE_CLASS( Tr2ActionPlayCurveSet ) : public ITr2ControllerAction
{
public:
	Tr2ActionPlayCurveSet( IRoot* lockobj = nullptr );

	EXPOSE_TO_BLUE();

	virtual void Start( Tr2Controller& controller );
	virtual void Stop( Tr2Controller& controller );
private:
	std::string m_curveSetName;
	float m_startTime;
	float m_endTime;
	bool m_range;
	bool m_looped;
};

TYPEDEF_BLUECLASS( Tr2ActionPlayCurveSet );
