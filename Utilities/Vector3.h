#pragma once


struct Vector3
{
	Vector3();
	Vector3( float x, float y, float z );
	Vector3( const XMVECTOR& other );

	operator const XMVECTOR() const;

	float operator[]( int32_t index ) const;
	float& operator[]( int32_t index );

	Vector3& operator+=( const Vector3& other );
	Vector3& operator-=( const Vector3& other );
	Vector3& operator*=( const Vector3& other );
	Vector3& operator*=( float f );
	Vector3& operator/=( float f );

	Vector3 operator+() const;
	Vector3 operator-() const;

	const Vector3 operator+( const Vector3& other ) const;
	const Vector3 operator-( const Vector3& other ) const;
	const Vector3 operator*( const Vector3& other ) const;
	const Vector3 operator*( float f ) const;
	const Vector3 operator/( float f ) const;

	bool operator==( const Vector3& other ) const;
	bool operator!=( const Vector3& other ) const;

	const Vector2& GetXY() const;
	Vector2& GetXY();

	float x, y, z;
};

// --------------------------------------------------------------------------------------
inline Vector3::Vector3()
{
}

// --------------------------------------------------------------------------------------
inline Vector3::Vector3( float x_, float y_, float z_ )
	: x( x_ ),
	y( y_ ),
	z( z_ )
{
}

// --------------------------------------------------------------------------------------
inline Vector3::Vector3( const XMVECTOR& other )
{
	XMStoreFloat3( ( XMFLOAT3* )this, other );
}

// --------------------------------------------------------------------------------------
inline Vector3::operator const XMVECTOR() const
{
	return XMLoadFloat3( ( const XMFLOAT3* )this );
}

// ----------------------------------------------------------------------------------
inline float Vector3::operator[]( int32_t index ) const
{
	return ( &x )[index];
}

// ----------------------------------------------------------------------------------
inline float& Vector3::operator[]( int32_t index )
{
	return ( &x )[index];
}

// --------------------------------------------------------------------------------------
inline Vector3& Vector3::operator+=( const Vector3& other )
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Vector3& Vector3::operator-=( const Vector3& other )
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Vector3& Vector3::operator*=( const Vector3& other )
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Vector3& Vector3::operator*=( float f )
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Vector3& Vector3::operator/=( float f )
{
	const float fDiv = 1.0f / f;
	x *= fDiv;
	y *= fDiv;
	z *= fDiv;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Vector3 Vector3::operator+() const
{
	return Vector3( *this );
}

// --------------------------------------------------------------------------------------
inline Vector3 Vector3::operator-() const
{
	return Vector3( -x, -y, -z );
}

// --------------------------------------------------------------------------------------
inline const Vector3 Vector3::operator+( const Vector3& other ) const
{
	return Vector3( *this ) += other;
}

// --------------------------------------------------------------------------------------
inline const Vector3 Vector3::operator-( const Vector3& other ) const
{
	return Vector3( *this ) -= other;
}

// --------------------------------------------------------------------------------------
inline const Vector3 Vector3::operator*( const Vector3& other ) const
{
	return Vector3( *this ) *= other;
}

// --------------------------------------------------------------------------------------
inline const Vector3 Vector3::operator*( float f ) const
{
	return Vector3( *this ) *= f;
}

// --------------------------------------------------------------------------------------
inline const Vector3 Vector3::operator/( float f ) const
{
	return Vector3( *this ) /= f;
}

// --------------------------------------------------------------------------------------
inline bool Vector3::operator==( const Vector3& other ) const
{
	return ( x == other.x && y == other.y && z == other.z );
}

// --------------------------------------------------------------------------------------
inline bool Vector3::operator!=( const Vector3& other ) const
{
	return ( x != other.x || y != other.y || z != other.z );
}

// --------------------------------------------------------------------------------------
inline const Vector2& Vector3::GetXY() const
{
	return *reinterpret_cast<const Vector2*>( &x );
}

// --------------------------------------------------------------------------------------
inline Vector2& Vector3::GetXY()
{
	return *reinterpret_cast<Vector2*>( &x );
}

inline const Vector3 operator*( float f, const Vector3& other )
{
	return Vector3( other ) *= f;
}


inline bool IsMatch( Be::Var* value, const Vector3& t )
{
	return (Be::Var*)&t == value;
}

// --------------------------------------------------------------------------------------
inline float Dot( const Vector3& v1, const Vector3& v2 )
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// --------------------------------------------------------------------------------------
inline float LengthSq( const Vector3& v )
{
	return Dot( v, v );
}

// --------------------------------------------------------------------------------------
inline float Length( const Vector3& v )
{
	return std::sqrt( LengthSq( v ) );
}

// --------------------------------------------------------------------------------------
inline Vector3 Normalize( const Vector3& v )
{
	auto length = Length( v );
	return v / length;
}

// --------------------------------------------------------------------------------------
inline Vector3 Cross( const Vector3& v1, const Vector3& v2 )
{
	return Vector3( v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x );
}

// --------------------------------------------------------------------------------------
inline Vector3 ClampLength( const Vector3& v, float maxLength )
{
	if( Length( v ) <= maxLength )
	{
		return v;
	}
	return Normalize( v ) * maxLength;
}

// --------------------------------------------------------------------------------------
inline Vector3 Lerp( const Vector3& v1, const Vector3& v2, float s )
{
	return v1 + ( v2 - v1 ) * s;
}

// --------------------------------------------------------------------------------------
inline Vector3 Maximize( const Vector3& v1, const Vector3& v2 )
{
	return Vector3( std::max( v1.x, v2.x ), std::max( v1.y, v2.y ), std::max( v1.z, v2.z ) );
}

// --------------------------------------------------------------------------------------
inline Vector3 Minimize( const Vector3& v1, const Vector3& v2 )
{
	return Vector3( std::min( v1.x, v2.x ), std::min( v1.y, v2.y ), std::min( v1.z, v2.z ) );
}

// --------------------------------------------------------------------------------------
inline Vector3 Hermite( const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float s )
{
	float k3 = 2.f * s * s * s - 3 * s * s + 1;
	float k2 = -2.f * s * s * s + 3 * s * s;
	float k1 = s * s * s - 2 * s * s + s;
	float k0 = s * s * s - s * s;

	return Vector3(
		k3 * v1.x + k2 * v2.x + k1 * t1.x + k0 * t2.x,
		k3 * v1.y + k2 * v2.y + k1 * t1.y + k0 * t2.y,
		k3 * v1.z + k2 * v2.z + k1 * t1.z + k0 * t2.z );
}

// --------------------------------------------------------------------------------------
inline bool SphereBoundProbe(
	const Vector3& center,
	float radius,
	const Vector3& rayPosition,
	const Vector3& rayDirection )
{
	Vector3 difference = rayPosition - center;
	float a = LengthSq( rayDirection );
	float b = Dot( difference, rayDirection );
	float c = LengthSq( difference ) - radius * radius;
	float d = b * b - a * c;
	return !( ( d <= 0.0f ) || ( 2.0f * sqrt( d ) <= b ) );
}

// --------------------------------------------------------------------------------------
inline void ComputeBoundingSphere(
	const Vector3* firstPosition,
	uint32_t numVertices,
	uint32_t stride,
	Vector3& center,
	float& radius )
{
	Vector3 temp( 0.0f, 0.0f, 0.0f );
	radius = 0.0f;
	if( numVertices == 0 )
	{
		center = temp;
		return;
	}

	const uint8_t* data = reinterpret_cast<const uint8_t*>( firstPosition );

	for( uint32_t i = 0; i < numVertices; i++ )
	{
		temp += *reinterpret_cast<const Vector3*>( data );
		data += stride;
	}

	center = temp / float( numVertices );
	data = reinterpret_cast<const uint8_t*>( firstPosition );

	for( uint32_t i = 0; i < numVertices; i++ )
	{
		float d = LengthSq( center - *reinterpret_cast<const Vector3*>( data ) );
		data += stride;
		if( d > radius )
		{
			radius = d;
		}
	}
	radius = sqrt( radius );
}
