////////////////////////////////////////////////////////////
//
//    Created:   October 2015
//    Copyright: CCP 2015
//

#include "StdAfx.h"
#include "Tr2GpuUniqueEmitter.h"

Tr2GpuUniqueEmitter::Tr2GpuUniqueEmitter( IRoot* lockObj )
	:Tr2GpuSharedEmitter( lockObj ),
	m_attractorPosition( 0.f, 0.f, 0.f )
{
	m_id = uintptr_t( this ) | ( 1 << ( sizeof( uintptr_t ) - 1 ) );
}

void Tr2GpuUniqueEmitter::GenerateID()
{
}

void Tr2GpuUniqueEmitter::Update( const UpdateArguments& arguments )
{
	if( m_params.attractorStrength != 0.f )
	{
		m_params.attractorPosition = XMVector3TransformCoord( m_attractorPosition, arguments.parentTransform );
		m_params.attractorPosition -= arguments.originShift;
		UpdateHash();
	}
	Tr2GpuSharedEmitter::Update( arguments );
}