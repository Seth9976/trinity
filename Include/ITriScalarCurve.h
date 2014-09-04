/* 
	*************************************************************************

	ITriScalarCurve.h

	Author:    Hilmar Veigar Pétursson
	Created:   November 2000
	OS:        Win32
	Project:   Trinity

	Description:   

		Yeap


	Dependencies:

		DirectX 9.0, Probably more, ytbd.

	(c) CCP 2000

	*************************************************************************
*/

#ifndef _ITRISCALARCURVE_H_
#define _ITRISCALARCURVE_H_

#include "ITriFunction.h"

BLUE_INTERFACE(ITriScalarCurve) : public ITriScalarFunction
{	
	virtual void AddKey(
		float t, 
		float v, 
		float l,  
		float r, 
		TRIINTERPOLATION i
		) = 0;
	
	virtual void Sort(
		) = 0;
	
	virtual void ScaleValue(
		float s
		) = 0;

	virtual float Length(
		) = 0;
};

#endif