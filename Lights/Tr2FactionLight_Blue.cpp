#include "StdAfx.h"
#include "Tr2FactionLight.h"

BLUE_DEFINE( Tr2FactionLight );

const Be::ClassInfo* Tr2FactionLight::ExposeToBlue()
{
    EXPOSURE_BEGIN( Tr2FactionLight, "" )
		MAP_INTERFACE( Tr2FactionLight )
		MAP_INTERFACE( Tr2Light )
        MAP_INTERFACE( INotify )

		MAP_ATTRIBUTE( "name", m_name, "Name so artists dont get confused", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "position", m_lightData.position, "Light position", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "boneIndex", m_lightData.boneIndex, "The bone index that this light is connected to\n:jessica-widget: boneindex", Be::READWRITE | Be::PERSIST | Be::NOTIFY )

		MAP_ATTRIBUTE( "radius", m_lightData.radius, "Light radius", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "innerRadius", m_lightData.innerRadius, "Inner light radius (to mimick a glowing sphere)", Be::READWRITE | Be::PERSIST )

		MAP_ATTRIBUTE_WITH_CHOOSER("factionColor", m_selectedColor, "Light color", Be::READWRITE | Be::PERSIST | Be::NOTIFY | Be::ENUM, SOFDataFactionColorChooser::EveSOFDataFactionColorSetTypeChooser )
        MAP_PROPERTY_READONLY( "selectedColor", GetSelectedColor, "Light color helper" )

		MAP_ATTRIBUTE( "brightness", m_lightData.brightness, "Light brightness (modulates color) for easier animation", Be::READWRITE | Be::PERSIST | Be::NOTIFY )
		MAP_ATTRIBUTE( "noiseAmplitude", m_lightData.noiseAmplitude, "Brightness noise amplitude\n:jessica-group: Noise", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "noiseFrequency", m_lightData.noiseFrequency, "Brightness noise frequency\n:jessica-group: Noise", Be::READWRITE | Be::PERSIST )
		MAP_ATTRIBUTE( "noiseOctaves", m_lightData.noiseOctaves, "Brightness turbulence octaves\n:jessica-group: Noise", Be::READWRITE | Be::PERSIST )
	EXPOSURE_END( )
}