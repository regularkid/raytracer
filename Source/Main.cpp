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
		m_camera.Init(this);
		m_camera.SetLookat(camPos, camLookAt);

		m_raytracer.Init(this, m_camera);

		using namespace std::chrono;
		time_point<steady_clock> startTime = high_resolution_clock::now();
		m_raytracer.RenderScene();
		duration<float> duration = high_resolution_clock::now() - startTime;
		printf("Total Time: %.02f sec", duration.count());

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		return true;
	}

private:
	Raytracer m_raytracer;
	Camera m_camera;
};


int main()
{
	Game game;
	if (game.Construct(854, 480, 1, 1))
	//if (game.Construct(128, 128, 4, 4))
	{
		game.Start();
	}

	return 0;
}