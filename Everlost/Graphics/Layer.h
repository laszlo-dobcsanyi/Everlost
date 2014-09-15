#ifndef GRAPHICS_LAYER_H
#define GRAPHICS_LAYER_H

#define WIN32_LEAN_AND_MEAN

#include <d3d9.h> 
#include <d3dx9.h> 

class Layer;

struct LayerNode
{
	Layer* layer;
	LayerNode* next;

	LayerNode(Layer* _layer) : layer(_layer), next(0) { };
};


class Layer
{
public:
	virtual ~Layer() { };

	virtual void Render() = 0;

	virtual void Show() { visible = true; };
	virtual void Hide() { visible = false; };
	virtual bool Visible() { return visible; };
	virtual bool Collide(const int& _x, const int& _y) { if ((surface.left <= _x && _x <= surface.right) && (surface.top <= _y && _y <= surface.bottom)) return true; return false; };

	virtual void Handle_KeyUp(const UINT_PTR& _key) = 0;
	virtual bool Handle_MouseUp(const int& _x, const int& _y, const bool& _left_button) = 0;

protected:
	RECT surface;
	bool visible;
	
	Layer(const int& _x, const int& _y, const int& _width, const int& _height) : visible(true) { surface.left = _x; surface.top = _y; surface.right = _x + _width; surface.bottom = _y + _height; };

private:
	Layer(const Layer& _other);
	Layer& operator=(const Layer& _other);
};

#endif