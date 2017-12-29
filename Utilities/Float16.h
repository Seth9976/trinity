#pragma once

struct Float_16
{
	Float_16();
	explicit Float_16( float other );

	operator float() const;

	bool operator == ( const Float_16& ) const;
	bool operator != ( const Float_16& ) const;
protected:
	static bool IsInf( double num );
	static bool IsNan( double num );
	static bool SignBit( double num );
	static uint16_t Float32To16( float in );
	static float Float16To32( const uint16_t in );

	uint16_t m_value;
};

struct Vector2_16
{
	Vector2_16();
	Vector2_16( Float_16 x_, Float_16 y_ );
	Vector2_16( float x_, float y_ );
	explicit Vector2_16( const Vector2& other );

	operator Vector2() const;

	Float_16 x, y;
};

struct Vector3_16
{
	Vector3_16();
	Vector3_16( Float_16 x_, Float_16 y_, Float_16 z_ );
	Vector3_16( float x_, float y_, float z_ );
	explicit Vector3_16( const Vector3& other );

	operator Vector3() const;

	Float_16 x, y, z;
};

struct Vector4_16
{
	Vector4_16();
	Vector4_16( Float_16 x_, Float_16 y_, Float_16 z_, Float_16 w_ );
	Vector4_16( float x_, float y_, float z_, float w_ );
	explicit Vector4_16( const Vector4& other );

	operator Vector4() const;

	Float_16 x, y, z, w;
};


static_assert( sizeof( Float_16 ) == 2, "Float_16 size incorrect, alignment issues?" );
static_assert( sizeof( Vector2_16 ) == 2 * 2, "Vector2_16 size incorrect, alignment issues?" );
static_assert( sizeof( Vector3_16 ) == 3 * 2, "Vector3_16 size incorrect, alignment issues?" );
static_assert( sizeof( Vector4_16 ) == 4 * 2, "Vector4_16 size incorrect, alignment issues?" );


// --------------------------------------------------------------------------------------
inline Float_16::Float_16()
{
}

// --------------------------------------------------------------------------------------
inline Float_16::Float_16( float other )
	: m_value( Float32To16( other ) )
{
}

// --------------------------------------------------------------------------------------
inline Float_16::operator float() const
{
	return Float16To32( m_value );
}

// --------------------------------------------------------------------------------------
inline bool Float_16::operator == ( const Float_16& other ) const
{
	return m_value == other.m_value;
}

// --------------------------------------------------------------------------------------
inline bool Float_16::operator != ( const Float_16& other ) const
{
	return m_value != other.m_value;
}


#ifdef _MSC_VER

inline bool Float_16::IsInf( double num )
{
	return !_finite( num ) && !_isnan( num );
}

inline bool Float_16::IsNan( double num )
{
	return !!_isnan( num );
}

inline bool Float_16::SignBit( double num )
{
	return _copysign( 1.0, num ) < 0;
}

#elif defined( __ANDROID__ )

inline bool Float_16::IsInf( double num )
{
	return std::isinf( num );
}

inline bool Float_16::IsNan( double num )
{
	return isnan( num );
}

inline bool Float_16::SignBit( double num )
{
	return std::signbit( num );
}

#elif !defined( __APPLE__) && !defined(__ORBIS__)

inline bool Float_16::IsInf( double num )
{
	return isinf( num );
}

inline bool Float_16::IsNan( double num )
{
	return isnan( num );
}

inline bool Float_16::SignBit( double num )
{
	return std::signbit( num );
}

#else

inline bool Float_16::IsInf( double num )
{
	return isinf( num );
}

inline bool Float_16::IsNan( double num )
{
	return isnan( num );
}

inline bool Float_16::SignBit( double num )
{
	return signbit( num );
}

#endif

// --------------------------------------------------------------------------------------
inline uint16_t Float_16::Float32To16( float in )
{
	int exp = 0, origexp;
	float tmp = fabs( in );
	int sign = SignBit( in );
	uint32_t mantissa;
	uint16_t ret;

	/* Deal with special numbers */
	if( IsInf( in ) )
	{
		return ( sign ? 0xffff : 0x7fff );
	}
	if( IsNan( in ) )
	{
		return ( sign ? 0xffff : 0x7fff );
	}
	if( in == 0.0f )
	{
		return ( sign ? 0x8000 : 0x0000 );
	}

	if( tmp < powf( 2, 10 ) )
	{
		do
		{
			tmp *= 2.0f;
			exp--;
		} while( tmp < powf( 2, 10 ) );
	}
	else if( tmp >= powf( 2, 11 ) )
	{
		do
		{
			tmp /= 2.0f;
			exp++;
		} while( tmp >= powf( 2, 11 ) );
	}

	exp += 10;  /* Normalize the mantissa */
	exp += 15;  /* Exponent is encoded with excess 15 */

	origexp = exp;

	mantissa = (uint32_t)tmp;
	if( ( tmp - mantissa == 0.5f && mantissa % 2 == 1 ) || /* round half to even */
		( tmp - mantissa > 0.5f ) )
	{
		mantissa++; /* round to nearest, away from zero */
	}
	if( mantissa == 2048 )
	{
		mantissa = 1024;
		exp++;
	}

	if( exp > 31 )
	{
		/* too big */
		ret = 0x7fff; /* INF */
	}
	else if( exp <= 0 )
	{
		uint32_t rounding = 0;

		/* Denormalized half float */

		/* return 0x0000 (=0.0) for numbers too small to represent in half floats */
		if( exp < -11 )
		{
			return ( sign ? 0x8000 : 0x0000 );
		}
		exp = origexp;

		/* the 13 extra bits from single precision are used for rounding */
		mantissa = (uint32_t)( tmp * powf( 2, 13 ) );
		mantissa >>= 1 - exp; /* denormalize */

		mantissa -= ~( mantissa >> 13 ) & 1; /* round half to even */
											 /* remove 13 least significant bits to get half float precision */
		mantissa >>= 12;
		rounding = mantissa & 1;
		mantissa >>= 1;

		ret = mantissa + rounding;
	}
	else
	{
		ret = ( exp << 10 ) | ( mantissa & 0x3ff );
	}

	ret |= ( ( sign ? 1 : 0 ) << 15 ); /* Add the sign */
	return ret;
}

// --------------------------------------------------------------------------------------
inline float Float_16::Float16To32( const uint16_t in )
{

	const uint16_t s = ( in & 0x8000 );
	const uint16_t e = ( in & 0x7C00 ) >> 10;
	const uint16_t m = in & 0x3FF;
	const float sgn = ( s ? -1.0f : 1.0f );

	if( ( in & ~0x8000 ) == 0x7fff )
	{
		return s ? -std::numeric_limits<float>::quiet_NaN() : std::numeric_limits<float>::quiet_NaN();
	}

	if( e == 0 )
	{
		if( m == 0 )
		{
			return sgn * 0.0f; /* +0.0 or -0.0 */
		}
		else
		{
			return sgn * powf( 2, -14.0f ) * ( m / 1024.0f );
		}
	}
	else
	{
		return sgn * powf( 2, e - 15.0f ) * ( 1.0f + ( m / 1024.0f ) );
	}
}

// --------------------------------------------------------------------------------------
inline Vector2_16::Vector2_16()
{
}

// --------------------------------------------------------------------------------------
inline Vector2_16::Vector2_16( float x_, float y_ )
	:x( x_ ),
	y( y_ )
{
}

// --------------------------------------------------------------------------------------
inline Vector2_16::Vector2_16( Float_16 x_, Float_16 y_ )
	:x( x_ ),
	y( y_ )
{
}

// --------------------------------------------------------------------------------------
inline Vector2_16::Vector2_16( const Vector2& other )
	:x( other.x ),
	y( other.y )
{
}

// --------------------------------------------------------------------------------------
inline Vector2_16::operator Vector2() const
{
	return Vector2( x, y );
}

// --------------------------------------------------------------------------------------
inline Vector3_16::Vector3_16()
{
}

// --------------------------------------------------------------------------------------
inline Vector3_16::Vector3_16( Float_16 x_, Float_16 y_, Float_16 z_ )
	:x( x_ ),
	y( y_ ),
	z( z_ )
{
}

// --------------------------------------------------------------------------------------
inline Vector3_16::Vector3_16( float x_, float y_, float z_ )
	:x( x_ ),
	y( y_ ),
	z( z_ )
{
}

// --------------------------------------------------------------------------------------
inline Vector3_16::Vector3_16( const Vector3& other )
	:x( other.x ),
	y( other.y ),
	z( other.z )
{
}

// --------------------------------------------------------------------------------------
inline Vector3_16::operator Vector3() const
{
	return Vector3( x, y, z );
}

// --------------------------------------------------------------------------------------
inline Vector4_16::Vector4_16()
{
}

// --------------------------------------------------------------------------------------
inline Vector4_16::Vector4_16( Float_16 x_, Float_16 y_, Float_16 z_, Float_16 w_ )
	:x( x_ ),
	y( y_ ),
	z( z_ ),
	w( w_ )
{
}

// --------------------------------------------------------------------------------------
inline Vector4_16::Vector4_16( float x_, float y_, float z_, float w_ )
	:x( x_ ),
	y( y_ ),
	z( z_ ),
	w( w_ )
{
}

// --------------------------------------------------------------------------------------
inline Vector4_16::Vector4_16( const Vector4& other )
	:x( other.x ),
	y( other.y ),
	z( other.z ),
	w( other.w )
{
}

// --------------------------------------------------------------------------------------
inline Vector4_16::operator Vector4() const
{
	return Vector4( x, y, z, w );
}
