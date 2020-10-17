#pragma once

#include "ray.h"

class Shape
{
public:
    virtual bool RayIntersect(const Ray& ray) { return false; };
};

class Sphere
    : public Shape
{
public:
    // ----------------------------------------------------------------------------
    Sphere(const Vec3& pos, const float radius)
        : m_pos(pos)
        , m_radius(radius)
        , m_radiusSqr(radius * radius)
    {}

    // ----------------------------------------------------------------------------
    bool RayIntersect(const Ray& ray) override
    {
        const Vec3 rayToSphere = m_pos - ray.m_pos;
        const Vec3 closestPointOnRay = ray.m_pos + ray.m_dir*(rayToSphere.Dot(ray.m_dir));
        const float distToSphereSqr = (closestPointOnRay - m_pos).LengthSqr();
        return distToSphereSqr <= m_radiusSqr;
    }

    // ----------------------------------------------------------------------------
    Vec3 m_pos;
    float m_radius = 0.0f;
    float m_radiusSqr = 0.0f;
};