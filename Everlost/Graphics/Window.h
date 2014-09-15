#ifndef GRAPHICS_WINDOW_H
#define GRAPHICS_WINDOW_H

#include <list>

#define WIN32_LEAN_AND_MEAN

#include <d3d9.h> 
#include <d3dx9.h> 

#include <boost\foreach.hpp>

enum WindowStates
{
	NOTHING,
	CHANGE_GAME,
	CHANGE_LAUNCHER
};

struct LayerNode;

class Window
{
public:
	virtual ~Window() { BOOST_FOREACH(LayerNode* current, objects) { delete current; } };

	virtual void Render() = 0;

	virtual void Handle_KeyUp(const UINT_PTR& _key) = 0;
	virtual void Handle_MouseUp(const int& _x, const int& _y, const bool& _left_button) = 0;

protected:
	Window() { };

	LayerNode* active;
	std::list<LayerNode*> objects;

private:
	Window(const Window& _other);
	Window& operator=(const Window& _other);
};

#endif