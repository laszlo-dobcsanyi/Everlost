#ifndef CORE_CORE_H
#define CORE_CORE_H

#define WIN32_LEAN_AND_MEAN

#include <d3d9.h> 
#include <d3dx9.h> 

#include "Game\Game.h"
#include "Core\Connection.h"
#include "Graphics\Window.h"
#include "Graphics\Resources.h"
#include "Graphics\Window_Game.h"
#include "Graphics\Window_Launcher.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

Game* game;

HWND form;
Window* window;
WindowStates window_state;
Resources* resources;
LPDIRECT3DDEVICE9 device;

Connection* connection;

LRESULT CALLBACK HandleForm(HWND han_Wind, UINT uint_Message, WPARAM wParam, LPARAM lParam)
{   
	switch(uint_Message)
    {
        case WM_KEYUP:
			window->Handle_KeyUp(wParam);
			break;
		
		case WM_LBUTTONUP:
			window->Handle_MouseUp(LOWORD(lParam), HIWORD(lParam), true);
			break;

		case WM_RBUTTONUP:
			window->Handle_MouseUp(LOWORD(lParam), HIWORD(lParam), false);
			break;
	}

	return DefWindowProc(han_Wind, uint_Message, wParam, lParam);
}

void HandleWindowState()
{
	switch (window_state)
	{
		case CHANGE_GAME:
			{
				Window* game_window = new Window_Game();
				Window* current_window = window;
				window = game_window;
				delete current_window;

				window_state = WindowStates::NOTHING;
			}
			break;

		case CHANGE_LAUNCHER:
			{
				Window* launcher_window = new Window_Launcher();
				Window* current_window = window;
				window = launcher_window;
				delete current_window;

				window_state = WindowStates::NOTHING;
			}
			break;
	}
}

HWND CreateForm(LPCTSTR str_Title, int int_Width, int int_Height)
{
	WNDCLASSEX wnd_Structure;

    wnd_Structure.cbSize = sizeof(WNDCLASSEX);
    wnd_Structure.style = CS_HREDRAW | CS_VREDRAW;
    wnd_Structure.lpfnWndProc = HandleForm;
    wnd_Structure.cbClsExtra = 0;
    wnd_Structure.cbWndExtra = 0;
    wnd_Structure.hInstance = GetModuleHandle(NULL);
    wnd_Structure.hIcon = NULL;
    wnd_Structure.hCursor = NULL;
    wnd_Structure.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wnd_Structure.lpszMenuName = NULL;
    wnd_Structure.lpszClassName = "WindowClassName";
    wnd_Structure.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

    RegisterClassEx(&wnd_Structure);

    return CreateWindowEx(WS_EX_CONTROLPARENT, "WindowClassName", str_Title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, 600, 0, int_Width, int_Height, NULL, NULL, GetModuleHandle(NULL), NULL);
}

LPDIRECT3DDEVICE9 CreateDevice(HWND han_WindowToBindTo)
{
    LPDIRECT3D9 p_dx_Object;
    LPDIRECT3DDEVICE9 p_dx_Device;

    p_dx_Object = Direct3DCreate9(D3D_SDK_VERSION);
    if (p_dx_Object == NULL)
    {
        MessageBox(han_WindowToBindTo,"DirectX Runtime library not installed!","InitializeDevice()",MB_OK);
    }

    D3DPRESENT_PARAMETERS dx_PresParams;
		ZeroMemory( &dx_PresParams, sizeof(dx_PresParams) );
		dx_PresParams.Windowed = TRUE;
		dx_PresParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		dx_PresParams.BackBufferFormat = D3DFMT_UNKNOWN;

	if (FAILED(p_dx_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, han_WindowToBindTo, D3DCREATE_HARDWARE_VERTEXPROCESSING, &dx_PresParams, &p_dx_Device)))
		if (FAILED(p_dx_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, han_WindowToBindTo, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &dx_PresParams, &p_dx_Device)))
			MessageBox(han_WindowToBindTo,"Failed to create even the reference device!","InitializeDevice()",MB_OK);

	p_dx_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	p_dx_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	p_dx_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    return p_dx_Device;
}

void Initialize()
{
	#ifdef LOGGING
	Logger::Write(0, "> Initializing..");
	Logger::Write(0, "\t> Creating Form..");
	#endif

	form = CreateForm("Everlost - Launcher", 1024 + 16, 768 + 39);

	#ifdef LOGGING
	Logger::Write(0, "\t> Creating Device..");
	#endif

	device = CreateDevice(form);

	#ifdef LOGGING
	Logger::Write(0, "\t> Creating Game..");
	#endif

	game = new Game();

	#ifdef LOGGING
	Logger::Write(0, "\t> Creating Resources..");
	#endif

	resources = new Resources();

	#ifdef LOGGING
	Logger::Write(0, "\t> Creating Connection..");
	#endif

	connection = new Connection();

	#ifdef LOGGING
	Logger::Write(0, "< Initialized!");
	#endif
}

#endif