////////////////////////////////////////////////////////////////////////////////
//
// Created:		December 2021
// Copyright:	CCP 2021
//

#include "StdAfx.h"
#include "Tr2PPDepthOfFieldEffect.h"

BLUE_DEFINE( Tr2PPDepthOfFieldEffect );

const Be::VarChooser BokehShapeChooser[] = {
	{ "Disk", BeCast( Tr2Bokeh::Disk ), "A disk shape" },
	{ "Rectangle", BeCast( Tr2Bokeh::Rectangle ), "A Rectangle shape" },
	{ "Triangle", BeCast( Tr2Bokeh::Triangle ), "A Triangle shape" },
	{ 0 }
};

BLUE_REGISTER_ENUM_EX( "BokehShapeType", Tr2Bokeh::Shape, BokehShapeChooser, ENUM_REG_ENUM_OBJECT_ON_MODULE );

const Be::ClassInfo* Tr2PPDepthOfFieldEffect::ExposeToBlue()
{
	EXPOSURE_BEGIN( Tr2PPDepthOfFieldEffect, "" )
		MAP_INTERFACE( Tr2PPEffect )

		MAP_ATTRIBUTE( "focalDistance", m_focalDistance, "The distance from the camera to the focal plane. Any object that lies on the focal plane is fully in-focus", Be::READWRITE | Be::PERSIST | Be::NOTIFY )
		MAP_ATTRIBUTE( "focalLength", m_focalLength, "How far an object can be from the focal plane before it goes completely out-of-focus", Be::READWRITE | Be::PERSIST | Be::NOTIFY )
		MAP_ATTRIBUTE( "scale", m_scale, "A value that scales the blur kernel", Be::READWRITE | Be::PERSIST | Be::NOTIFY )
		MAP_ATTRIBUTE_WITH_CHOOSER( "bokehShape", m_bokehShape, "What is the shape of the bokeh", Be::READWRITE | Be::PERSIST | Be::NOTIFY | Be::ENUM, BokehShapeChooser );
		
	EXPOSURE_CHAINTO( Tr2PPEffect )
}
