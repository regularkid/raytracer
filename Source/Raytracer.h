#pragma once

#include <memory>
#include <vector>
#include "Camera.h"
#include "Shapes.h"
#include "olcPixelGameEngine.h"

class Raytracer
{
public:
    // ----------------------------------------------------------------------------
    void Init(olc::PixelGameEngine* game, const Camera& camera)
    {
        m_game = game;
        m_camera = camera;

        // Temp
        m_scene.push_back(std::make_shared<Sphere>(Vec3(0, 0, 0), 1.0f));
    }

    // ----------------------------------------------------------------------------
    void Trace()
    {
        const int screenWidth = m_game->ScreenWidth();
        const int screenHeight = m_game->ScreenHeight();

        for (int y = 0; y < screenWidth; ++y)
        {
            for (int x = 0; x < screenHeight; ++x)
            {
                Ray ray = m_camera.GetRayForScreenPos(x, y, screenWidth, screenHeight);
                
                // Temp
                if (m_scene[0]->RayIntersect(ray))
                {
                    m_game->Draw(x, y, olc::Pixel(255, 0, 0));
                }
                else
                {
                    m_game->Draw(x, y, olc::Pixel(0, 0, 0));
                }
            }
        }
    }

    Camera m_camera;
    std::vector<std::shared_ptr<Shape>> m_scene;
    olc::PixelGameEngine* m_game = nullptr;
};