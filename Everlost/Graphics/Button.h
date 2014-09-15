#ifndef GRAPHICS_BUTTON_H
#define GRAPHICS_BUTTON_H

#include <boost\function.hpp>

#include "Graphics\Layer.h"

class Button
	: public Layer
{
public:
	Button(const int& _x, const int& _y, const int& _width, const int& _height, const std::string& _texture, const boost::function<void (void)>& _callback);
	virtual ~Button();

	virtual void Render();

	virtual void Handle_KeyUp(const UINT_PTR& _key);
	virtual bool Handle_MouseUp(const int& _x, const int& _y, const bool& _left_button);

protected:
	LPDIRECT3DTEXTURE9 texture;
	boost::function<void (void)> callback;

private:
	Button(const Button& _other);
	Button& operator=(const Button& _other); 
};

#endif