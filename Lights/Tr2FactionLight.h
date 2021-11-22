//    Created:   November 2021
//    Copyright: CCP 2021

#pragma once
#include "Tr2Light.h"
#include "Eve/SpaceObjectFactory/EveSOFData.h"

class Tr2LightManager;

BLUE_CLASS( Tr2FactionLight ):
	public Tr2Light
{
public:
	EXPOSE_TO_BLUE();
    Tr2FactionLight( IRoot* lockobj = nullptr );

    bool OnModified( Be::Var* value ) override;

    void SetInheritProperties( const Color* colorSet );
    void RenderDebugInfo( ITr2DebugRenderer2& renderer, const Matrix& worldMatrix, const granny_matrix_3x4* bones = nullptr, size_t boneCount = 0) override;
    Color GetSelectedColor() const;

private:
    void SetLightColorFromFactionColor();
    int m_selectedColor;
    Color m_colorSet[SOFDataFactionColorChooser::TYPE_MAX];
};

TYPEDEF_BLUECLASS( Tr2FactionLight );