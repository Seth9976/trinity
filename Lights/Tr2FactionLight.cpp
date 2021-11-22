#include "StdAfx.h"
#include "Tr2FactionLight.h"


Tr2FactionLight::Tr2FactionLight( IRoot* lockobj ):
	Tr2Light( lockobj ),
    m_selectedColor( -1 )
{
	m_type = SPOT_LIGHT;
    memset( m_colorSet, 0, sizeof( m_colorSet ) );
}

// INotify
bool Tr2FactionLight::OnModified( Be::Var* value )
{
    SetLightColorFromFactionColor();
    return true;
}

void Tr2FactionLight::SetLightColorFromFactionColor()
{
    if( m_selectedColor >= 0 && m_selectedColor < SOFDataFactionColorChooser::TYPE_MAX )
    {
        m_lightData.color = m_colorSet[m_selectedColor];
    }
}

void Tr2FactionLight::SetInheritProperties( const Color* colorSet )
{
    if (colorSet)
    {
        memcpy( m_colorSet, colorSet, sizeof( m_colorSet ) );
        SetLightColorFromFactionColor();
    }
}

Color Tr2FactionLight::GetSelectedColor() const
{
        return m_lightData.color;
}


void Tr2FactionLight::RenderDebugInfo( ITr2DebugRenderer2& renderer, const Matrix& worldMatrix, const granny_matrix_3x4* bones, size_t boneCount )
{
	auto baseColor = m_lightData.color * m_lightData.brightness;
	baseColor.a = 0.1;
	auto selectedColor = baseColor + Color( 0.0, 0.0, 0.0, 0.2 );

	Matrix lightMatrix = m_boneTransform;
	if( m_lightData.boneIndex >= 0 && m_lightData.boneIndex < boneCount ) {
		TriMatrixCopyFrom3x4( &lightMatrix, &bones[m_lightData.boneIndex] );
	}
    lightMatrix *= worldMatrix;
	
	renderer.DrawSphere( this, lightMatrix, m_lightData.position, m_lightData.radius, 10, Tr2DebugRenderer::Solid, Tr2DebugColor( selectedColor, baseColor ) );
	renderer.DrawSphere( this, lightMatrix, m_lightData.position, m_lightData.innerRadius, 10, Tr2DebugRenderer::Solid, Tr2DebugColor( selectedColor, baseColor) );
}

