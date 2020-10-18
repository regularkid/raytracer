#pragma once

#include "Color.h"
#include "olcPixelGameEngine.h"

class Material
{
public:
    // ----------------------------------------------------------------------------
    Material(const Color& ambient, const Color& diffuse)
        : m_ambient(ambient)
        , m_diffuse(diffuse)
    {}

    // ----------------------------------------------------------------------------
    Color m_ambient;
    Color m_diffuse;
    olc::Pixel m_pixelColor;
};