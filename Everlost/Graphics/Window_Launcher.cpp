#include "Core\Macro.h"
#include "Graphics\Window_Launcher.h"

#include <boost\bind.hpp>

#include "Core\Connection.h"

extern Connection* connection;
extern LPDIRECT3DDEVICE9 device;

Window_Launcher::Window_Launcher()
	: Window()
{
	#ifdef LOGGING
	Logger::Write(4, "> Launcher Window constructor..");
	#endif

	surface.left = 512 -128; surface.right = 512 + 128; surface.top = 96; surface.bottom = 96 + 64;
	D3DXCreateFont(device, 76, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, "Arial Black", &font);
	D3DXCreateTextureFromFileEx(device, "data\\interface\\launcher_background.png", D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &background);

	UsernameBox = new TextBox(512 - 256 + 32, 512 - 128, 512 - 64, 32, "Username: ", false, 42, "Arial Black", 0xAA000000, "lacko");
	PasswordBox = new TextBox(512 - 256 + 32, 512 - 64 , 512 - 64, 32, "Password: ", true, 42, "Arial Black", 0xAA000000, "atya");

	LayerNode** layers = new LayerNode*[4];
	layers[0] = new LayerNode(UsernameBox);
	layers[1] = new LayerNode(PasswordBox);
	layers[2] = new LayerNode(new Button(1024 - 175 - 64, 768 - 44 - 64, 175, 44, "data\\interface\\button_login.png", boost::bind(&Window_Launcher::Process_LoginButton, this)));
	layers[3] = new LayerNode(new Button(64, 768 - 44 - 64, 175, 44, "data\\interface\\button_registrate.png", boost::bind(&Window_Launcher::Process_RegistrateButton, this)));

	for (int current = 0; current < 4; ++current)
	{
		layers[current]->next = layers[(current + 1) % 4];
		objects.push_back(layers[current]);
	}

	active = layers[0];
}

void Window_Launcher::Render()
{
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    device->BeginScene();

	LPD3DXSPRITE sprite;
	D3DXCreateSprite(device, &sprite);
	sprite->Begin(0);

	//D3DXMATRIX mat;
	//D3DXMatrixTransformation2D(&mat, NULL, 0., &D3DXVECTOR2(1024. / 1920., 768. / 1080.), NULL, 0., NULL);
	//sprite->SetTransform(&mat);
	sprite->Draw(background, NULL, NULL, &D3DXVECTOR3(0, 0, 0), 0xFFFFFFFF);
	font->DrawText(sprite, "Everlost", -1, &surface, DT_LEFT|DT_NOCLIP, 0xCC000000);
	
	sprite->End();
	sprite->Release();

	BOOST_FOREACH(LayerNode* current, objects)
	{
		if (current->layer->Visible())
			current->layer->Render();
	}

	device->EndScene();
	device->Present(NULL, NULL, NULL, NULL);
}

void Window_Launcher::Handle_KeyUp(const UINT_PTR& _key)
{
	if (_key == VK_TAB)
	{
		active = active->next;
		return;
	}

	active->layer->Handle_KeyUp(_key);
}

void Window_Launcher::Handle_MouseUp(const int& _x, const int& _y, const bool& _left_button)
{
	BOOST_FOREACH(LayerNode* current, objects)
	{
		if (current->layer->Visible())
			if (current->layer->Collide(_x, _y))
			{
				active = current;
				if (active->layer->Handle_MouseUp(_x, _y, _left_button)) break;
			}
	}
}

void Window_Launcher::Process_LoginButton()
{
	connection->Send(Connection::ClientCommand::LOGIN, UsernameBox->Text() + ";" + PasswordBox->Text());
}

void Window_Launcher::Process_RegistrateButton()
{
	/// TODO add registration form
}


Window_Launcher::~Window_Launcher()
{
	#ifdef LOGGING
	Logger::Write(4, "> Launcher Window destructor..");
	#endif

	background->Release();
}

