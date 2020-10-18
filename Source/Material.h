#pragma once

#include "Color.h"
#include "olcPixelGameEngine.h"

class Material
{
public:
    // ----------------------------------------------------------------------------
    Material(const Color& color)
        : m_color(color)
    {}

    // ----------------------------------------------------------------------------
    Color m_color;
    olc::Pixel m_pixelColor;
};