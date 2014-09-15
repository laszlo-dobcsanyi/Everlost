#ifndef GRAPHICS_WINDOW_LAUNCHER_H
#define GRAPHICS_WINDOW_LAUNCHER_H

#include "Graphics\Button.h"
#include "Graphics\TextBox.h"
#include "Graphics\Window.h"

class Window_Launcher
	: public Window
{
public:
	Window_Launcher();
	~Window_Launcher();

	void Render();

	void Handle_KeyUp(const UINT_PTR& _key);
	void Handle_MouseUp(const int& _x, const int& _y, const bool& _left_button);

private:
	RECT surface;
	ID3DXFont* font;
	LPDIRECT3DTEXTURE9 background;

	TextBox* UsernameBox;
	TextBox* PasswordBox;

	void Process_LoginButton();
	void Process_RegistrateButton();

	Window_Launcher(const Window_Launcher& _other);
	Window_Launcher& operator=(const Window_Launcher& _other);
};

#endif