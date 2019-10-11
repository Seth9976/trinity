////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#include "StdAfx.h"
#include "Tr2ControllerFloatVariable.h"


Tr2ControllerFloatVariable::Tr2ControllerFloatVariable( IRoot* )
	:m_type( FLOAT ),
	m_value( 0 ),
	m_defaultValue( 0 )
{
}

bool Tr2ControllerFloatVariable::Initialize()
{
	m_value = m_defaultValue;
	return true;
}

const std::string& Tr2ControllerFloatVariable::GetName() const
{
	return m_name;
}

float* Tr2ControllerFloatVariable::GetPointerForParser()
{
	return &m_value;
}

float Tr2ControllerFloatVariable::GetValue() const
{
	return m_value;
}
