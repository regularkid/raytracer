#pragma once

#include <algorithm>
#include "Vec3.h"
#include "olcPixelGameEngine.h"

class Color
    : public Vec3
{
public:
    // ----------------------------------------------------------------------------
    Color() = default;
    Color(const float r, const float g, const float b)
        : Vec3(r, g, b)
    {}
    Color(const Vec3& v)
        : Vec3(v)
    {}

    // ----------------------------------------------------------------------------
    olc::Pixel ToPixelColor() const
    {
        const int r = min(max(static_cast<int>(m_x * 255.0f), 0), 255);
        const int g = min(max(static_cast<int>(m_y * 255.0f), 0), 255);
        const int b = min(max(static_cast<int>(m_z * 255.0f), 0), 255);
        return olc::Pixel(r, g, b);
    }
};