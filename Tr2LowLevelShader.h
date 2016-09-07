#pragma once
#ifndef Tr2LowLevelShader_h
#define Tr2LowLevelShader_h

#include "Shader/Tr2Shader.h"

// --------------------------------------------------------------------------------------
// Description:
//   Shows where does the code for a low-level shader comes from (cache, source or error
//   shader). Used for debugging.
// SeeAlso:
//   Tr2LowLevelShader, Tr2HighLevelShader
// --------------------------------------------------------------------------------------
enum Tr2ShaderCodeSource
{
	// Shader code comes from error effect
	SS_ERROR_SHADER,
	// Shader code comes from pre-compiled cache file
	SS_FROM_CACHE,
	// Shader code comes from source file
	SS_FROM_SOURCE,
};

// --------------------------------------------------------------------------------------
// Description:
//   Tr2LowLevelShader is a class that sets up the low level RS and SS, and sets up pixel
//   and vertex shaders.
// SeeAlso:
//   Tr2ShaderMaterial, Tr2LowLevelShaderPass
// --------------------------------------------------------------------------------------
BLUE_CLASS( Tr2LowLevelShader ) :
	public Tr2Shader
{
public:
	Tr2LowLevelShader( IRoot* lockobj = NULL );
	~Tr2LowLevelShader();

	EXPOSE_TO_BLUE();

	unsigned int GetPermuteIndex() const { return m_permuteIndex; }
	Tr2EffectDefine* GetCompilerDefines() const { return m_compilerDefines; }
		
	void SetPermuteIndex( unsigned int permuteIndex );
	void SetCodeSource( Tr2ShaderCodeSource codeSource );
	void SetCompilerDefines( Tr2EffectDefine* d );
	void SetEffectPath( const std::string& effectPath );

private:
	// Permute index, derived from situation flag bits
	unsigned int m_permuteIndex;
	// Actual fxc compiler defines
	Tr2EffectDefine* m_compilerDefines;
	// Where does shader source comes from (for debugging)
	Tr2ShaderCodeSource m_codeSource;

	bool m_inErrorState;

	std::string m_effectPath;

#if BLUE_WITH_PYTHON
	static PyObject* PyGetDefines( PyObject* , PyObject* );
#endif
};

TYPEDEF_BLUECLASS( Tr2LowLevelShader );

#endif
