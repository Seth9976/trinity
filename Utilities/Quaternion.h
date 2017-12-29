#pragma once


struct Quaternion
{
	Quaternion();
	Quaternion( float x, float y, float z, float w );
	Quaternion( const XMVECTOR& other );
	Quaternion( const Vector4& other );

	operator const XMVECTOR() const;

	Quaternion& operator+=( const Quaternion& other );
	Quaternion& operator-=( const Quaternion& other );
	Quaternion& operator*=( const Quaternion& other );
	Quaternion& operator*=( float f );
	Quaternion& operator/=( float f );

	Quaternion operator+() const;
	Quaternion operator-() const;

	const Quaternion operator+( const Quaternion& other ) const;
	const Quaternion operator-( const Quaternion& other ) const;
	const Quaternion operator*( const Quaternion& other ) const;

	const Quaternion operator*( float f ) const;
	const Quaternion operator/( float f ) const;

	bool operator==( const Quaternion& other ) const;
	bool operator!=( const Quaternion& other ) const;

	float x, y, z, w;
};

// --------------------------------------------------------------------------------------
inline Quaternion::Quaternion() 
{
}

// --------------------------------------------------------------------------------------
inline Quaternion::Quaternion( float x_, float y_, float z_, float w_ )
	:x( x_ ),
	y( y_ ),
	z( z_ ),
	w( w_ )
{
}

// --------------------------------------------------------------------------------------
inline Quaternion::Quaternion( const XMVECTOR& other )
{
	XMStoreFloat4( reinterpret_cast<XMFLOAT4*>( this ), other );
}

// --------------------------------------------------------------------------------------
inline Quaternion::Quaternion( const Vector4& other )
	:x( other.x ),
	y( other.y ),
	z( other.z ),
	w( other.w )
{
}

// --------------------------------------------------------------------------------------
inline Quaternion::operator const XMVECTOR() const
{
	return XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>( this ) );
}

// --------------------------------------------------------------------------------------
inline Quaternion& Quaternion::operator+=( const Quaternion& other )
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Quaternion& Quaternion::operator-=( const Quaternion& other )
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Quaternion& Quaternion::operator*=( const Quaternion& other )
{
	*this = *this * other;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Quaternion& Quaternion::operator*=( float f )
{
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Quaternion& Quaternion::operator/=( float f )
{
	const float fDiv = 1.0f / f;
	x *= fDiv;
	y *= fDiv;
	z *= fDiv;
	w *= fDiv;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Quaternion Quaternion::operator+() const
{
	return Quaternion( *this );
}

// --------------------------------------------------------------------------------------
inline Quaternion Quaternion::operator-() const
{
	return Quaternion( -x, -y, -z, -w );
}

// --------------------------------------------------------------------------------------
inline const Quaternion Quaternion::operator+( const Quaternion& other ) const
{
	return Quaternion( *this ) += other;
}

// --------------------------------------------------------------------------------------
inline const Quaternion Quaternion::operator-( const Quaternion& other ) const
{
	return Quaternion( *this ) -= other;
}

// --------------------------------------------------------------------------------------
inline const Quaternion Quaternion::operator*( const Quaternion& other ) const
{
	Quaternion result;
	result.x = other.w * x + other.x * w + other.y * z - other.z * y;
	result.y = other.w * y - other.x * z + other.y * w + other.z * x;
	result.z = other.w * z + other.x * y - other.y * x + other.z * w;
	result.w = other.w * w - other.x * x - other.y * y - other.z * z;
	return result;
}

// --------------------------------------------------------------------------------------
inline const Quaternion Quaternion::operator*( float f ) const
{
	return Quaternion( *this ) *= f;
}

// --------------------------------------------------------------------------------------
inline const Quaternion Quaternion::operator/( float f ) const
{
	return Quaternion( *this ) /= f;
}

// --------------------------------------------------------------------------------------
inline bool Quaternion::operator==( const Quaternion& other ) const
{
	return ( x == other.x && y == other.y && z == other.z && w == other.w );
}

// --------------------------------------------------------------------------------------
inline bool Quaternion::operator!=( const Quaternion& other ) const
{
	return ( x != other.x || y != other.y || z != other.z || w != other.w );
}

// --------------------------------------------------------------------------------------
inline const Quaternion operator*( float f, const Quaternion& other )
{
	return Quaternion( other ) *= f;
}

// --------------------------------------------------------------------------------------
inline bool IsMatch( Be::Var* value, const Quaternion& t )
{
	return (Be::Var*)&t == value;
}


// --------------------------------------------------------------------------------------
inline Quaternion IdentityQuaternion()
{
	return Quaternion( 0.0f, 0.0f, 0.0f, 1.0f );
}

// --------------------------------------------------------------------------------------
template<>
inline void BlueGetNullValue( Quaternion& resultRef )
{
	resultRef = IdentityQuaternion();
}

// --------------------------------------------------------------------------------------
inline float LengthSq( const Quaternion& q )
{
	return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

// --------------------------------------------------------------------------------------
inline float Length( const Quaternion& q )
{
	return sqrt( LengthSq( q ) );
}

// --------------------------------------------------------------------------------------
inline Quaternion Normalize( const Quaternion& q )
{
	float l = 1.0f / Length( q );
	return q * l;
}

// --------------------------------------------------------------------------------------
inline Quaternion Inverse( const Quaternion& q )
{
	float l = 1.0f / LengthSq( q );
	return Quaternion( -q.x * l, -q.y * l, -q.z * l, q.w * l );
}

// --------------------------------------------------------------------------------------
inline Quaternion Conjugate( const Quaternion& q )
{
	Quaternion out;
	out.x = -q.x;
	out.y = -q.y;
	out.z = -q.z;
	out.w = q.w;
	return out;
}

// --------------------------------------------------------------------------------------
inline Quaternion Exp( const Quaternion& q )
{
	float norm = sqrt( q.x * q.x + q.y * q.y + q.z * q.z );
	Quaternion out;
	if( norm )
	{
		out.x = sin( norm ) * q.x / norm;
		out.y = sin( norm ) * q.y / norm;
		out.z = sin( norm ) * q.z / norm;
		out.w = cos( norm );
	}
	else
	{
		out.x = 0.0f;
		out.y = 0.0f;
		out.z = 0.0f;
		out.w = 1.0f;
	}
	return out;


}

// --------------------------------------------------------------------------------------
inline float Dot( const Quaternion& q1, const Quaternion& q2 )
{
	return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

// --------------------------------------------------------------------------------------
inline Quaternion RotationQuaternion( const Matrix& m )
{
	Quaternion out;
	int i, maxi;
	float maxdiag, S, trace;

	trace = m.m[0][0] + m.m[1][1] + m.m[2][2] + 1.0f;
	if( trace > 1.0f )
	{
		out.x = ( m.m[1][2] - m.m[2][1] ) / ( 2.0f * sqrt( trace ) );
		out.y = ( m.m[2][0] - m.m[0][2] ) / ( 2.0f * sqrt( trace ) );
		out.z = ( m.m[0][1] - m.m[1][0] ) / ( 2.0f * sqrt( trace ) );
		out.w = sqrt( trace ) / 2.0f;
		return out;
	}
	maxi = 0;
	maxdiag = m.m[0][0];
	for( i = 1; i<3; i++ )
	{
		if( m.m[i][i] > maxdiag )
		{
			maxi = i;
			maxdiag = m.m[i][i];
		}
	}
	switch( maxi )
	{
	case 0:
		S = 2.0f * sqrt( 1.0f + m.m[0][0] - m.m[1][1] - m.m[2][2] );
		out.x = 0.25f * S;
		out.y = ( m.m[0][1] + m.m[1][0] ) / S;
		out.z = ( m.m[0][2] + m.m[2][0] ) / S;
		out.w = ( m.m[1][2] - m.m[2][1] ) / S;
		break;
	case 1:
		S = 2.0f * sqrt( 1.0f + m.m[1][1] - m.m[0][0] - m.m[2][2] );
		out.x = ( m.m[0][1] + m.m[1][0] ) / S;
		out.y = 0.25f * S;
		out.z = ( m.m[1][2] + m.m[2][1] ) / S;
		out.w = ( m.m[2][0] - m.m[0][2] ) / S;
		break;
	case 2:
		S = 2.0f * sqrt( 1.0f + m.m[2][2] - m.m[0][0] - m.m[1][1] );
		out.x = ( m.m[0][2] + m.m[2][0] ) / S;
		out.y = ( m.m[1][2] + m.m[2][1] ) / S;
		out.z = 0.25f * S;
		out.w = ( m.m[0][1] - m.m[1][0] ) / S;
		break;
	}
	return out;
}

// --------------------------------------------------------------------------------------
inline Quaternion RotationQuaternion( float yaw, float pitch, float roll )
{
	float sinYaw = sin( yaw / 2.0f );
	float cosYaw = cos( yaw / 2.0f );
	float sinPitch = sin( pitch / 2.0f );
	float cosPitch = cos( pitch / 2.0f );
	float sinRoll = sin( roll / 2.0f );
	float cosRoll = cos( roll / 2.0f );

	Quaternion out;
	out.x = sinYaw * cosPitch * sinRoll + cosYaw * sinPitch * cosRoll;
	out.y = sinYaw * cosPitch * cosRoll - cosYaw * sinPitch * sinRoll;
	out.z = cosYaw * cosPitch * sinRoll - sinYaw * sinPitch * cosRoll;
	out.w = cosYaw * cosPitch * cosRoll + sinYaw * sinPitch * sinRoll;
	return out;
}

// --------------------------------------------------------------------------------------
inline Quaternion RotationQuaternion( const Vector3& axis, float angle )
{
	Vector3 temp = Normalize( axis );
	Quaternion out;
	out.x = sin( angle / 2.0f ) * temp.x;
	out.y = sin( angle / 2.0f ) * temp.y;
	out.z = sin( angle / 2.0f ) * temp.z;
	out.w = cos( angle / 2.0f );
	return out;
}

// --------------------------------------------------------------------------------------
inline Quaternion Slerp( const Quaternion& q1, const Quaternion& q2, float t )
{
	float epsilon = 1.0f;
	float temp = 1.0f - t;
	float u = t;
	float dot = Dot( q1, q2 );
	if( dot < 0.0f )
	{
		epsilon = -1.0f;
		dot = -dot;
	}
	if( 1.0f - dot > 0.001f )
	{
		float theta = acos( dot );
		temp = sin( theta * temp ) / sin( theta );
		u = sin( theta * u ) / sin( theta );
	}
	Quaternion out;
	out.x = temp * q1.x + epsilon * u * q2.x;
	out.y = temp * q1.y + epsilon * u * q2.y;
	out.z = temp * q1.z + epsilon * u * q2.z;
	out.w = temp * q1.w + epsilon * u * q2.w;
	return out;
}

// --------------------------------------------------------------------------------------
inline std::pair<Vector3, float> GetAxisAngle( const Quaternion& q )
{
	return std::make_pair( Vector3( q.x, q.y, q.z ), acos( q.w ) );
}
