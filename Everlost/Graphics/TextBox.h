#ifndef GRAPHICS_TEXTBOX_H
#define GRAPHICS_TEXTBOX_H

#include "Graphics\Layer.h"

class TextBox
	: public Layer
{
public:
	TextBox(const int& _x, const int& _y, const int& _width, const int& _height, const std::string& _label, bool _masked, int _font_height, LPCSTR _font_name, D3DCOLOR _font_color, const std::string _text);
	virtual ~TextBox();

	virtual void Render();
	
	virtual void Handle_KeyUp(const UINT_PTR& _key);
	virtual bool Handle_MouseUp(const int& _x, const int& _y, const bool& _left_button);

	const std::string& Text() { return text; };

protected:
	bool masked;
	std::string text;
	std::string label;

	D3DCOLOR color;
	ID3DXFont* font;

private:
	TextBox(const TextBox& _other);
	TextBox& operator=(const TextBox& _other);
};

#endif