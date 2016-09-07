////////////////////////////////////////////////////////////
//
//    Created:   June 2010
//    Copyright: CCP 2010
//
#include "StdAfx.h"
#include "Tr2LowLevelShader.h"

using namespace Tr2RenderContextEnum;

// --------------------------------------------------------------------------------------
// Description:
//   Constructor.  Initializes members to default values.
// --------------------------------------------------------------------------------------
Tr2LowLevelShader::Tr2LowLevelShader( IRoot* lockobj ) :
	m_permuteIndex( 0xFFFFFFFF ),
	m_compilerDefines( NULL ),
	m_codeSource( SS_ERROR_SHADER ),
	m_inErrorState( false ),
	m_effectPath()
{	
}

// --------------------------------------------------------------------------------------
// Description:
//   Frees the dynamically-allocated compiler defines array.
// --------------------------------------------------------------------------------------
Tr2LowLevelShader::~Tr2LowLevelShader()
{
	CCP_FREE( m_compilerDefines );
}

// --------------------------------------------------------------------------------------
// Description:
//   Sets the permute index, which is a bitwise OR of the permute bits for this shader
//   configuration
// Arguments:
//   permuteIndex - The permuteIndex of this low-level shader
// --------------------------------------------------------------------------------------
void Tr2LowLevelShader::SetPermuteIndex( unsigned int permuteIndex )
{
	m_permuteIndex = permuteIndex;
}

// --------------------------------------------------------------------------------------
// Description:
//   Sets code source for this shader (for debugging)
// Arguments:
//   codeSource - The code source for this shader
// --------------------------------------------------------------------------------------
void Tr2LowLevelShader::SetCodeSource( Tr2ShaderCodeSource codeSource )
{
	m_codeSource = codeSource;
}

// --------------------------------------------------------------------------------------
// Description:
//   Sets the compiler defines for this low-level shader.  The compiler defines are
//   generated from the situation flags when a low-level shader is bound.
// Arguments:
//   defs - Array of #defines for the effect compiler
// --------------------------------------------------------------------------------------
void Tr2LowLevelShader::SetCompilerDefines( Tr2EffectDefine* defs )
{
	if( m_compilerDefines && ( m_compilerDefines != defs ) )
	{
		CCP_FREE( m_compilerDefines );
	}

	m_compilerDefines = defs;
}

// --------------------------------------------------------------------------------------
// Description:
//   Sets the path to the effect source file
// Arguments:
//   effectPath - The path to the effect source file
// --------------------------------------------------------------------------------------
void Tr2LowLevelShader::SetEffectPath( const std::string& effectPath )
{
	m_effectPath = effectPath;
}
