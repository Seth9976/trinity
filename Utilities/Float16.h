#pragma once

#ifdef _WIN32

struct Float_16 : public D3DXFLOAT16
{
	Float_16()
	{
	}

	Float_16( const D3DXFLOAT16& other )
		: D3DXFLOAT16( other )
	{
	}

	Float_16( float other )
		: D3DXFLOAT16( other )
	{
	}

	operator float() const
	{
		return const_cast<Float_16*>( this )->D3DXFLOAT16::operator FLOAT();
	}
};

struct Vector2_16 : public D3DXVECTOR2_16F
{
	Vector2_16()
	{
	}

	Vector2_16( Float_16 x, Float_16 y )
		:D3DXVECTOR2_16F( x, y )
	{
	}

	operator Vector2() const
	{
		return Vector2( Float_16( x ), Float_16( y ) );
	}
};

struct Vector3_16 : public D3DXVECTOR3_16F
{
	Vector3_16()
	{
	}

	Vector3_16( Float_16 x, Float_16 y, Float_16 z )
		:D3DXVECTOR3_16F( x, y, z )
	{
	}

	Vector3_16( const Vector3& other )
		:D3DXVECTOR3_16F( other )
	{
	}

	operator Vector3() const
	{
		return Vector3( &x );
	}
};

#else

#include "CcpMath/include/Float16.h"
typedef CcpMath::Float16 Float_16;
using CcpMath::Vector2_16;

#endif