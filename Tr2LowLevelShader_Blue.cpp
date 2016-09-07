////////////////////////////////////////////////////////////
//
//    Created:   June 2010
//    Copyright: CCP 2010
//
#include "StdAfx.h"
#include "Tr2LowLevelShader.h"

static Be::VarChooser Tr2ShaderCodeSourceChooser[] =
{
	{
		"ERROR_SHADER",     
		BeCast( SS_ERROR_SHADER ),     
		"Error shader"
	},
	{
		"FROM_CACHE",     
		BeCast( SS_FROM_CACHE ),     
		"Loaded from pre-compiled cache"
	},
	{
		"FROM_SOURCE",     
		BeCast( SS_FROM_SOURCE ),     
		"Loaded from source code"
	},
	{ 0 }
};

BLUE_REGISTER_ENUM_EX( 
    "Tr2ShaderCodeSource", 
	Tr2ShaderCodeSource, 
    Tr2ShaderCodeSourceChooser,
    ENUM_REG_ENUM_OBJECT_ON_MODULE
);


BLUE_DEFINE( Tr2LowLevelShader );

#if BLUE_WITH_PYTHON
PyObject* Tr2LowLevelShader::PyGetDefines( PyObject* self, PyObject* args )
{
	Tr2LowLevelShader* This = BluePythonCast<Tr2LowLevelShader*>( self );

	std::string result;

	if( This->m_compilerDefines )
	{
		Tr2EffectDefine* scan = This->m_compilerDefines;
		while( scan->name && scan->value )
		{
			result += scan->name;
			result += " = ";
			result += scan->value;
			result += '\n';

			++scan;
		}
	}

	if( result.empty() )
	{
		Py_RETURN_NONE;
	}
	
    return PyString_FromStringAndSize( &result[0], result.size() );
}
#endif

const Be::ClassInfo* Tr2LowLevelShader::ExposeToBlue()
{
	EXPOSURE_BEGIN( Tr2LowLevelShader, "" )
		MAP_INTERFACE(Tr2LowLevelShader)
		
	MAP_ATTRIBUTE
	(
		"permuteIndex",
		m_permuteIndex,
		"The permutation index for this shader",
		Be::READ 
	);
		
	MAP_ATTRIBUTE_WITH_CHOOSER
	(
		"codeSource",
		m_codeSource,
		"Where does the code for this shader come from (trinity.Tr2ShaderCodeSource)",
		Be::READ | Be::ENUM,
		Tr2ShaderCodeSourceChooser
	);

	MAP_METHOD
	(
		"GetDefines",
		PyGetDefines,
		"Returns a string holding all the defines used in this\n"
		"shader, and their values. Ie a debug friendly version\n"
		"of permuteIndex"
	)


	EXPOSURE_END()
}