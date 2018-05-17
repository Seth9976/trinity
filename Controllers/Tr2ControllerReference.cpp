#include "StdAfx.h"
#include "Tr2ControllerReference.h"


Tr2ControllerReference::Tr2ControllerReference( IRoot* )
{
}

bool Tr2ControllerReference::Initialize()
{
	if( !m_path.empty() )
	{
		m_controller = BeResMan->LoadObject<ITr2Controller>( m_path.c_str() );
	}
	return true;
}

bool Tr2ControllerReference::OnModified( Be::Var* value )
{
	if( IsMatch( value, m_path ) )
	{
		m_controller = nullptr;
		if( !m_path.empty() )
		{
			m_controller = BeResMan->LoadObject<ITr2Controller>( m_path.c_str() );
		}
	}
	return true;
}

void Tr2ControllerReference::Link( IRoot& owner )
{
	if( m_controller )
	{
		m_controller->Link( owner );
	}
}

void Tr2ControllerReference::Unlink()
{
	if( m_controller )
	{
		m_controller->Unlink();
	}
}

void Tr2ControllerReference::Start()
{
	if( m_controller )
	{
		m_controller->Start();
	}
}

void Tr2ControllerReference::Stop()
{
	if( m_controller )
	{
		m_controller->Stop();
	}
}

void Tr2ControllerReference::Update()
{
	if( m_controller )
	{
		m_controller->Update();
	}
}

void Tr2ControllerReference::SetVariable( const char* name, float value )
{
	if( m_controller )
	{
		m_controller->SetVariable( name, value );
	}
}
