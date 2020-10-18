#pragma once

#include "Vec3.h"

class Light
{
public:
    // ----------------------------------------------------------------------------
    Light() = default;
    Light(const Vec3& pos)
        : m_pos(pos)
    {}

    // ----------------------------------------------------------------------------
    Vec3 m_pos;
};