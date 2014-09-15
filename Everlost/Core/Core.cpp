#include "Core\Macro.h"
#include "Core\Core.h"

int main()
{
	Initialize();

	window_state = WindowStates::CHANGE_LAUNCHER;

	MSG message;
	float elapsed_time = 0;
	boost::chrono::steady_clock::time_point now;
	boost::chrono::steady_clock::time_point last_update = boost::chrono::steady_clock::now();
	while(true)
	{
		HandleWindowState();

		now = boost::chrono::steady_clock::now(); boost::chrono::duration<float> difference = now - last_update; last_update = now;
		elapsed_time += difference.count();

		if (PeekMessage(&message, form, 0, 0, PM_REMOVE)) DispatchMessage(&message);
		if (0.020f < elapsed_time)
		{
			game->Update(elapsed_time);
			window->Render();
			elapsed_time = 0.f;
		}

		Sleep(5);
	}
}
