#pragma once
#ifndef Tr2Shader_h
#define Tr2Shader_h

// Trinity headers
#include "ITr2ShaderState.h"


BLUE_CLASS( Tr2Shader ) :
	public ITr2ShaderState
{
public:
	Tr2Shader( IRoot* lockobj = NULL );
	~Tr2Shader();

	EXPOSE_TO_BLUE();

	/////////////////////////////////////////////////////////////////////////////////////
	// ITr2ShaderState
	virtual uint32_t GetPassCount() const;

	virtual void ApplyAllStateForPass(	uint32_t passIndex,
										Tr2RenderContext &renderContext ) const;
	// Return a bitfield that indicates which shader types are used by this effect/material.
	virtual uint32_t GetShaderTypeMask();

	const Tr2EffectDescription& GetEffectDescription() const;

	// individual accessors if needed.
	virtual void ApplyRenderStates( 
					uint32_t passIndex,
					Tr2RenderContext &renderContext ) const;

	virtual void ApplySamplerStates( 
					uint32_t passIndex, 
					Tr2RenderContextEnum::ShaderType type,
					Tr2RenderContext &renderContext ) const;

	virtual void ApplyShader( 
					uint32_t passIndex, 
					Tr2RenderContextEnum::ShaderType type,
					Tr2RenderContext &renderContext ) const;

	virtual const Tr2EffectConstant* GetConstant( const char* name ) const;
	virtual const Tr2EffectResource* GetResource( const char* name ) const;
	virtual const Tr2EffectParameterAnnotationMap* GetParameterAnnotations( const char* parameterName ) const;


	unsigned int GetSortValue() const;
	
	Tr2EffectDescription& GetEffect();
	void ProcessEffect();

private:
	unsigned int m_sortValue;
	unsigned m_shaderTypeMask;
	Tr2EffectDescription m_effect;
};

TYPEDEF_BLUECLASS( Tr2Shader );

#endif
