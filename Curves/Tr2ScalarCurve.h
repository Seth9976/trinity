#pragma once
#ifndef Tr2ScalarCurve_h
#define Tr2ScalarCurve_h

// Trinity headers

#include "include/Tr2Curve.h"

// --------------------------------------------------------------------------------------
// Description:
//   Tr2ScalarKey is a specialization of Tr2Key for floats (scalars).
// SeeAlso:
//   Tr2Key, Tr2ScalarCurve
// --------------------------------------------------------------------------------------
BLUE_CLASS( Tr2ScalarKey ):
	public IRoot,
	public Tr2Key<float>
{
public:
	// Constructor
	Tr2ScalarKey( IRoot* lockobj = NULL );

	EXPOSE_TO_BLUE();

	// Left and right tangents
	float m_leftTangent;
	float m_rightTangent;
};
BLUE_DECLARE_VECTOR( Tr2ScalarKey );


// --------------------------------------------------------------------------------------
// Description:
//   Tr2ScalarCurve is a specialization of Tr2Curve for interpolating between float 
//   (scalar) keys.
// SeeAlso:
//   Tr2Curve, Tr2ScalarKey
// --------------------------------------------------------------------------------------
BLUE_CLASS( Tr2ScalarCurve ) :
	public Tr2CurveBase<Tr2ScalarKey, PTr2ScalarKeyVector, float>,
	public ITriScalarFunction
{
public:
	// Constructor
	Tr2ScalarCurve( IRoot* lockobj = NULL );

	EXPOSE_TO_BLUE();

	//////////////////////////////////////////////////////////////////////////
	// ITriScalarFunction
	virtual void UpdateValue( double time );
	virtual float Update( Be::Time time );
	virtual float Update( double time );
	virtual float GetValueAt( Be::Time time );
	virtual float GetValueAt( double time );
	virtual void ScaleTime( float s );

	// Gets left tangent for a given key index
	float GetKeyLeftTangent( unsigned int idx ) const;
	// Sets left tangent for a given key index
	void SetKeyLeftTangent( unsigned int idx, float tangent );

	// Gets right tangent for a given key index
	float GetKeyRightTangent( unsigned int idx ) const;
	// Sets right tangent for a given key index
	void SetKeyRightTangent( unsigned int idx, float tangent );

	// Sort keys into ascending order by key time
	void Sort();

	// Interpolate between scalar key values
	float* Interpolate( float* out, Tr2ScalarKey* lastKey, Tr2ScalarKey* nextKey );

	// Start and end tangents
	float m_startTangent;
	float m_endTangent;

private:
	// Virtual function to add a scalar key at a specific time
	void AddKey_( float time, const float& value );
};

TYPEDEF_BLUECLASS( Tr2ScalarCurve );
TYPEDEF_BLUECLASS( Tr2ScalarKey );

#endif //Tr2ScalarCurve_h
