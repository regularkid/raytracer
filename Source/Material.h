#pragma once

#include "Color.h"
#include "olcPixelGameEngine.h"

class Material
{
public:
    // ----------------------------------------------------------------------------
    Material(const Color& ambient, const Color& diffuse, const Color& specular, const float specularPower)
        : m_ambient(ambient)
        , m_diffuse(diffuse)
        , m_specular(specular)
        , m_specularPower(specularPower)
    {}

    // ----------------------------------------------------------------------------
    Color m_ambient;
    Color m_diffuse;
    Color m_specular;
    float m_specularPower = 0.0f;
    olc::Pixel m_pixelColor;
};