#pragma once

#include <assert.h>
#include <memory>
#include "Material.h"
#include "Ray.h"
#include "RayHit.h"

class Shape
{
public:
    // ----------------------------------------------------------------------------
    Shape() = default;
    Shape(const Vec3& pos)
        : m_pos(pos)
    {}

    // ----------------------------------------------------------------------------
    virtual RayHit Raycast(const Ray& ray) { return RayHit(); };

    // ----------------------------------------------------------------------------
    Vec3 m_pos;
};

class Sphere
    : public Shape
{
public:
    // ----------------------------------------------------------------------------
    Sphere(const Vec3& pos, const float radius, const std::shared_ptr<Material> material)
        : Shape(pos)
        , m_radius(radius)
        , m_radiusSqr(radius * radius)
        , m_material(material)
    {
        assert(m_material);
    }

    // ----------------------------------------------------------------------------
    RayHit Raycast(const Ray& ray) override
    {
        const Vec3 rayToSphere = m_pos - ray.m_pos;
        const float rayToSphereDotDir = rayToSphere.Dot(ray.m_dir);
        if (rayToSphereDotDir > 0.0f)
        {
            const Vec3 closestPointOnRay = ray.m_pos + (ray.m_dir * rayToSphereDotDir);
            const Vec3 sphereToClosestPoint = closestPointOnRay - m_pos;
            const float distToSphereSqr = sphereToClosestPoint.LengthSqr();
            if (distToSphereSqr <= m_radiusSqr)
            {
                // Calculate distance to hit position on surface of sphere
                const float distToClosestPoint = (closestPointOnRay - ray.m_pos).Length();
                const float distClosestPointToSurface = sqrt(m_radiusSqr - distToSphereSqr);
                const float distRayToSurface = distToClosestPoint - distClosestPointToSurface;

                // Surface is in front of ray?
                if (distRayToSurface > 0.0f)
                {
                    // Calculate hit pos/normal
                    Vec3 hitPos = ray.m_pos + (ray.m_dir * distRayToSurface);
                    Vec3 hitNormal = (hitPos - m_pos).GetNormalized();

                    return RayHit(hitPos, hitNormal, distRayToSurface*distRayToSurface, m_material.get());
                }
            }
        }

        return RayHit();
    }

    // ----------------------------------------------------------------------------
    float m_radius = 0.0f;
    float m_radiusSqr = 0.0f;
    const std::shared_ptr<Material> m_material;
};