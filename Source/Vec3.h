#pragma once

#include <assert.h>
#include <cfloat>
#include <cmath>
#include <smmintrin.h>
#include "Math.h"

// Converted from original float implementation -> SSE using https://github.com/pelletier/vector3
class Vec3
{
public:
    // ----------------------------------------------------------------------------
    Vec3()
        : mmvalue(_mm_setzero_ps())
    {}

    Vec3(float x, float y, float z)
        : mmvalue(_mm_set_ps(0, z, y, x))   // For some reason params are backwards (high slot -> low slot)
    {}

    Vec3(__m128 m)
        : mmvalue(m)
    {}

    // ----------------------------------------------------------------------------
    Vec3 operator-() const
    {
        static const __m128 SIGNMASK = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
        return _mm_xor_ps(mmvalue, SIGNMASK);
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
        return _mm_add_ps(mmvalue, other.mmvalue);
    }

    // ----------------------------------------------------------------------------
    Vec3 operator-(const Vec3& other) const
    {
        return _mm_sub_ps(mmvalue, other.mmvalue);
    }

    // ----------------------------------------------------------------------------
    Vec3 operator*(const float scalar) const
    {
        return _mm_mul_ps(mmvalue, _mm_set1_ps(scalar));
    }

    // ----------------------------------------------------------------------------
    Vec3 operator/(const float scalar) const
    {
        return _mm_div_ps(mmvalue, _mm_set1_ps(scalar));
    }
    
    // ----------------------------------------------------------------------------
    void operator+=(const Vec3& other)
    {
        mmvalue = _mm_add_ps(mmvalue, other.mmvalue);
    }

    // ----------------------------------------------------------------------------
    void operator-=(const Vec3& other)
    {
        mmvalue = _mm_sub_ps(mmvalue, other.mmvalue);
    }

    // ----------------------------------------------------------------------------
    void operator*=(const float scalar)
    {
        mmvalue = _mm_mul_ps(mmvalue, _mm_set1_ps(scalar));
    }

    // ----------------------------------------------------------------------------
    void operator/=(const float scalar)
    {
        mmvalue = _mm_div_ps(mmvalue, _mm_set1_ps(scalar));
    }

    // ----------------------------------------------------------------------------
    bool IsZero() const
    {
        return IS_ZERO(m_x) && IS_ZERO(m_y) && IS_ZERO(m_z);
    }

    // ----------------------------------------------------------------------------
    float Dot(const Vec3& other) const
    {
        // _mm_dp_ps multiplies values together use bit mask for which slots:
        // high 4 bits denote which ones to multiply (we only want first 3 (x,y,z) slots): 7
        // low 4 bits denote which ones to store result in (we just put it in the lowest slot): 1
        // _mm_cvtss_f32 returns the lowest slot value
        return _mm_cvtss_f32(_mm_dp_ps(mmvalue, other.mmvalue, 0x71));
    }

    // ----------------------------------------------------------------------------
    Vec3 Cross(const Vec3& other) const
    {
        return _mm_sub_ps(
            _mm_mul_ps(_mm_shuffle_ps(mmvalue, mmvalue, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(other.mmvalue, other.mmvalue, _MM_SHUFFLE(3, 1, 0, 2))),
            _mm_mul_ps(_mm_shuffle_ps(mmvalue, mmvalue, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(other.mmvalue, other.mmvalue, _MM_SHUFFLE(3, 0, 2, 1)))
        );
    }

    // ----------------------------------------------------------------------------
    float Length() const
    {
        return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(mmvalue, mmvalue, 0x71)));
    }

    // ----------------------------------------------------------------------------
    float LengthSqr() const
    {
        return _mm_cvtss_f32(_mm_dp_ps(mmvalue, mmvalue, 0x71));
    }

    // ----------------------------------------------------------------------------
    Vec3 GetNormalized() const
    {
        return _mm_div_ps(mmvalue, _mm_sqrt_ps(_mm_dp_ps(mmvalue, mmvalue, 0xFF)));
    }

    // ----------------------------------------------------------------------------
    void Normalize()
    {
        *this = _mm_div_ps(mmvalue, _mm_sqrt_ps(_mm_dp_ps(mmvalue, mmvalue, 0xFF)));
    }

    // ----------------------------------------------------------------------------
    void Reflect(const Vec3& axis)
    {
        const float dot = Dot(axis);
        const Vec3 closestPointOnAxis = axis * dot;
        const Vec3 toClosestPointOnAxis = closestPointOnAxis - *this;
        *this = *this + (toClosestPointOnAxis * 2.0f);
    }

    // ----------------------------------------------------------------------------
    Vec3 GetReflected(const Vec3& axis) const
    {
        Vec3 t(*this);
        t.Reflect(axis);
        return t;
    }

    // ----------------------------------------------------------------------------
    union
    {
        struct { float m_x, m_y, m_z; };
        __m128 mmvalue;
    };
};