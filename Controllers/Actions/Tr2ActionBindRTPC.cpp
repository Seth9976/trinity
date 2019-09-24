////////////////////////////////////////////////////////////
//
//    Created:   September 2019
//    Copyright: CCP 2019
//

#include "StdAfx.h"
#include "ITr2SoundEmitterOwner.h"
#include "Tr2ActionBindRTPC.h"
#include "Tr2ExpressionTermInfo.h"
#include "Controllers/Tr2Controller.h"
#include "Controllers/Tr2ControllerFloatVariable.h"


Tr2ActionBindRTPC::Tr2ActionBindRTPC( IRoot* ):
	m_controller( nullptr ),
	m_value( "" )
{
}

void Tr2ActionBindRTPC::Link( Tr2Controller& controller )
{
	m_controller = &controller;
}

void Tr2ActionBindRTPC::Unlink()
{
	m_controller = nullptr;
	m_evaluator.Clear();
}

void Tr2ActionBindRTPC::Start( Tr2Controller& controller )
{
	controller.RegisterUpdateable( *this );

	if( ITr2SoundEmitterOwnerPtr emitters = BlueCastPtr( controller.GetOwner() ) )
	{
		if( auto emitter = emitters->FindSoundEmitter( m_emitterName.c_str() ) )
		{
			m_emitter = emitter;
		}
	}
}

void Tr2ActionBindRTPC::Stop( Tr2Controller& controller )
{
	controller.UnRegisterUpdateable( *this );
}

void Tr2ActionBindRTPC::Update( Be::Time realTime, Be::Time simTime )
{
	if ( m_emitter && IsExpressionValid() )
	{
		float value = std::get<1>( m_evaluator.Eval() );
		m_emitter->SetRTPC( m_rtpcName.c_str(), value );
	}
}

bool Tr2ActionBindRTPC::OnModified( Be::Var* value )
{
	if( !m_controller )
	{
		return true;
	}
	if( IsMatch( value, m_value ) )
	{
		m_evaluator.SetExpr( m_value.c_str(), *m_controller);
	}
	return true;
}

bool Tr2ActionBindRTPC::IsExpressionValid() const
{
	return m_evaluator.IsExpressionValid();
}

bool Tr2ActionBindRTPC::IsAttrExpressionValid( const char* ) const
{
	return IsExpressionValid();
}

std::vector<Tr2ExpressionTermInfoPtr> Tr2ActionBindRTPC::GetExpressionTermInfo() const
{
	std::vector<Tr2ExpressionTermInfoPtr> result;
	m_evaluator.GetExpressionTermInfo( result );

	if( m_controller )
	{
		auto& variables = m_controller->GetVariables();
		for( auto it = begin( variables ); it != end( variables ); ++it )
		{
			result.push_back( Tr2ExpressionTermInfo::Variable( "Variables", ( *it )->GetName().c_str(), "controller variable" ) );
		}
	}
	return result;
}

BlueStdResult Tr2ActionBindRTPC::EvaluateExpression( const char* expression, float& value ) const
{
	if( !m_controller )
	{
		return BlueStdResult( BLUE_STD_RESULT_RUNTIME_ERROR, "controller needs to be running when evaluating expressions" );
	}
	Tr2ControllerExpression expr;
	auto error = expr.SetExpr( expression, *m_controller);
	if( !error.empty() )
	{
		return BlueStdResult( BLUE_STD_RESULT_VALUE_ERROR, error.c_str() );
	}
	auto result = expr.Eval();
	if( !result.first )
	{
		return BlueStdResult( BLUE_STD_RESULT_RUNTIME_ERROR, "error evaluating expression" );
	}
	value = result.second;
	return BlueStdResult();
}
