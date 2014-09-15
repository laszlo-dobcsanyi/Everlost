#ifndef GRAPHICS_WINDOW_GAME_H
#define GRAPHICS_WINDOW_GAME_H

#include "Graphics\Window.h"

class Character;

class Window_Game
	: public Window
{
public:
	Window_Game();
	~Window_Game();

	void Render();

	void Handle_KeyUp(const UINT_PTR& _key);
	void Handle_MouseUp(const int& _x, const int& _y, const bool& _left_button);

private:
	void DrawFields();
	void DrawObjects();
	void DrawUnits();
	void DrawInterface();

	void DrawLoading();

	Character* GetCharacter(const int& _x, const int& _y);

	Window_Game(const Window_Game& _other);
	Window_Game& operator=(const Window_Game& _other);
};

#endif