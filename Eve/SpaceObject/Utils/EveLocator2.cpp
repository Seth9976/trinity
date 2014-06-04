#include "StdAfx.h"
#include "EveLocator2.h"


EveLocator2::EveLocator2( IRoot* lockobj )
{
	D3DXMatrixIdentity( &m_transform );
}