#pragma once

#include "Color.h"

class RayHit
{
public:
    // ----------------------------------------------------------------------------
    RayHit() = default;
    RayHit(const float distSqr, const Color& color)
        : m_distSqr(distSqr)
        , m_color(color)
    {}

    // ----------------------------------------------------------------------------
    float m_distSqr = FLT_MAX;
    Color m_color;
};