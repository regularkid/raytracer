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

        m_materials.push_back(std::make_shared<Material>(Color(0.4f, 0.0f, 0.0f),       // Ambient
                                                         Color(1.0f, 0.0f, 0.0f),       // Diffuse
                                                         Color(1.0f, 1.0f, 1.0f),       // Specular
                                                         30.0f));                       // Specular Power

        m_lights.push_back(std::make_shared<Light>(Vec3(-5.0f, 5.0f, 5.0f)));           // Position

        m_scene.push_back(std::make_shared<Sphere>(Vec3(0, 0, 0),                       // Center
                                                   1.0f,                                // Radius
                                                   m_materials.at(0)));                 // Material
    }

    // ----------------------------------------------------------------------------
    void RenderScene()
    {
        const int screenWidth = m_game->ScreenWidth();
        const int screenHeight = m_game->ScreenHeight();

        // Move light
        const float lightDist = 5.0f;
        const float lightHeight = 5.0f;
        const float lightMoveSpeed = 1.0f;
        const float elapsedFrameTime = m_game->GetElapsedTime();
        static float elapsedGameTime = 0.0f;
        elapsedGameTime += elapsedFrameTime;
        const Vec3 lightPos(std::cosf(elapsedGameTime * lightMoveSpeed) * lightDist,
                            lightHeight,
                            std::sinf(elapsedGameTime * lightMoveSpeed) * lightDist);
        m_lights[0]->m_pos = lightPos;

        // Trace each pixel on screen
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
            hit = shape->Raycast(ray);
            if (hit.m_distSqr < closestHit.m_distSqr)
            {
                closestHit = hit;
            }
        }

        // Lighting
        if (closestHit.IsValid())
        {
            const Material* hitMaterial = closestHit.m_material;

            float diffuse = 0.0f;
            float specular = 0.0f;
            for (const std::shared_ptr<Light> light : m_lights)
            {
                const Vec3 hitPosToLightDir = (light->m_pos - closestHit.m_pos).GetNormalized();
                const float hitNormalDotLightDir = hitPosToLightDir.Dot(closestHit.m_normal);
                diffuse += fmax(hitNormalDotLightDir, 0.0f);

                const Vec3 lightDirReflected = hitPosToLightDir.GetReflected(closestHit.m_pos, closestHit.m_normal);
                const float lightDirReflectedDotEye = lightDirReflected.Dot(-ray.m_dir);
                specular += std::pow(fmax(lightDirReflectedDotEye, 0.0f), hitMaterial->m_specularPower);
            }

            return hitMaterial->m_ambient + (hitMaterial->m_diffuse * diffuse) + (hitMaterial->m_specular * specular);
        }

        return Color();
    }

    Camera m_camera;
    std::vector<std::shared_ptr<Shape>> m_scene;
    std::vector<std::shared_ptr<Material>> m_materials;
    std::vector<std::shared_ptr<Light>> m_lights;
    olc::PixelGameEngine* m_game = nullptr;
};