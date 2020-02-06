////////////////////////////////////////////////////////////
//
//    Created:   February 2019
//    Copyright: CCP 2019
//

#include "StdAfx.h"
#include "Tr2SpotLight.h"
#include "Tr2DebugRenderer.h"

#include "Tr2LightManager.h"
#include "Include/TriMath.h"

Tr2SpotLight::Tr2SpotLight( IRoot* lockobj ):
	Tr2Light( lockobj )
{
	m_type = SPOT_LIGHT;
}

void Tr2SpotLight::RenderDebugInfo( ITr2DebugRenderer2& renderer, const Matrix& worldMatrix )
{
	auto baseColor = m_lightData.color * m_lightData.brightness;
	baseColor.a = 0.1;
	auto selectedColor = baseColor + Color( 0.0, 0.0, 0.0, 0.1 );
	
	auto lightRotation = RotationMatrix( Normalize( m_lightData.rotation * RotationQuaternion( worldMatrix ) ) );
	auto translationMatrix = TranslationMatrix( worldMatrix.GetTranslation() );
	float scaling = XMVectorGetX( XMVectorAdd( XMVector3LengthEst( worldMatrix.GetX() ),
		XMVectorAdd( XMVector3LengthEst( worldMatrix.GetY() ), XMVector3LengthEst( worldMatrix.GetZ() ) ) ) ) / 3.f;

	Vector3 outerEnd = Transform( Vector3( 0.0, 0.0, m_lightData.radius * scaling ), lightRotation ).GetXYZ();
	outerEnd = TransformCoord( outerEnd + m_lightData.position, translationMatrix );

	Vector3 innerEnd = Transform( Vector3( 0.0, 0.0, m_lightData.innerRadius * scaling ), lightRotation ).GetXYZ();
	innerEnd = TransformCoord( innerEnd + m_lightData.position, translationMatrix );

	Vector3 start = TransformCoord( m_lightData.position, translationMatrix );
	float outerConeRadius = tan( TRI_2PI * m_lightData.outerAngle / 360.f ) * m_lightData.radius * scaling;
	float innerConeRadius = tan( TRI_2PI * m_lightData.innerAngle / 360.f ) * m_lightData.innerRadius * scaling;
	renderer.DrawCone( this, outerEnd, start, outerConeRadius, 10, Tr2DebugRenderer::Solid, Tr2DebugColor( selectedColor, baseColor ) );
	renderer.DrawCone( this, innerEnd, start, innerConeRadius, 10, Tr2DebugRenderer::Solid, Tr2DebugColor( selectedColor, baseColor ) );
}