#pragma once

#include "Math.h"
#include "Ray.h"
#include "olcPixelGameEngine.h"

class Camera
{
public:
    // ----------------------------------------------------------------------------
    Camera() = default;
    Camera(const Vec3& pos, const Vec3& dir, const float fov = 90.0f, const float projPlaneDist = 1.0f)
        : m_pos(pos)
        , m_dir(dir)
        , m_fov(fov)
        , m_projPlaneDist(projPlaneDist)
    {
        CalculateProjPlane();
    }

    // ----------------------------------------------------------------------------
    void Init(olc::PixelGameEngine* game)
    {
        m_game = game;
        m_aspectRatio = static_cast<float>(m_game->ScreenWidth()) / static_cast<float>(m_game->ScreenHeight());
    }

    // ----------------------------------------------------------------------------
    void CalculateProjPlane()
    {
        // Assumptions:
        // 1. Square resolution - include aspect ratio if that every changes
        // 2. Camera will never point straight up
        const Vec3 projPlaneCenter = m_pos + (m_dir * m_projPlaneDist);
        const float planeHalfSize = tanf(DEG2RAD(m_fov*0.5f)) * m_projPlaneDist;
        m_projPlaneSize = planeHalfSize * 2.0f;
        m_projPlaneRight = m_dir.Cross(Vec3(0, 1, 0)).GetNormalized();
        m_projPlaneUp = m_projPlaneRight.Cross(m_dir).GetNormalized();
        m_projPlaneTopLeft = (projPlaneCenter - (m_projPlaneRight*planeHalfSize*m_aspectRatio) + (m_projPlaneUp*planeHalfSize));
    }

    // ----------------------------------------------------------------------------
    void SetLookat(const Vec3& pos, const Vec3& lookAtPos)
    {
        m_pos = pos;
        m_dir = (lookAtPos - pos).GetNormalized();
        CalculateProjPlane();
    }

    // ----------------------------------------------------------------------------
    Ray GetRayForScreenPos(int xScreen, int yScreen, int screenWidth, int screenHeight) const
    {
        const float xRatio = (static_cast<float>(xScreen) + 0.5f) / static_cast<float>(screenWidth);
        const float yRatio = (static_cast<float>(yScreen) + 0.5f) / static_cast<float>(screenHeight);
        const Vec3 projPlanePos = m_projPlaneTopLeft + (m_projPlaneRight*xRatio*m_projPlaneSize*m_aspectRatio) - (m_projPlaneUp*yRatio*m_projPlaneSize);
        return Ray(m_pos, (projPlanePos - m_pos).GetNormalized());
    }

    // ----------------------------------------------------------------------------
    Vec3 m_pos;
    Vec3 m_dir;
    float m_aspectRatio = 1.0f;
    float m_fov = 90.0f;
    float m_projPlaneDist = 1.0f;
    float m_projPlaneSize = 0.0f;
    Vec3 m_projPlaneRight;
    Vec3 m_projPlaneUp;
    Vec3 m_projPlaneTopLeft;
    olc::PixelGameEngine* m_game = nullptr;
};