#pragma once

#include "Vec3.h"

class Ray
{
public:
    // ----------------------------------------------------------------------------
    Ray() = default;
    Ray(const Vec3& pos, const Vec3& dir)
        : m_pos(pos)
        , m_dir(dir)
    {}

    // ----------------------------------------------------------------------------
    Vec3 m_pos;
    Vec3 m_dir;
};