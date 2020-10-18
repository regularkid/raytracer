#pragma once

#include "Color.h"
#include "Material.h"

class RayHit
{
public:
    // ----------------------------------------------------------------------------
    RayHit() = default;
    RayHit(const float distSqr, const Material* material)
        : m_distSqr(distSqr)
        , m_material(material)
    {}

    // ----------------------------------------------------------------------------
    float m_distSqr = FLT_MAX;
    const Material* m_material = nullptr;;
};