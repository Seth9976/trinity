#pragma once
#ifndef Tr2MayaScalarCurve_h
#define Tr2MayaScalarCurve_h
#include "Tr2MayaAnimationEngine.h"
#include "include/ITriFunction.h"
#include "include/ITriCurveLength.h"

#if BLUE_WITH_PYTHON

BLUE_CLASS( Tr2MayaScalarCurve ):
	public ITriScalarFunction,
	public IInitialize,
	public ITriCurveLength,
	public INotify
{
public:
	EXPOSE_TO_BLUE();
	Tr2MayaScalarCurve( IRoot* lockobj = NULL );

	//////////////////////////////////////////////////////////////////////////
	// ITriScalarFunction
	virtual void UpdateValue( double time );
	virtual float Update( Be::Time time );
	virtual float Update( double time );
	virtual float GetValueAt( Be::Time time );
	virtual float GetValueAt( double time );
	virtual void ScaleTime( float s );

	//////////////////////////////////////////////////////////////////////////
	// IInitialize
	bool Initialize();

	//////////////////////////////////////////////////////////////////////////
	// INotify
	virtual bool OnModified( Be::Var* value );

	//////////////////////////////////////////////////////////////////////////
	// ITriCurveLength
	float Length() { return m_length; }

	// Accessors
	void	setIndex( int index );
	int		getIndex( void ) const { return m_index; }	
private:

	int							m_index;
	Tr2MayaAnimationEnginePtr	m_animationEngine;
	float						m_length;
	float						m_value;
	float						m_scale;
	std::string					m_name;
	void		ComputeLength();
};

TYPEDEF_BLUECLASS( Tr2MayaScalarCurve );

#endif

#endif //Tr2MayaScalarCurve_h
