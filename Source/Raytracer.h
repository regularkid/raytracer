#pragma once

#include <memory>
#include <vector>
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "RayHit.h"
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

        m_materials.push_back(std::make_shared<Material>(Color(0.1f, 0.0f, 0.0f), Color(1.0f, 0.0f, 0.0f)));

        m_lights.push_back(std::make_shared<Light>(Vec3(-5.0f, 5.0f, 5.0f)));

        m_scene.push_back(std::make_shared<Sphere>(Vec3(0, 0, 0), 1.0f, m_materials.at(0)));
    }

    // ----------------------------------------------------------------------------
    void RenderScene()
    {
        const int screenWidth = m_game->ScreenWidth();
        const int screenHeight = m_game->ScreenHeight();

        for (int y = 0; y < screenWidth; ++y)
        {
            for (int x = 0; x < screenHeight; ++x)
            {
                Ray ray = m_camera.GetRayForScreenPos(x, y, screenWidth, screenHeight);
                Color color = Trace(ray);
                m_game->Draw(x, y, color.ToPixelColor());
            }
        }
    }

    // ----------------------------------------------------------------------------
    Color Trace(const Ray& ray)
    {
        // Get color of closest hit
        RayHit hit;
        RayHit closestHit;
        for (const std::shared_ptr<Shape> shape : m_scene)
        {
            hit = shape->Trace(ray);
            if (hit.m_distSqr < closestHit.m_distSqr)
            {
                closestHit = hit;
            }
        }

        // Lighting
        const Material* hitMaterial = closestHit.m_material;
        if (hitMaterial)
        {
            float lightIntensity = 0.0f;
            for (const std::shared_ptr<Light> light : m_lights)
            {
                // Light -> hit pos 
            }

            Color diffuse = hitMaterial->m_diffuse;
            
            return hitMaterial->m_ambient + diffuse;
        }

        return Color();
    }

    Camera m_camera;
    std::vector<std::shared_ptr<Shape>> m_scene;
    std::vector<std::shared_ptr<Material>> m_materials;
    std::vector<std::shared_ptr<Light>> m_lights;
    olc::PixelGameEngine* m_game = nullptr;
};