#pragma once

#include <assert.h>
#include <cfloat>
#include <cmath>
#include "Math.h"

class Vec3
{
public:
    // ----------------------------------------------------------------------------
    Vec3() = default;
    Vec3(float x, float y, float z)
        : m_x(x)
        , m_y(y)
        , m_z(z)
    {}

    // ----------------------------------------------------------------------------
    Vec3 operator-() const
    {
        return Vec3(-m_x, -m_y, -m_z);
    }

    // ----------------------------------------------------------------------------
    bool operator==(const Vec3& other) const
    {
        return IS_CLOSE(m_x, other.m_x) &&
               IS_CLOSE(m_y, other.m_y) &&
               IS_CLOSE(m_z, other.m_z);
    }

    // ----------------------------------------------------------------------------
    Vec3 operator+(const Vec3& other) const
    {
        return Vec3(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
    }

    // ----------------------------------------------------------------------------
    Vec3 operator-(const Vec3& other) const
    {
        return Vec3(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
    }

    // ----------------------------------------------------------------------------
    Vec3 operator*(const float scalar) const
    {
        return Vec3(m_x * scalar, m_y * scalar, m_z * scalar);
    }

    // ----------------------------------------------------------------------------
    Vec3 operator/(const float scalar) const
    {
        assert(!IS_ZERO(scalar));
        return Vec3(m_x / scalar, m_y / scalar, m_z / scalar);
    }
    
    // ----------------------------------------------------------------------------
    void operator+=(const Vec3& other)
    {
        m_x += other.m_x;
        m_y += other.m_y;
        m_z += other.m_z;
    }

    // ----------------------------------------------------------------------------
    void operator-=(const Vec3& other)
    {
        m_x -= other.m_x;
        m_y -= other.m_y;
        m_z -= other.m_z;
    }

    // ----------------------------------------------------------------------------
    void operator*=(const float scalar)
    {
        m_x *= scalar;
        m_y *= scalar;
        m_z *= scalar;
    }

    // ----------------------------------------------------------------------------
    void operator/=(const float scalar)
    {
        assert(!IS_ZERO(scalar));
        m_x /= scalar;
        m_y /= scalar;
        m_z /= scalar;
    }

    // ----------------------------------------------------------------------------
    bool IsZero() const
    {
        return IS_ZERO(m_x) && IS_ZERO(m_y) && IS_ZERO(m_z);
    }

    // ----------------------------------------------------------------------------
    float Dot(const Vec3& other) const
    {
        return (m_x * other.m_x) + (m_y * other.m_y) + (m_z * other.m_z);
    }

    // ----------------------------------------------------------------------------
    Vec3 Cross(const Vec3& other) const
    {
        return Vec3((m_y * other.m_z) - (m_z * other.m_y),
                    (m_z * other.m_x) - (m_x * other.m_z),
                    (m_x * other.m_y) - (m_y * other.m_z));
    }

    // ----------------------------------------------------------------------------
    float Length() const
    {
        if (IsZero())
        {
            return 0.0f;
        }

        return sqrt((m_x * m_x) + (m_y * m_y) + (m_z * m_z));
    }

    // ----------------------------------------------------------------------------
    float LengthSqr() const
    {
        return (m_x * m_x) + (m_y * m_y) + (m_z * m_z);
    }

    // ----------------------------------------------------------------------------
    Vec3 GetNormalized() const
    {
        if (IsZero())
        {
            return Vec3();
        }

        const float length = Length();
        return Vec3(m_x / length, m_y / length, m_z / length);
    }

    // ----------------------------------------------------------------------------
    void Normalize()
    {
        if (!IsZero())
        {
            const float length = Length();
            m_x /= length;
            m_y /= length;
            m_z /= length;
        }
    }

    // ----------------------------------------------------------------------------
    void Reflect(const Vec3& origin, const Vec3& axis)
    {
        const Vec3 originToP = *this - origin;
        const float dot = originToP.Dot(axis);
        const Vec3 closestPointOnAxis = origin + (axis * dot);
        const Vec3 toClosestPointOnAxis = closestPointOnAxis - *this;
        *this = *this + (toClosestPointOnAxis * 2.0f);
    }

    // ----------------------------------------------------------------------------
    Vec3 GetReflected(const Vec3& origin, const Vec3& axis) const
    {
        Vec3 t(*this);
        t.Reflect(origin, axis);
        return t;
    }

    // ----------------------------------------------------------------------------
    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_z = 0.0f;
};