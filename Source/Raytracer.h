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

        m_materials.push_back(std::make_shared<Material>(Color(0.0f, 0.4f, 0.0f),       // Ambient
                                                         Color(0.0f, 1.0f, 0.0f),       // Diffuse
                                                         Color(1.0f, 1.0f, 1.0f),       // Specular
                                                         30.0f));                       // Specular Power

        m_materials.push_back(std::make_shared<Material>(Color(0.0f, 0.0f, 0.4f),       // Ambient
                                                         Color(0.0f, 0.0f, 1.0f),       // Diffuse
                                                         Color(1.0f, 1.0f, 1.0f),       // Specular
                                                         30.0f));                       // Specular Power

        m_materials.push_back(std::make_shared<Material>(Color(1.0f, 1.0f, 1.0f),       // Ambient
                                                         Color(1.0f, 1.0f, 1.0f),       // Diffuse
                                                         Color(1.0f, 1.0f, 1.0f),       // Specular
                                                         30.0f));                       // Specular Power

        m_materials.push_back(std::make_shared<Material>(Color(0.25f, 0.25f, 0.25f),    // Ambient
                                                         Color(0.75f, 0.75f, 0.75f),    // Diffuse
                                                         Color(0.1f, 0.1f, 0.1f),       // Specular
                                                         1.0f));                        // Specular Power

        m_lights.push_back(std::make_shared<Light>(Vec3(-5.0f, 10.0f, 5.0f)));          // Position

        m_scene.push_back(std::make_shared<Sphere>(Vec3(0, 0, 0),                       // Center
                                                   4.0f,                                // Radius
                                                   m_materials.at(0)));                 // Material

        m_scene.push_back(std::make_shared<Sphere>(Vec3(-13, 6, -18),                   // Center
                                                   10.0f,                               // Radius
                                                   m_materials.at(1)));                 // Material

        m_scene.push_back(std::make_shared<Sphere>(Vec3(12, 2, -6),                     // Center
                                                   6.0f,                                // Radius
                                                   m_materials.at(2)));                 // Material

        m_scene.push_back(std::make_shared<Sphere>(Vec3(0, -10004.0f, 0),               // Center
                                                   10000.0f,                            // Radius
                                                   m_materials.at(4)));                 // Material

        // Special light visualization sphere is at end of list
#if 1
        m_scene.push_back(std::make_shared<Sphere>(Vec3(12, 0, -6),                     // Center
                                                   1.0f,                                // Radius
                                                   m_materials.at(3)));                 // Material
#endif
    }

    // ----------------------------------------------------------------------------
    void RenderScene()
    {
        const int screenWidth = m_game->ScreenWidth();
        const int screenHeight = m_game->ScreenHeight();

        const float elapsedFrameTime = m_game->GetElapsedTime();
        static float elapsedGameTime = 0.0f;
        elapsedGameTime += elapsedFrameTime;

        // Move light
#if 1
        const float lightDist = 5.0f;
        const float lightHeight = 5.0f;
        const float lightMoveSpeed = 1.0f;
        const Vec3 lightPos(std::cosf(elapsedGameTime * lightMoveSpeed) * lightDist,
                            lightHeight,
                            std::sinf(elapsedGameTime * lightMoveSpeed) * lightDist);
        m_lights[0]->m_pos = lightPos;
#endif

        // Move spheres
#if 1
        m_scene[m_scene.size() - 1]->m_pos = m_lights[0]->m_pos;
#endif

        // Trace each pixel on screen
        for (int y = 0; y < screenHeight; ++y)
        {
            for (int x = 0; x < screenWidth; ++x)
            {
                Ray ray = m_camera.GetRayForScreenPos(x, y, screenWidth, screenHeight);
                Color color = Trace(ray, x, y);
                m_game->Draw(x, y, color.ToPixelColor());
            }
        }
    }

    // ----------------------------------------------------------------------------
    Color Trace(const Ray& ray, const int xScreen, const int yScreen)
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

            const float shadowMultiplier = GetShadowMultiplier(closestHit.m_pos);

            if (shadowMultiplier > 0.0f && shadowMultiplier < 1.0f)
            {
                int x = 0;
            }
            
            float diffuse = 0.0f;
            float specular = 0.0f;
            for (const std::shared_ptr<Light> light : m_lights)
            {
                const Vec3 hitPosToLightDir = (light->m_pos - closestHit.m_pos).GetNormalized();
                const float hitNormalDotLightDir = hitPosToLightDir.Dot(closestHit.m_normal);
                diffuse += fmax(hitNormalDotLightDir, 0.0f);

                const Vec3 lightDirReflected = hitPosToLightDir.GetReflected(closestHit.m_normal);
                const float lightDirReflectedDotEye = lightDirReflected.Dot(-ray.m_dir);
                specular += std::pow(fmax(lightDirReflectedDotEye, 0.0f), hitMaterial->m_specularPower);
            }

            return hitMaterial->m_ambient +
                   (hitMaterial->m_diffuse * diffuse * shadowMultiplier) +
                   (hitMaterial->m_specular * specular * shadowMultiplier);
        }

        return Color(0.3f, 0.75f, 1.0f) * (1.0f - (static_cast<float>(yScreen) / static_cast<float>(m_game->ScreenHeight())));
    }

    // ----------------------------------------------------------------------------
    float GetShadowMultiplier(const Vec3& p) const
    {
        const float lightHalfWidth = 1.0f;
        const int numLightPointsPerAxis = 10;
        const float lightPosDelta = (lightHalfWidth * 2.0f) / static_cast<float>(numLightPointsPerAxis);
        Vec3 lightPos;
        float accum = 0.0f;
        int numAccum = 0;

        for (const std::shared_ptr<Light> light : m_lights)
        {
            for (int xIdx = 0; xIdx < numLightPointsPerAxis; ++xIdx)
            {
                lightPos.m_x = (light->m_pos.m_x - lightHalfWidth) + (lightPosDelta * xIdx);
                for (int yIdx = 0; yIdx < numLightPointsPerAxis; ++yIdx)
                {
                    lightPos.m_y = (light->m_pos.m_y - lightHalfWidth) + (lightPosDelta * yIdx);
                    for (int zIdx = 0; zIdx < numLightPointsPerAxis; ++zIdx)
                    {
                        lightPos.m_z = (light->m_pos.m_z - lightHalfWidth) + (lightPosDelta * zIdx);

                        Vec3 toLightDir = lightPos - p;
                        const float toLightDistSqr = toLightDir.LengthSqr();
                        toLightDir.Normalize();
                        const Ray toLightRay(p, toLightDir);

                        RayHit hit;
                        size_t shapeIdx = 0;
                        bool isInShadown = false;
                        for (const std::shared_ptr<Shape> shape : m_scene)
                        {
                            // Ignore special light visualization sphere at end of scene list
                            if (shapeIdx < m_scene.size() - 1)
                            {
                                hit = shape->Raycast(toLightRay);
                                if (hit.IsValid() && hit.m_distSqr < toLightDistSqr)
                                {
                                    isInShadown = true;
                                    break;
                                }
                            }

                            ++shapeIdx;
                        }

                        if (!isInShadown)
                        {
                            accum += 1.0f;
                        }
                        ++numAccum;
                    }
                }
            }
        }

        if (numAccum > 0)
        {
            return accum / static_cast<float>(numAccum);
        }

        return 1.0f;
    }

    // ----------------------------------------------------------------------------
    Camera m_camera;
    std::vector<std::shared_ptr<Shape>> m_scene;
    std::vector<std::shared_ptr<Material>> m_materials;
    std::vector<std::shared_ptr<Light>> m_lights;
    olc::PixelGameEngine* m_game = nullptr;
};