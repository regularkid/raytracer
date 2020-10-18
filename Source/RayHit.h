#pragma once

#include "Color.h"
#include "Material.h"

class RayHit
{
public:
    // ----------------------------------------------------------------------------
    RayHit() = default;
    RayHit(const Vec3& pos, const Vec3& normal, const float distSqr, const Material* material)
        : m_pos(pos)
        , m_normal(normal)
        , m_distSqr(distSqr)
        , m_material(material)
    {}

    // ----------------------------------------------------------------------------
    bool IsValid() const
    {
        return m_distSqr < FLT_MAX && m_material;
    }

    // ----------------------------------------------------------------------------
    Vec3 m_pos;
    Vec3 m_normal;
    float m_distSqr = FLT_MAX;
    const Material* m_material = nullptr;;
};