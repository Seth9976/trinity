#include "StdAfx.h"
#include "Tr2ScalarCurve.h"

// --------------------------------------------------------------------------------------
// Description:
//   Scalar key comparison function.
// Arguments:
//   context - ignored
//   a - First scalar key comparison operand
//   b - Second scalar key comparison operand
// Return Value:
//   true, if key a's time is less that key b's time
//   false, otherwise
// --------------------------------------------------------------------------------------
static bool CompareKeys( IRoot* context, Tr2ScalarKey* a, Tr2ScalarKey* b )
{
	return a->m_time < b->m_time;
}



// --------------------------------------------------------------------------------------
// Description:
//   Default constructor.
// --------------------------------------------------------------------------------------
Tr2ScalarKey::Tr2ScalarKey( IRoot* lockobj ) 
{
	m_value = 0.0f;
	m_time = 0.0f;
	m_leftTangent = 0.0f;
	m_rightTangent = 0.0f;
	m_interpolation = LINEAR;
}



// --------------------------------------------------------------------------------------
// Description:
//   Default constructor.
// --------------------------------------------------------------------------------------
Tr2ScalarCurve::Tr2ScalarCurve( IRoot* lockobj ) : 
	Tr2CurveBase<Tr2ScalarKey, PTr2ScalarKeyVector, float>( lockobj )
{
	m_startValue = 0.0f;
	m_endValue = 0.0f;
	m_startTangent = 0.0f;
	m_endTangent = 0.0f;
	m_currentValue = 0.0f;
}

void Tr2ScalarCurve::UpdateValue( double time )
{
	m_currentValue = GetValue( time );
}

float Tr2ScalarCurve::Update( Be::Time time )
{
	return m_currentValue = GetValue( TimeAsDouble( time ) );
}

float Tr2ScalarCurve::Update( double time )
{
	return m_currentValue = GetValue( time );
}

float Tr2ScalarCurve::GetValueAt( Be::Time time )
{
	return GetValue( TimeAsDouble( time ) );
}

float Tr2ScalarCurve::GetValueAt( double time )
{
	return GetValue( time );
}

void Tr2ScalarCurve::ScaleTime( float s )
{
	m_timeScale = s;
}

// --------------------------------------------------------------------------------------
// Description:
//   Private virtual function to add a scalar key at a particular time.
// Arguments:
//   time - The time at which to add the key
//   value - The scalar value for the key
// --------------------------------------------------------------------------------------
void Tr2ScalarCurve::AddKey_( float time, const float& value )
{
	Tr2ScalarKeyPtr key;
	if( !key.CreateInstance() )
	{
		return;
	}

	key->m_time = time;
	key->m_value = value;
	key->m_leftTangent = 0.0f;
	key->m_rightTangent = 0.0f;
	key->m_interpolation = (Interpolation)m_interpolation;
	m_keys.Insert( -1, key );
}

// --------------------------------------------------------------------------------------
// Description:
//  Sorts the scalar keys into ascending order based on key time.
// --------------------------------------------------------------------------------------
void Tr2ScalarCurve::Sort()
{
	if ( m_keys.size() > 1 )
	{
		m_keys.Sort( (IList::CompareFn)CompareKeys, NULL );
		// We might have added a key passed the length of the curve
		if ( m_keys.back()->m_time > m_length )
		{
			Tr2ScalarKey* back = m_keys.back();
			float preLength = m_length;
			float endValue = m_endValue;
			float endTangent = m_endTangent;

			m_length = back->m_time;
			m_endValue = back->m_value;
			m_endTangent = back->m_leftTangent;
			if( preLength > 0.0f )
			{	
				back->m_time = preLength;
				back->m_value = endValue;
				back->m_leftTangent = endTangent;
			}
		}
	}
}

// --------------------------------------------------------------------------------------
// Description:
//   Gets the left tangent value for a given key index.  If the index is out of range,
//   the curve's start tangent is returned instead.
// Arguments:
//   idx - Key index
// Return Value:
//   Left tangent value for the given key index, or the curve's start tangent, if the
//   index is out of range.
// --------------------------------------------------------------------------------------
float Tr2ScalarCurve::GetKeyLeftTangent( unsigned int idx ) const
{
	if ( idx < (unsigned int)m_keys.size() )
	{
		return m_keys[idx]->m_leftTangent;
	}
	return m_startTangent;
}

// --------------------------------------------------------------------------------------
// Description:
//   Sets the left tangent for the given key index.  If the index is out of range,
//   nothing happens.
// Arguments:
//   idx - Key index
//   tangent - Left tangent value
// --------------------------------------------------------------------------------------
void Tr2ScalarCurve::SetKeyLeftTangent( unsigned int idx, float tangent )
{
	if ( idx < (unsigned int)m_keys.size() )
	{
		m_keys[idx]->m_leftTangent = tangent;
	}
}

// --------------------------------------------------------------------------------------
// Description:
//   Gets the right tangent value for a given key index.  If the index is out of range,
//   the curve's end tangent is returned instead.
// Arguments:
//   idx - Key index
// Return Value:
//   Right tangent value for the given key index, or the curve's end tangent, if the
//   index is out of range.
// --------------------------------------------------------------------------------------
float Tr2ScalarCurve::GetKeyRightTangent( unsigned int idx ) const
{
	if ( idx < (unsigned int)m_keys.size() )
	{
		return m_keys[idx]->m_rightTangent;
	}
	return m_endTangent;
}

// --------------------------------------------------------------------------------------
// Description:
//   Sets the right tangent for the given key index.  If the index is out of range,
//   nothing happens.  The tangent value is only meaningful if the curve uses Hermite
//   interpolation.
// Arguments:
//   idx - Key index
//   tangent - Right tangent value
// --------------------------------------------------------------------------------------
void Tr2ScalarCurve::SetKeyRightTangent( unsigned int idx, float tangent )
{
	if ( idx < (unsigned int)m_keys.size() )
	{
		m_keys[idx]->m_rightTangent = tangent;
	}
}

// --------------------------------------------------------------------------------------
// Description:
//   Interpolates between two scalar keys using the curve's internal time as the 
//   interpolation control factor.  Interpolation mode is either piecewise linear or
//   Hermite.
// Arguments:
//   out - [output] Interpolated scalar value
//   startKey - First scalar key from which to interpolate
//   endKey - Second scalar key from which to interpolate
// Return Value:
//   Interpolated scalar value
// --------------------------------------------------------------------------------------
float* Tr2ScalarCurve::Interpolate( float* out, Tr2ScalarKey* lastKey, Tr2ScalarKey* nextKey )
{
	*out = m_startValue;
	float time = m_localTime;
	float deltaTime = m_length;
	float startValue = m_startValue;
	float endValue = m_endValue;
	unsigned int interp = m_interpolation;
	if ( lastKey )
	{
		interp = lastKey->m_interpolation;
		time -=  lastKey->m_time;
	}

	// The tr2 curves have by default a start and end point
	switch( interp )
	{
	case LINEAR:
		{	// We are in between two keys start------x---0----x------end
			if ( lastKey && nextKey  )
			{
				startValue = lastKey->m_value;
				endValue = nextKey->m_value;
				deltaTime = nextKey->m_time - lastKey->m_time;
			}
			// We are in between the start of the curve and the next key
			// start--0---x-------x------end
			else if ( lastKey == NULL && nextKey != NULL )
			{
				endValue = nextKey->m_value;
				deltaTime = nextKey->m_time;
			}
			// We are in between the end of the curve and the last key
			// start------x-------x--0---end
			else if ( lastKey != NULL && nextKey == NULL )
			{
				startValue = lastKey->m_value;
				deltaTime = m_length - lastKey->m_time;
			}// else there are no keys, just the start and end point . start---0---end
			*out = startValue + ( endValue - startValue)*(time/deltaTime);
			break;
		}
	case HERMITE:
		{
			float inTangent = m_startTangent;
			float outTangent = m_endTangent;
			// We are in between two keys start------x---0----x------end
			if ( lastKey && nextKey  )
			{
				startValue = lastKey->m_value;
				inTangent = lastKey->m_rightTangent;
				endValue = nextKey->m_value;
				outTangent = nextKey->m_leftTangent;
				deltaTime = nextKey->m_time - lastKey->m_time;
			}
			// We are in between the start of the curve and the next key
			// start--0---x-------x------end
			else if ( lastKey == NULL && nextKey != NULL )
			{
				endValue = nextKey->m_value;
				outTangent = nextKey->m_leftTangent;
				deltaTime = nextKey->m_time;
			}
			// We are in between the end of the curve and the last key
			// start------x-------x--0---end
			else if ( lastKey != NULL && nextKey == NULL )
			{
				startValue = lastKey->m_value;
				inTangent = lastKey->m_rightTangent;
				deltaTime = m_length - lastKey->m_time;
			}// else there are no keys, just the start and end point . start---0---end

			float s = time/deltaTime;
			float s2 = s*s;
			float s3 = s2*s;

			float c2 = -2.0f*s3 + 3.0f*s2;
			float c1 =  1.0f - c2;
			float c4 = s3 - s2;
			float c3 = s + c4 - s2;

			*out = startValue*c1 + endValue*c2 + inTangent * c3 + outTangent * c4;
			break;
		}
	default:
		break;
	}

	return out;
}