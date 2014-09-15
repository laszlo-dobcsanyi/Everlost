#include "Core\Macro.h"
#include "Graphics\Window_Game.h"

#include "Game\Game.h"
#include "Graphics\Resources.h"

#include "Graphics\Layer_Game_Group.h"
#include "Graphics\Layer_Game_Invitation.h"

extern Game* game;
extern Resources* resources;
extern LPDIRECT3DDEVICE9 device;

Window_Game::Window_Game()
	: Window()
{
	#ifdef LOGGING
	Logger::Write(4, "> Game Window constructor..");
	#endif

	LayerNode** layers = new LayerNode*[2];
	layers[0] = new LayerNode(new InvitationLayer(512 - 468 / 2, 64, 468, 199));
	layers[1] = new LayerNode(new GroupLayer(16, 128, 256, 512));

	for (int current = 0; current < 2; ++current)
	{
		layers[current]->next = layers[(current + 1) % 2];
		objects.push_back(layers[current]);
	}

	active = layers[0];
}

void Window_Game::Render()
{
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    device->BeginScene();

	if ((game->hero != 0) && (resources->Ready()))
	{
		DrawFields();
		DrawObjects();
		DrawUnits();

		DrawInterface();
	}
	else
	{
		DrawLoading();
	}

	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

Window_Game::~Window_Game()
{
	#ifdef LOGGING
	Logger::Write(4, "> Game Window destructor..");
	#endif
}

