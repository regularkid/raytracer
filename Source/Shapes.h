#pragma once

#include <assert.h>
#include <memory>
#include "Material.h"
#include "Ray.h"
#include "RayHit.h"

class Shape
{
public:
    virtual RayHit Trace(const Ray& ray) { return RayHit(); };
};

class Sphere
    : public Shape
{
public:
    // ----------------------------------------------------------------------------
    Sphere(const Vec3& pos, const float radius, const std::shared_ptr<Material> material)
        : m_pos(pos)
        , m_radius(radius)
        , m_radiusSqr(radius * radius)
        , m_material(material)
    {
        assert(m_material);
    }

    // ----------------------------------------------------------------------------
    RayHit Trace(const Ray& ray) override
    {
        const Vec3 rayToSphere = m_pos - ray.m_pos;
        const Vec3 closestPointOnRay = ray.m_pos + ray.m_dir*(rayToSphere.Dot(ray.m_dir));
        const float distToSphereSqr = (closestPointOnRay - m_pos).LengthSqr();
        if (distToSphereSqr < m_radiusSqr)
        {
            return RayHit(distToSphereSqr, m_material.get());
        }

        return RayHit();
    }

    // ----------------------------------------------------------------------------
    Vec3 m_pos;
    float m_radius = 0.0f;
    float m_radiusSqr = 0.0f;
    const std::shared_ptr<Material> m_material;
};