#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Raytracer.h"

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Raytracer";
	}

	bool OnUserCreate() override
	{
		const Vec3 camPos = Vec3(0, 10.0f, 20.0f);
		const Vec3 camLookAt = Vec3(0, 0, 0);
		m_camera.SetLookat(camPos, camLookAt);

		m_raytracer.Init(this, m_camera);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		m_raytracer.RenderScene();

		return true;
	}

private:
	Raytracer m_raytracer;
	Camera m_camera;
};


int main()
{
	Game game;
	if (game.Construct(128, 128, 4, 4))
		game.Start();

	return 0;
}