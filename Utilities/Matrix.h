#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"


struct Matrix
{
	Matrix();
	Matrix(
		float f11, float f12, float f13, float f14,
		float f21, float f22, float f23, float f24,
		float f31, float f32, float f33, float f34,
		float f41, float f42, float f43, float f44 );
	Matrix( const XMMATRIX& other );

	float& operator()( unsigned int row, unsigned int col );
	float operator()( unsigned int row, unsigned int col ) const;

	operator XMMATRIX() const;

	Matrix& operator+=( const Matrix& other );
	Matrix& operator-=( const Matrix& other );
	Matrix& operator*=( const Matrix& other );
	Matrix& operator*=( float f );
	Matrix& operator/=( float f );

	Matrix operator+() const;
	Matrix operator-() const;

	const Matrix operator+( const Matrix& other ) const;
	const Matrix operator-( const Matrix& other ) const;
	const Matrix operator*( const Matrix& other ) const;
	const Matrix operator*( float f ) const;
	const Matrix operator/( float f ) const;

	bool operator==( const Matrix& other ) const;
	bool operator!=( const Matrix& other ) const;

	const Vector3& GetTranslation() const;
	Vector3& GetTranslation();
	const Vector3& GetX() const;
	Vector3& GetX();
	const Vector3& GetY() const;
	Vector3& GetY();
	const Vector3& GetZ() const;
	Vector3& GetZ();

	union 
	{
		struct 
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;

		};
		float m[4][4];
	};
};

#include "Quaternion.h"


// --------------------------------------------------------------------------------------
inline Matrix::Matrix() 
{
}

// --------------------------------------------------------------------------------------
inline Matrix::Matrix(
	float f11, float f12, float f13, float f14,
	float f21, float f22, float f23, float f24,
	float f31, float f32, float f33, float f34,
	float f41, float f42, float f43, float f44 )
{
	_11 = f11; _12 = f12; _13 = f13; _14 = f14;
	_21 = f21; _22 = f22; _23 = f23; _24 = f24;
	_31 = f31; _32 = f32; _33 = f33; _34 = f34;
	_41 = f41; _42 = f42; _43 = f43; _44 = f44;
}

// --------------------------------------------------------------------------------------
inline Matrix::Matrix( const XMMATRIX& other )
{
	XMStoreFloat4x4( ( XMFLOAT4X4* )this, other );
}

// --------------------------------------------------------------------------------------
inline float& Matrix::operator()( unsigned int row, unsigned int col )
{
	return m[row][col];
}

// --------------------------------------------------------------------------------------
inline float Matrix::operator()( unsigned int row, unsigned int col ) const
{
	return m[row][col];
}

// --------------------------------------------------------------------------------------
inline Matrix::operator XMMATRIX() const
{
	return XMLoadFloat4x4( ( const XMFLOAT4X4* )this );
}

// --------------------------------------------------------------------------------------
inline Matrix& Matrix::operator+=( const Matrix& other )
{
	_11 += other._11;
	_12 += other._12;
	_13 += other._13;
	_14 += other._14;

	_21 += other._21;
	_22 += other._22;
	_23 += other._23;
	_24 += other._24;

	_31 += other._31;
	_32 += other._32;
	_33 += other._33;
	_34 += other._34;

	_41 += other._41;
	_42 += other._42;
	_43 += other._43;
	_44 += other._44;

	return *this;
}

// --------------------------------------------------------------------------------------
inline Matrix& Matrix::operator-=( const Matrix& other )
{
	_11 += other._11;
	_12 += other._12;
	_13 += other._13;
	_14 += other._14;

	_21 += other._21;
	_22 += other._22;
	_23 += other._23;
	_24 += other._24;

	_31 += other._31;
	_32 += other._32;
	_33 += other._33;
	_34 += other._34;

	_41 += other._41;
	_42 += other._42;
	_43 += other._43;
	_44 += other._44;

	return *this;
}

// --------------------------------------------------------------------------------------
inline Matrix& Matrix::operator*=( const Matrix& other )
{
	*this = *this * other;
	return *this;
}

// --------------------------------------------------------------------------------------
inline Matrix& Matrix::operator*=( float f )
{
	_11 *= f;
	_12 *= f;
	_13 *= f;
	_14 *= f;

	_21 *= f;
	_22 *= f;
	_23 *= f;
	_24 *= f;

	_31 *= f;
	_32 *= f;
	_33 *= f;
	_34 *= f;

	_41 *= f;
	_42 *= f;
	_43 *= f;
	_44 *= f;

	return *this;
}

// --------------------------------------------------------------------------------------
inline Matrix& Matrix::operator/=( float f )
{
	const float fDiv = 1.0f / f;

	_11 *= fDiv;
	_12 *= fDiv;
	_13 *= fDiv;
	_14 *= fDiv;

	_21 *= fDiv;
	_22 *= fDiv;
	_23 *= fDiv;
	_24 *= fDiv;

	_31 *= fDiv;
	_32 *= fDiv;
	_33 *= fDiv;
	_34 *= fDiv;

	_41 *= fDiv;
	_42 *= fDiv;
	_43 *= fDiv;
	_44 *= fDiv;

	return *this;
}

// --------------------------------------------------------------------------------------
inline Matrix Matrix::operator+() const
{
	return Matrix( *this );
}

// --------------------------------------------------------------------------------------
inline Matrix Matrix::operator-() const
{
	return Matrix( -_11, -_12, -_13, -_14,
		-_21, -_22, -_23, -_24,
		-_31, -_32, -_33, -_34,
		-_41, -_42, -_43, -_44 );
}

// --------------------------------------------------------------------------------------
inline const Matrix Matrix::operator+( const Matrix& other ) const
{
	return Matrix( *this ) += other;
}

// --------------------------------------------------------------------------------------
inline const Matrix Matrix::operator-( const Matrix& other ) const
{
	return Matrix( *this ) -= other;
}

// --------------------------------------------------------------------------------------
inline const Matrix Matrix::operator*( const Matrix& other ) const
{
	Matrix out;
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
		{
			out.m[i][j] = m[i][0] * other.m[0][j] + m[i][1] * other.m[1][j] + m[i][2] * other.m[2][j] + m[i][3] * other.m[3][j];
		}
	}
	return out;
}

// --------------------------------------------------------------------------------------
inline const Matrix Matrix::operator*( float f ) const
{
	return Matrix( *this ) *= f;
}

// --------------------------------------------------------------------------------------
inline const Matrix Matrix::operator/( float f ) const
{
	return Matrix( *this ) /= f;
}

// --------------------------------------------------------------------------------------
inline bool Matrix::operator==( const Matrix& other ) const
{
	return (
		_11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
		_21 == other._21 && _22 == other._22 && _23 == other._23 && _24 == other._24 &&
		_31 == other._31 && _32 == other._32 && _33 == other._33 && _34 == other._34 &&
		_41 == other._41 && _42 == other._42 && _43 == other._43 && _44 == other._44 );
}

// --------------------------------------------------------------------------------------
inline bool Matrix::operator!=( const Matrix& other ) const
{
	return (
		_11 != other._11 || _12 != other._12 || _13 != other._13 || _14 != other._14 ||
		_21 != other._21 || _22 != other._22 || _23 != other._23 || _24 != other._24 ||
		_31 != other._31 || _32 != other._32 || _33 != other._33 || _34 != other._34 ||
		_41 != other._41 || _42 != other._42 || _43 != other._43 || _44 != other._44 );
}

// --------------------------------------------------------------------------------------
inline const Vector3& Matrix::GetTranslation() const
{
	return *reinterpret_cast<const Vector3*>( &_41 );
}

// --------------------------------------------------------------------------------------
inline Vector3& Matrix::GetTranslation()
{
	return *reinterpret_cast<Vector3*>( &_41 );
}

// --------------------------------------------------------------------------------------
inline const Vector3& Matrix::GetX() const
{
	return *reinterpret_cast<const Vector3*>( &_11 );
}

// --------------------------------------------------------------------------------------
inline Vector3& Matrix::GetX()
{
	return *reinterpret_cast<Vector3*>( &_11 );
}

// --------------------------------------------------------------------------------------
inline const Vector3& Matrix::GetY() const
{
	return *reinterpret_cast<const Vector3*>( &_21 );
}

// --------------------------------------------------------------------------------------
inline Vector3& Matrix::GetY()
{
	return *reinterpret_cast<Vector3*>( &_21 );
}

// --------------------------------------------------------------------------------------
inline const Vector3& Matrix::GetZ() const
{
	return *reinterpret_cast<const Vector3*>( &_31 );
}

// --------------------------------------------------------------------------------------
inline Vector3& Matrix::GetZ()
{
	return *reinterpret_cast<Vector3*>( &_31 );
}

// --------------------------------------------------------------------------------------
inline const Matrix operator*( float f, const Matrix& other )
{
	return Matrix( other ) *= f;
}

// --------------------------------------------------------------------------------------
inline Vector4 operator*( Vector4 p, const Matrix& m )
{
	return Vector4( 
		p.x * m._11 + p.y * m._21 + p.z * m._31 + p.w * m._41,
		p.x * m._12 + p.y * m._22 + p.z * m._32 + p.w * m._42,
		p.x * m._13 + p.y * m._23 + p.z * m._33 + p.w * m._43,
		p.x * m._14 + p.y * m._24 + p.z * m._34 + p.w * m._44
		);
}

// --------------------------------------------------------------------------------------
inline Vector4 operator*( const Matrix& m, Vector4 p )
{
	return Vector4( 
		m._11 * p.x + m._12 * p.y + m._13 * p.z + m._14 * p.w,
		m._21 * p.x + m._22 * p.y + m._23 * p.z + m._24 * p.w,
		m._31 * p.x + m._32 * p.y + m._33 * p.z + m._34 * p.w,
		m._41 * p.x + m._42 * p.y + m._43 * p.z + m._44 * p.w
		);
}


// --------------------------------------------------------------------------------------
inline bool IsMatch( Be::Var* value, const Matrix& t )
{
	return (Be::Var*)&t == value;
}

// --------------------------------------------------------------------------------------
inline Matrix IdentityMatrix()
{
	return Matrix(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 );
}

// --------------------------------------------------------------------------------------
inline Matrix TranslationMatrix( float x, float y, float z )
{
	return Matrix(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		x, y, z, 1.f );
}

// --------------------------------------------------------------------------------------
inline Matrix TranslationMatrix( const Vector3& translation )
{
	return Matrix(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		translation.x, translation.y, translation.z, 1.f );
}

// --------------------------------------------------------------------------------------
inline Matrix ScalingMatrix( float sx, float sy, float sz )
{
	return Matrix(
		sx, 0.f, 0.f, 0.f,
		0.f, sy, 0.f, 0.f,
		0.f, 0.f, sz, 0.f,
		0.f, 0.f, 0.f, 1.f );
}

// --------------------------------------------------------------------------------------
inline Matrix ScalingMatrix( const Vector3& scaling )
{
	return Matrix(
		scaling.x, 0.f, 0.f, 0.f,
		0.f, scaling.y, 0.f, 0.f,
		0.f, 0.f, scaling.z, 0.f,
		0.f, 0.f, 0.f, 1.f );
}

// --------------------------------------------------------------------------------------
inline Matrix RotationMatrix( const Quaternion& q )
{
	Matrix out;
	out.m[0][0] = 1.0f - 2.0f * ( q.y * q.y + q.z * q.z );
	out.m[0][1] = 2.0f * ( q.x *q.y + q.z * q.w );
	out.m[0][2] = 2.0f * ( q.x * q.z - q.y * q.w );
	out.m[0][3] = 0.0f;
	out.m[1][0] = 2.0f * ( q.x * q.y - q.z * q.w );
	out.m[1][1] = 1.0f - 2.0f * ( q.x * q.x + q.z * q.z );
	out.m[1][2] = 2.0f * ( q.y *q.z + q.x *q.w );
	out.m[1][3] = 0.0f;
	out.m[2][0] = 2.0f * ( q.x * q.z + q.y * q.w );
	out.m[2][1] = 2.0f * ( q.y *q.z - q.x *q.w );
	out.m[2][2] = 1.0f - 2.0f * ( q.x * q.x + q.y * q.y );
	out.m[2][3] = 0.0f;
	out.m[3][0] = 0.0f;
	out.m[3][1] = 0.0f;
	out.m[3][2] = 0.0f;
	out.m[3][3] = 1.0f;
	return out;
}

// --------------------------------------------------------------------------------------
inline Matrix RotationMatrix( const Vector3& axis, float angle )
{
	Vector3 normal = Normalize( axis );

	float sinAngle = sin( angle );
	float cosAngle = cos( angle );

	Matrix out;
	out.m[0][0] = ( 1.0f - cosAngle ) * normal.x * normal.x + cosAngle;
	out.m[1][0] = ( 1.0f - cosAngle ) * normal.x * normal.y - sinAngle * normal.z;
	out.m[2][0] = ( 1.0f - cosAngle ) * normal.x * normal.z + sinAngle * normal.y;
	out.m[3][0] = 0.f;
	out.m[0][1] = ( 1.0f - cosAngle ) * normal.y * normal.x + sinAngle * normal.z;
	out.m[1][1] = ( 1.0f - cosAngle ) * normal.y * normal.y + cosAngle;
	out.m[2][1] = ( 1.0f - cosAngle ) * normal.y * normal.z - sinAngle * normal.x;
	out.m[3][1] = 0.f;
	out.m[0][2] = ( 1.0f - cosAngle ) * normal.z * normal.x - sinAngle * normal.y;
	out.m[1][2] = ( 1.0f - cosAngle ) * normal.z * normal.y + sinAngle * normal.x;
	out.m[2][2] = ( 1.0f - cosAngle ) * normal.z * normal.z + cosAngle;
	out.m[3][2] = 0.f;
	out.m[0][3] = 0.f;
	out.m[1][3] = 0.f;
	out.m[2][3] = 0.f;
	out.m[3][3] = 1.f;
	return out;
}

// --------------------------------------------------------------------------------------
inline Matrix RotationXMatrix( float angle )
{
	float sinAngle = sin( angle );
	float cosAngle = cos( angle );

	Matrix out;
	out.m[0][0] = 1.0f;
	out.m[0][1] = 0.0f;
	out.m[0][2] = 0.0f;
	out.m[0][3] = 0.0f;
	out.m[1][0] = 0.0f;
	out.m[1][1] = cosAngle;
	out.m[1][2] = sinAngle;
	out.m[1][3] = 0.0f;
	out.m[2][0] = 0.0f;
	out.m[2][1] = -sinAngle;
	out.m[2][2] = cosAngle;
	out.m[2][3] = 0.0f;
	out.m[3][0] = 0.0f;
	out.m[3][1] = 0.0f;
	out.m[3][2] = 0.0f;
	out.m[3][3] = 1.0f;
	return out;
}

// --------------------------------------------------------------------------------------
inline Matrix RotationYMatrix( float angle )
{
	float sinAngle = sin( angle );
	float cosAngle = cos( angle );

	Matrix out;
	out.m[0][0] = cosAngle;
	out.m[0][1] = 0.0f;
	out.m[0][2] = -sinAngle;
	out.m[0][3] = 0.0f;
	out.m[1][0] = 0.0f;
	out.m[1][1] = 1.0f;
	out.m[1][2] = 0.0f;
	out.m[1][3] = 0.0f;
	out.m[2][0] = sinAngle;
	out.m[2][1] = 0.0f;
	out.m[2][2] = cosAngle;
	out.m[2][3] = 0.0f;
	out.m[3][0] = 0.0f;
	out.m[3][1] = 0.0f;
	out.m[3][2] = 0.0f;
	out.m[3][3] = 1.0f;
	return out;
}

// --------------------------------------------------------------------------------------
inline Matrix RotationZMatrix( float angle )
{
	float sinAngle = sin( angle );
	float cosAngle = cos( angle );

	Matrix out;
	out.m[0][0] = cosAngle;
	out.m[0][1] = sinAngle;
	out.m[0][2] = 0.0f;
	out.m[0][3] = 0.0f;
	out.m[1][0] = -sinAngle;
	out.m[1][1] = cosAngle;
	out.m[1][2] = 0.0f;
	out.m[1][3] = 0.0f;
	out.m[2][0] = 0.0f;
	out.m[2][1] = 0.0f;
	out.m[2][2] = 1.0f;
	out.m[2][3] = 0.0f;
	out.m[3][0] = 0.0f;
	out.m[3][1] = 0.0f;
	out.m[3][2] = 0.0f;
	out.m[3][3] = 1.0f;
	return out;
}

// --------------------------------------------------------------------------------------
inline Matrix Transpose( const Matrix& m )
{
	Matrix out;
	out._11 = m._11; out._12 = m._21; out._13 = m._31; out._14 = m._41;
	out._21 = m._12; out._22 = m._22; out._23 = m._32; out._24 = m._42;
	out._31 = m._13; out._32 = m._23; out._33 = m._33; out._34 = m._43;
	out._41 = m._14; out._42 = m._24; out._43 = m._34; out._44 = m._44;
	return out;
}

// --------------------------------------------------------------------------------------
inline float Determinant( const Matrix& m )
{
	float a0 = m._11 * m._22 - m._12 * m._21;
	float a1 = m._11 * m._23 - m._13 * m._21;
	float a2 = m._11 * m._24 - m._14 * m._21;
	float a3 = m._12 * m._23 - m._13 * m._22;
	float a4 = m._12 * m._24 - m._14 * m._22;
	float a5 = m._13 * m._24 - m._14 * m._23;
	float b0 = m._31 * m._42 - m._32 * m._41;
	float b1 = m._31 * m._43 - m._33 * m._41;
	float b2 = m._31 * m._44 - m._34 * m._41;
	float b3 = m._32 * m._43 - m._33 * m._42;
	float b4 = m._32 * m._44 - m._34 * m._42;
	float b5 = m._33 * m._44 - m._34 * m._43;
	return a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
}

// --------------------------------------------------------------------------------------
inline Matrix Inverse( const Matrix& m )
{
	Vector4 v, vec[3];
	Matrix out;

	float det = Determinant( m );
	if( !det )
	{
		out = m;
	}
	else
	{
		for( int i = 0; i < 4; i++ )
		{
			float signedDet = ( i & 1 ) ? -1.f : 1.f;
			signedDet /= det;
			for( int j = 0; j < 4; j++ )
			{
				if( j != i )
				{
					int a = j;
					if( j > i )
					{
						a = a - 1;
					}
					vec[a].x = m.m[j][0];
					vec[a].y = m.m[j][1];
					vec[a].z = m.m[j][2];
					vec[a].w = m.m[j][3];
				}
			}
			v = Cross( vec[0], vec[1], vec[2] );
			out.m[0][i] = signedDet * v.x;
			out.m[1][i] = signedDet * v.y;
			out.m[2][i] = signedDet * v.z;
			out.m[3][i] = signedDet * v.w;
		}
	}
	return out;
}

// --------------------------------------------------------------------------------------
inline bool Inverse( Matrix& out, float& det, const Matrix& m )
{
	Vector4 v, vec[3];

	det = Determinant( m );
	if( !det )
	{
		return false;
	}
	else
	{
		for( int i = 0; i < 4; i++ )
		{
			float signedDet = ( i & 1 ) ? -1.f : 1.f;
			signedDet /= det;
			for( int j = 0; j < 4; j++ )
			{
				if( j != i )
				{
					int a = j;
					if( j > i )
					{
						a = a - 1;
					}
					vec[a].x = m.m[j][0];
					vec[a].y = m.m[j][1];
					vec[a].z = m.m[j][2];
					vec[a].w = m.m[j][3];
				}
			}
			v = Cross( vec[0], vec[1], vec[2] );
			out.m[0][i] = signedDet * v.x;
			out.m[1][i] = signedDet * v.y;
			out.m[2][i] = signedDet * v.z;
			out.m[3][i] = signedDet * v.w;
		}
	}
	return true;
}

// --------------------------------------------------------------------------------------
inline bool Inverse( Matrix& out, const Matrix& m )
{
	float det;
	return Inverse( out, det, m );
}

// --------------------------------------------------------------------------------------
inline Matrix OrthoNormalBasisZ( const Vector3& z )
{
	Matrix out = IdentityMatrix();
	out.GetZ() = Normalize( z );
	if( std::abs( out.GetZ().x ) > 0.99f )
	{
		out.GetX() = Vector3( 0, 1, 0 );
	}
	else
	{
		out.GetX() = Vector3( 1, 0, 0 );
	}
	out.GetY() = Normalize( Cross( out.GetX(), out.GetZ() ) );
	out.GetX() = Cross( out.GetY(), out.GetZ() );
	return out;
}

// --------------------------------------------------------------------------------------
inline Matrix OrthographicProjection( float width, float height, float zNear, float zFar )
{
	Matrix out = IdentityMatrix();
	out.m[0][0] = 2.0f / width;
	out.m[1][1] = 2.0f / height;
	out.m[2][2] = 1.0f / ( zNear - zFar );
	out.m[3][2] = zNear / ( zNear - zFar );
	return out;
}

// --------------------------------------------------------------------------------------
inline Vector3 TransformCoord( const Vector3& coord, const Matrix& transform )
{
	Vector3 out;
	float norm = transform.m[0][3] * coord.x + transform.m[1][3] * coord.y + transform.m[2][3] * coord.z + transform.m[3][3];
	if( norm != 0 )
	{
		out.x = ( coord.x * transform._11 + coord.y * transform._21 + coord.z * transform._31 + transform._41 ) / norm;
		out.y = ( coord.x * transform._12 + coord.y * transform._22 + coord.z * transform._32 + transform._42 ) / norm;
		out.z = ( coord.x * transform._13 + coord.y * transform._23 + coord.z * transform._33 + transform._43 ) / norm;
	}
	else
	{
		out.x = 0.0f;
		out.y = 0.0f;
		out.z = 0.0f;
	}
	return out;
}

// --------------------------------------------------------------------------------------
inline Vector3 TransformNormal( const Vector3& normal, const Matrix& transform )
{
	return Vector3(
		normal.x * transform._11 + normal.y * transform._21 + normal.z * transform._31,
		normal.x * transform._12 + normal.y * transform._22 + normal.z * transform._32,
		normal.x * transform._13 + normal.y * transform._23 + normal.z * transform._33 );
}

// --------------------------------------------------------------------------------------
inline Vector4 Transform( const Vector4& point, const Matrix& transform )
{
	return Vector4(
		point.x * transform._11 + point.y * transform._21 + point.z * transform._31 + point.w * transform._41,
		point.x * transform._12 + point.y * transform._22 + point.z * transform._32 + point.w * transform._42,
		point.x * transform._13 + point.y * transform._23 + point.z * transform._33 + point.w * transform._43,
		point.x * transform._14 + point.y * transform._24 + point.z * transform._34 + point.w * transform._44 );
}

// --------------------------------------------------------------------------------------
inline Vector4 Transform( const Vector3& point, const Matrix& transform )
{
	return Vector4(
		point.x * transform._11 + point.y * transform._21 + point.z * transform._31 + transform._41,
		point.x * transform._12 + point.y * transform._22 + point.z * transform._32 + transform._42,
		point.x * transform._13 + point.y * transform._23 + point.z * transform._33 + transform._43,
		transform._14 + transform._24 + transform._34 + transform._44 );
}

// ----------------------------------------------------------------------------------
inline Vector4 Transform( const Vector2& v, const Matrix& m )
{
	Vector4 out;
	out.x = v.x * m._11 + v.y * m._21 + m._41;
	out.y = v.x * m._12 + v.y * m._22 + m._42;
	out.z = v.x * m._13 + v.y * m._23 + m._43;
	out.w = v.x * m._14 + v.y * m._24 + m._44;
	return out;
}

// --------------------------------------------------------------------------------------
inline void TransformCoords( void* coords, size_t count, size_t stride, const Matrix& transform )
{
	auto stream = static_cast<uint8_t*>( coords );
	for( size_t i = 0; i < count; ++i )
	{
		*reinterpret_cast<Vector3*>( stream ) = TransformCoord( *reinterpret_cast<Vector3*>( stream ), transform );
		stream += stride;
	}
}

// --------------------------------------------------------------------------------------
inline void TransformCoords( Vector3* coords, size_t size, const Matrix& transform )
{
	for( size_t i = 0; i < size; ++i )
	{
		coords[i] = TransformCoord( coords[i], transform );
	}
}

// --------------------------------------------------------------------------------------
template <size_t Size>
inline void TransformCoords( Vector3 (&coords)[Size], const Matrix& transform )
{
	for( size_t i = 0; i < Size; ++i )
	{
		coords[i] = TransformCoord( coords[i], transform );
	}
}

// --------------------------------------------------------------------------------------
template <typename OutIter, typename InIter>
inline void TransformCoords( OutIter dest, InIter first, InIter last, const Matrix& transform )
{
	while( first != last )
	{
		*dest = TransformCoord( *first, transform );
		++dest;
		++first;
	}
}

// --------------------------------------------------------------------------------------
inline Matrix TransformationMatrix(
	const Vector3* scalingCenter,
	const Quaternion* scalingRotation,
	const Vector3* scaling,
	const Vector3* rotationCenter,
	const Quaternion *rotation,
	const Vector3* translation )
{
	Matrix m1, m2, m3, m4, m5, m6, m7, p1, p2, p3, p4, p5;
	Quaternion prc;
	Vector3 psc, pt;

	if( !scalingCenter )
	{
		psc.x = 0.0f;
		psc.y = 0.0f;
		psc.z = 0.0f;
	}
	else
	{
		psc.x = scalingCenter->x;
		psc.y = scalingCenter->y;
		psc.z = scalingCenter->z;
	}
	if( !rotationCenter )
	{
		prc.x = 0.0f;
		prc.y = 0.0f;
		prc.z = 0.0f;
	}
	else
	{
		prc.x = rotationCenter->x;
		prc.y = rotationCenter->y;
		prc.z = rotationCenter->z;
	}
	if( !translation )
	{
		pt.x = 0.0f;
		pt.y = 0.0f;
		pt.z = 0.0f;
	}
	else
	{
		pt.x = translation->x;
		pt.y = translation->y;
		pt.z = translation->z;
	}
	m1 = TranslationMatrix( -psc );
	if( !scalingRotation )
	{
		m2 = IdentityMatrix();
		m4 = IdentityMatrix();
	}
	else
	{
		m4 = RotationMatrix( *scalingRotation );
		m2 = Inverse( m4 );
	}
	if( !scaling )
	{
		m3 = IdentityMatrix();
	}
	else
	{
		m3 = ScalingMatrix( *scaling );
	}
	if( !rotation )
	{
		m6 = IdentityMatrix();
	}
	else
	{
		m6 = RotationMatrix( *rotation );
	}
	m5 = TranslationMatrix( psc.x - prc.x, psc.y - prc.y, psc.z - prc.z );
	m7 = TranslationMatrix( prc.x + pt.x, prc.y + pt.y, prc.z + pt.z );
	return m1 * m2 * m3 * m4 * m5 * m6 * m7;
}

// --------------------------------------------------------------------------------------
inline Matrix TransformationMatrix(
	const Vector3& scaling,
	const Quaternion& rotation,
	const Vector3& translation )
{
	return ScalingMatrix( scaling ) * RotationMatrix( rotation ) * TranslationMatrix( translation );
}

// --------------------------------------------------------------------------------------
inline Matrix Transformation2DMatrix(
	const Vector2* scalingCenter,
	float scalingRotation,
	const Vector2* scaling,
	const Vector2* rotationCenter,
	float rotation,
	const Vector2* translation )
{
	Matrix m1, m2, m3, m4, m5, m6, m7;
	Quaternion prc;
	Vector3 psc, pt;

	if( !scalingCenter )
	{
		psc.x = 0.0f;
		psc.y = 0.0f;
	}
	else
	{
		psc.x = scalingCenter->x;
		psc.y = scalingCenter->y;
	}
	psc.z = 0.0f;
	if( !rotationCenter )
	{
		prc.x = 0.0f;
		prc.y = 0.0f;
	}
	else
	{
		prc.x = rotationCenter->x;
		prc.y = rotationCenter->y;
	}
	prc.z = 0.0f;
	if( !translation )
	{
		pt.x = 0.0f;
		pt.y = 0.0f;
	}
	else
	{
		pt.x = translation->x;
		pt.y = translation->y;
	}
	pt.z = 0.0f;
	m1 = TranslationMatrix( -psc.x, -psc.y, -psc.z );
	if( !scalingRotation )
	{
		m2 = IdentityMatrix();
		m4 = IdentityMatrix();
	}
	else
	{
		m4 = RotationZMatrix( scalingRotation );
		m2 = Inverse( m4 );
	}
	if( !scaling )
	{
		m3 = IdentityMatrix();
	}
	else
	{
		m3 = ScalingMatrix( scaling->x, scaling->y, 1.0f );
	}
	if( !rotation )
	{
		m6 = IdentityMatrix();
	}
	else
	{
		m6 = RotationZMatrix( rotation );
	}
	m5 = TranslationMatrix( psc.x - prc.x, psc.y - prc.y, psc.z - prc.z );
	m7 = TranslationMatrix( prc.x + pt.x, prc.y + pt.y, prc.z + pt.z );
	return m1 * m2 * m3 * m4 * m5 * m6 * m7;
}

// --------------------------------------------------------------------------------------
inline Matrix LookAtMatrix(
	const Vector3& peye,
	const Vector3& pat,
	const Vector3& pup )
{
	Vector3 right, rightn, up, upn, vec, vec2;
	vec2 = pat - peye;
	vec = Normalize( vec2 );
	right = Cross( pup, vec );
	up = Cross( vec, right );
	rightn = Normalize( right );
	upn = Normalize( up );
	Matrix out;
	out.m[0][0] = -rightn.x;
	out.m[1][0] = -rightn.y;
	out.m[2][0] = -rightn.z;
	out.m[3][0] = Dot( rightn, peye );
	out.m[0][1] = upn.x;
	out.m[1][1] = upn.y;
	out.m[2][1] = upn.z;
	out.m[3][1] = -Dot( upn, peye );
	out.m[0][2] = -vec.x;
	out.m[1][2] = -vec.y;
	out.m[2][2] = -vec.z;
	out.m[3][2] = Dot( vec, peye );
	out.m[0][3] = 0.0f;
	out.m[1][3] = 0.0f;
	out.m[2][3] = 0.0f;
	out.m[3][3] = 1.0f;
	return out;
}

// --------------------------------------------------------------------------------------
inline Matrix PerspectiveFovMatrix( float fovy, float aspect, float zn, float zf )
{
	Matrix out = IdentityMatrix();
	out.m[0][0] = 1.0f / ( aspect * tan( fovy / 2.0f ) );
	out.m[1][1] = 1.0f / tan( fovy / 2.0f );
	out.m[2][2] = zf / ( zn - zf );
	out.m[2][3] = -1.0f;
	out.m[3][2] = ( zf * zn ) / ( zn - zf );
	out.m[3][3] = 0.0f;
	return out;
}

// --------------------------------------------------------------------------------------
inline Matrix PerspectiveOffCenterMatrix(
	float l,
	float r,
	float b,
	float t,
	float zn,
	float zf )
{
	Matrix out = IdentityMatrix();
	out.m[0][0] = 2.0f * zn / ( r - l );
	out.m[1][1] = -2.0f * zn / ( b - t );
	out.m[2][0] = 1.0f + 2.0f * l / ( r - l );
	out.m[2][1] = -1.0f - 2.0f * t / ( b - t );
	out.m[2][2] = zf / ( zn - zf );
	out.m[3][2] = ( zn * zf ) / ( zn - zf );
	out.m[2][3] = -1.0f;
	out.m[3][3] = 0.0f;
	return out;
}

// --------------------------------------------------------------------------------------
inline Matrix OrthoMatrix( float w, float h, float zn, float zf )
{
	Matrix out = IdentityMatrix();
	out.m[0][0] = 2.0f / w;
	out.m[1][1] = 2.0f / h;
	out.m[2][2] = 1.0f / ( zn - zf );
	out.m[3][2] = zn / ( zn - zf );
	return out;
}

// --------------------------------------------------------------------------------------
inline Matrix OrthoOffCenterMatrix(
	float l,
	float r,
	float b,
	float t,
	float zn,
	float zf )
{
	Matrix out = IdentityMatrix();
	out.m[0][0] = 2.0f / ( r - l );
	out.m[1][1] = 2.0f / ( t - b );
	out.m[2][2] = 1.0f / ( zn - zf );
	out.m[3][0] = -1.0f - 2.0f *l / ( r - l );
	out.m[3][1] = 1.0f + 2.0f * t / ( b - t );
	out.m[3][2] = zn / ( zn - zf );
	return out;
}

// --------------------------------------------------------------------------------------
inline void Decompose( Vector3& scale, Quaternion& rotation, Vector3& translation, const Matrix& m )
{
	Matrix normalized;
	Vector3 vec;

	/*Compute the scaling part.*/
	vec.x = m.m[0][0];
	vec.y = m.m[0][1];
	vec.z = m.m[0][2];
	scale.x = Length( vec );

	vec.x = m.m[1][0];
	vec.y = m.m[1][1];
	vec.z = m.m[1][2];
	scale.y = Length( vec );

	vec.x = m.m[2][0];
	vec.y = m.m[2][1];
	vec.z = m.m[2][2];
	scale.z = Length( vec );

	/*Compute the translation part.*/
	translation.x = m.m[3][0];
	translation.y = m.m[3][1];
	translation.z = m.m[3][2];

	/*Let's calculate the rotation now*/
	if( ( scale.x == 0.0f ) || ( scale.y == 0.0f ) || ( scale.z == 0.0f ) )
	{
		rotation = Quaternion( 0.0f, 0.0f, 0.0f, 1.0f );
	}
	else
	{
		normalized.m[0][0] = m.m[0][0] / scale.x;
		normalized.m[0][1] = m.m[0][1] / scale.x;
		normalized.m[0][2] = m.m[0][2] / scale.x;
		normalized.m[1][0] = m.m[1][0] / scale.y;
		normalized.m[1][1] = m.m[1][1] / scale.y;
		normalized.m[1][2] = m.m[1][2] / scale.y;
		normalized.m[2][0] = m.m[2][0] / scale.z;
		normalized.m[2][1] = m.m[2][1] / scale.z;
		normalized.m[2][2] = m.m[2][2] / scale.z;

		rotation = RotationQuaternion( normalized );
	}
}
