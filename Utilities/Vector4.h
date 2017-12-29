#pragma once


struct Vector4
{
	Vector4();
	Vector4( float x, float y, float z, float w );
	Vector4( const Vector3& xyz, float w );
	explicit Vector4( const XMVECTOR& other );

	operator const XMVECTOR() const;

	float operator[]( int32_t index ) const;
	float& operator[]( int32_t index );

	Vector4& operator+=( const Vector4& other );
	Vector4& operator-=( const Vector4& other );
	Vector4& operator*=( const Vector4& other );
	Vector4& operator*=( float f );
	Vector4& operator/=( float f );

	Vector4 operator+() const;
	Vector4 operator-() const;

	const Vector4 operator+( const Vector4& other ) const;
	const Vector4 operator-( const Vector4& other ) const;
	const Vector4 operator*( const Vector4& other ) const;
	const Vector4 operator*( float f ) const;
	const Vector4 operator/( float f ) const;

	bool operator==( const Vector4& other ) const;
	bool operator!=( const Vector4& other ) const;

	const Vector3& GetXYZ() const;
	Vector3& GetXYZ();

	float x, y, z, w;
};

// --------------------------------------------------------------------------------------
inline Vector4::Vector4()
{
}

// --------------------------------------------------------------------------------------
inline Vector4::Vector4( float x_, float y_, float z_, float w_ )
	:x( x_ ),
	y( y_ ),
	z( z_ ),
	w( w_ )
{
}

// --------------------------------------------------------------------------------------
inline Vector4::Vector4( const Vector3& xyz, float w_ )
	: x( xyz.x ),
	y( xyz.y ),
	z( xyz.z ),
	w( w_ )
{
}

// --------------------------------------------------------------------------------------
inline Vector4::Vector4( const XMVECTOR& other )
{
	XMStoreFloat4( reinterpret_cast<XMFLOAT4*>( this ), other );
}

// --------------------------------------------------------------------------------------
inline Vector4::operator const XMVECTOR() const
{
	return XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>( this ) );
}

// ----------------------------------------------------------------------------------
inline float Vector4::operator[]( int32_t index ) const
{
	return ( &x )[index];
}

// ----------------------------------------------------------------------------------
inline float& Vector4::operator[]( int32_t index )
{
	return ( &x )[index];
}

// --------------------------------------------------------------------------------------
inline Vector4& Vector4::operator+=( const Vector4& other )
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Vector4& Vector4::operator-=( const Vector4& other )
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Vector4& Vector4::operator*=( const Vector4& other )
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
	w *= other.w;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Vector4& Vector4::operator*=( float f )
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Vector4& Vector4::operator/=( float f )
{
	const float fDiv = 1.0f / f;
	x *= fDiv;
	y *= fDiv;
	z *= fDiv;
	w *= fDiv;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Vector4 Vector4::operator+() const
{
	return Vector4( *this );
}

// --------------------------------------------------------------------------------------
inline Vector4 Vector4::operator-() const
{
	return Vector4( -x, -y, -z, -w );
}

// --------------------------------------------------------------------------------------
inline const Vector4 Vector4::operator+( const Vector4& other ) const
{
	return Vector4( *this ) += other;
}

// --------------------------------------------------------------------------------------
inline const Vector4 Vector4::operator-( const Vector4& other ) const
{
	return Vector4( *this ) -= other;
}

// --------------------------------------------------------------------------------------
inline const Vector4 Vector4::operator*( const Vector4& other ) const
{
	return Vector4( *this ) *= other;
}

// --------------------------------------------------------------------------------------
inline const Vector4 Vector4::operator*( float f ) const
{
	return Vector4( *this ) *= f;
}

// --------------------------------------------------------------------------------------
inline const Vector4 Vector4::operator/( float f ) const
{
	return Vector4( *this ) /= f;
}

// --------------------------------------------------------------------------------------
inline bool Vector4::operator==( const Vector4& other ) const
{
	return ( x == other.x && y == other.y && z == other.z && w == other.w );
}

// --------------------------------------------------------------------------------------
inline bool Vector4::operator!=( const Vector4& other ) const
{
	return ( x != other.x || y != other.y || z != other.z || w != other.w );
}

// --------------------------------------------------------------------------------------
inline const Vector3& Vector4::GetXYZ() const
{
	return *reinterpret_cast<const Vector3*>( &x );
}

// --------------------------------------------------------------------------------------
inline Vector3& Vector4::GetXYZ()
{
	return *reinterpret_cast<Vector3*>( &x );
}

// --------------------------------------------------------------------------------------
inline const Vector4 operator*( float f, const Vector4& other )
{
	return Vector4( other ) *= f;
}

// --------------------------------------------------------------------------------------
inline bool IsMatch( Be::Var* value, const Vector4& t )
{
	return (Be::Var*)&t == value;
}

// --------------------------------------------------------------------------------------
inline float Dot( const Vector4& v1, const Vector4& v2 )
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

// --------------------------------------------------------------------------------------
inline Vector4 Cross( const Vector4& v1, const Vector4& v2, const Vector4& v3 )
{
	return Vector4(
		v1.y * ( v2.z * v3.w - v3.z * v2.w ) - v1.z * ( v2.y * v3.w - v3.y * v2.w ) + v1.w * ( v2.y * v3.z - v2.z * v3.y ),
		-( v1.x * ( v2.z * v3.w - v3.z * v2.w ) - v1.z * ( v2.x * v3.w - v3.x * v2.w ) + v1.w * ( v2.x * v3.z - v3.x * v2.z ) ),
		v1.x * ( v2.y * v3.w - v3.y * v2.w ) - v1.y * ( v2.x *v3.w - v3.x * v2.w ) + v1.w * ( v2.x * v3.y - v3.x * v2.y ),
		-( v1.x * ( v2.y * v3.z - v3.y * v2.z ) - v1.y * ( v2.x * v3.z - v3.x *v2.z ) + v1.z * ( v2.x * v3.y - v3.x * v2.y ) ) );
}
