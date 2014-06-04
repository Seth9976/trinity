#include "StdAfx.h"
#include "EveAnimationData.h"

BLUE_DEFINE( EveAnimation );
const Be::ClassInfo* EveAnimation::ExposeToBlue()
{
    EXPOSURE_BEGIN( EveAnimation, "" )
        MAP_INTERFACE( EveAnimation )

		MAP_ATTRIBUTE( "name", m_name, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "loops", m_loops, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "delay", m_delay, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "speed", m_speed, "", Be::READWRITE | Be::PERSIST )
    EXPOSURE_END()
}


BLUE_DEFINE( EveAnimationCurve );
const Be::ClassInfo* EveAnimationCurve::ExposeToBlue()
{
    EXPOSURE_BEGIN( EveAnimationCurve, "" )
        MAP_INTERFACE( EveAnimationCurve )

		MAP_ATTRIBUTE( "name", m_name, "", Be::READWRITE | Be::PERSIST )
    EXPOSURE_END()
}

Be::VarChooser EveAnimCommandChooser[] =
{
	{ "None", BeCast( ANIM_CMD_NONE ), "None" },
	{ "PlayActivationCurve", BeCast( ANIM_CMD_PLAYACTIVATION ), "PlayActivationCurve" },
	{ "ActivateTurrets", BeCast( ANIM_CMD_ACTIVATE_TURRETS ), "ActivateTurrets" },
	{ "DeactivateTurrets", BeCast( ANIM_CMD_DEACTIVATE_TURRETS ), "DeactivateTurrets" },
	{ "ActivationStrengthZero", BeCast( ANIM_CMD_ACTIVATION_STRENGTH_ZERO ), "Set Activation strength to zero" },
	{ "ActivationStrengthOne", BeCast( ANIM_CMD_ACTIVATION_STRENGTH_ONE ), "Set Activation strength to one" },
	{ "TurnoffBoosters", BeCast( ANIM_CMD_TURNOFF_BOOSTERS ), "Tell non-warp boosters to go off" },
	{ "TurnonBoosters", BeCast( ANIM_CMD_TURNON_BOOSTERS ), "Tell non-warp boosters to start" },
	{ 0 }
};

BLUE_DEFINE( EveAnimationCommand );
const Be::ClassInfo* EveAnimationCommand::ExposeToBlue()
{
    EXPOSURE_BEGIN( EveAnimationCommand, "" )
        MAP_INTERFACE( EveAnimationCommand )

		MAP_ATTRIBUTE_WITH_CHOOSER( "command", m_command, "", Be::READWRITE | Be::PERSIST | Be::ENUM, EveAnimCommandChooser )
		MAP_ATTRIBUTE_WITH_CHOOSER( "data", m_data, "", Be::READWRITE | Be::PERSIST, NULL )
    EXPOSURE_END()
}

BLUE_DEFINE( EveAnimationSequence );
const Be::ClassInfo* EveAnimationSequence::ExposeToBlue()
{
    EXPOSURE_BEGIN( EveAnimationSequence, "" )
        MAP_INTERFACE( EveAnimationSequence )

		MAP_ATTRIBUTE( "animation", m_animation, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "curves", m_curves, "", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "commands", m_commands, "", Be::READWRITE | Be::PERSIST )
    EXPOSURE_END()
}


BLUE_DEFINE( EveTransitionSequence );
const Be::ClassInfo* EveTransitionSequence::ExposeToBlue()
{
    EXPOSURE_BEGIN( EveTransitionSequence, "" )
        MAP_INTERFACE( EveTransitionSequence )

		MAP_ATTRIBUTE( "transitionState", m_transitionState, "", Be::READWRITE | Be::PERSIST )
    EXPOSURE_CHAINTO( EveAnimationSequence )
}
