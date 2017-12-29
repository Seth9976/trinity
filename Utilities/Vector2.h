#pragma once


struct Vector2
{
	Vector2();
	Vector2( float x_, float y_ );
	Vector2( const XMVECTOR& other );

	operator const XMVECTOR() const;

	float operator[]( int32_t index ) const;
	float& operator[]( int32_t index );

	Vector2& operator+=( const Vector2& other );
	Vector2& operator-=( const Vector2& other );
	Vector2& operator*=( const Vector2& other );
	Vector2& operator/=( const Vector2& other );
	Vector2& operator*=( float f );
	Vector2& operator/=( float f );

	Vector2 operator+() const;
	Vector2 operator-() const;

	const Vector2 operator+( const Vector2& other ) const;
	const Vector2 operator-( const Vector2& other ) const;
	const Vector2 operator*( const Vector2& other ) const;
	const Vector2 operator/( const Vector2& other ) const;

	const Vector2 operator*( float f ) const;
	const Vector2 operator/( float f ) const;

	bool operator==( const Vector2& other ) const;
	bool operator!=( const Vector2& other ) const;

	float x, y;
};

// ----------------------------------------------------------------------------------
inline Vector2::Vector2()
{
}

// ----------------------------------------------------------------------------------
inline Vector2::Vector2( float x_, float y_ )
	: x( x_ ),
	y( y_ )
{
}

// ----------------------------------------------------------------------------------
inline Vector2::Vector2( const XMVECTOR& other )
{
	XMStoreFloat2( reinterpret_cast<XMFLOAT2*>( this ), other );
}

// ----------------------------------------------------------------------------------
inline Vector2::operator const XMVECTOR() const
{
	return XMLoadFloat2( reinterpret_cast<const XMFLOAT2*>( this ) );
}

// ----------------------------------------------------------------------------------
inline float Vector2::operator[]( int32_t index ) const
{
	return ( &x )[index];
}

// ----------------------------------------------------------------------------------
inline float& Vector2::operator[]( int32_t index )
{
	return ( &x )[index];
}

// ----------------------------------------------------------------------------------
inline Vector2& Vector2::operator+=( const Vector2& other )
{
	x += other.x;
	y += other.y;
	return *this;
}

// ----------------------------------------------------------------------------------
inline Vector2& Vector2::operator-=( const Vector2& other )
{
	x -= other.x;
	y -= other.y;
	return *this;
}

// ----------------------------------------------------------------------------------
inline Vector2& Vector2::operator*=( const Vector2& other )
{
	x *= other.x;
	y *= other.y;
	return *this;
}

// ----------------------------------------------------------------------------------
inline Vector2& Vector2::operator/=( const Vector2& other )
{
	x /= other.x;
	y /= other.y;
	return *this;
}

// ----------------------------------------------------------------------------------
inline Vector2& Vector2::operator*=( float f )
{
	x *= f;
	y *= f;
	return *this;
}

// ----------------------------------------------------------------------------------
inline Vector2& Vector2::operator/=( float f )
{
	const float fDiv = 1.0f / f;
	x *= fDiv;
	y *= fDiv;
	return *this;
}

// ----------------------------------------------------------------------------------
inline Vector2 Vector2::operator+() const
{
	return Vector2( *this );
}

// ----------------------------------------------------------------------------------
inline Vector2 Vector2::operator-() const
{
	return Vector2( -x, -y );
}

// ----------------------------------------------------------------------------------
inline const Vector2 Vector2::operator+( const Vector2& other ) const
{
	return Vector2( *this ) += other;
}

// ----------------------------------------------------------------------------------
inline const Vector2 Vector2::operator-( const Vector2& other ) const
{
	return Vector2( *this ) -= other;
}

// ----------------------------------------------------------------------------------
inline const Vector2 Vector2::operator*( const Vector2& other ) const
{
	return Vector2( *this ) *= other;
}

// ----------------------------------------------------------------------------------
inline const Vector2 Vector2::operator/( const Vector2& other ) const
{
	return Vector2( *this ) /= other;
}

// ----------------------------------------------------------------------------------
inline const Vector2 Vector2::operator*( float f ) const
{
	return Vector2( *this ) *= f;
}

// ----------------------------------------------------------------------------------
inline const Vector2 Vector2::operator/( float f ) const
{
	return Vector2( *this ) /= f;
}

// ----------------------------------------------------------------------------------
inline bool Vector2::operator==( const Vector2& other ) const
{
	return ( x == other.x && y == other.y );
}

// ----------------------------------------------------------------------------------
inline bool Vector2::operator!=( const Vector2& other ) const
{
	return ( x != other.x || y != other.y );
}

// ----------------------------------------------------------------------------------
inline const Vector2 operator*( float f, const Vector2& other )
{
	return Vector2( other ) *= f;
}

// ----------------------------------------------------------------------------------
inline float Length( const Vector2& v )
{
	return std::sqrt( v.x * v.x + v.y * v.y );
}

// ----------------------------------------------------------------------------------
inline Vector2 Normalize( const Vector2& v )
{
	float l = 1.f / sqrt( v.x * v.x + v.y * v.y );
	return Vector2( v.x * l, v.y * l );
}
