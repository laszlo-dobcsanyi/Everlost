#include "Core\Macro.h"
#include "Graphics\TextBox.h"

extern LPDIRECT3DDEVICE9 device;

TextBox::TextBox(const int& _x, const int& _y, const int& _width, const int& _height, const std::string& _label, bool _masked, int _font_height, LPCSTR _font_name, D3DCOLOR _font_color, const std::string _text)
	: Layer(_x, _y, _width, _height),
	  label(_label),
	  masked(_masked),
	  color(_font_color),
	  text(_text),

	  font(0)
{
	D3DXCreateFont(device, _font_height, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, _font_name, &font);
}

void TextBox::Render()
{
	LPD3DXSPRITE sprite;
	D3DXCreateSprite(device, &sprite);
	sprite->Begin(0);

	if (!masked) font->DrawText(sprite, (label + text).c_str(), -1, &surface, DT_LEFT|DT_NOCLIP, color);
	else
	{
		std::string mask = "";
		for(unsigned int i = 0; i < text.length(); ++i)
			mask.push_back('*');

		font->DrawText(sprite, (label + mask).c_str(), -1, &surface, DT_LEFT|DT_NOCLIP,  color);
	}

	sprite->End();
	sprite->Release();
}

void TextBox::Handle_KeyUp(const UINT_PTR& _key)
{
	switch(_key)
	{
		case 'A' : text += 'a'; break;
		case 'B' : text += 'b'; break;
		case 'C' : text += 'c'; break;
		case 'D' : text += 'd'; break;
		case 'E' : text += 'e'; break;
		case 'F' : text += 'f'; break;
		case 'G' : text += 'g'; break;
		case 'H' : text += 'h'; break;
		case 'I' : text += 'i'; break;
		case 'J' : text += 'j'; break;
		case 'K' : text += 'k'; break;
		case 'L' : text += 'l'; break;
		case 'M' : text += 'm'; break;
		case 'N' : text += 'n'; break;
		case 'O' : text += 'o'; break;
		case 'P' : text += 'p'; break;
		case 'Q' : text += 'q'; break;
		case 'R' : text += 'r'; break;
		case 'S' : text += 's'; break;
		case 'T' : text += 't'; break;
		case 'U' : text += 'u'; break;
		case 'V' : text += 'v'; break;
		case 'W' : text += 'w'; break;
		case 'X' : text += 'x'; break;
		case 'Y' : text += 'y'; break;
		case 'Z' : text += 'z'; break;

		case '0' : text += '0'; break;
		case '1' : text += '1'; break;
		case '2' : text += '2'; break;
		case '3' : text += '3'; break;
		case '4' : text += '4'; break;
		case '5' : text += '5'; break;
		case '6' : text += '6'; break;
		case '7' : text += '7'; break;
		case '8' : text += '8'; break;
		case '9' : text += '9'; break;

		case VK_BACK: if (text.length() != 0) text.pop_back(); break;
	}
}

bool TextBox::Handle_MouseUp(const int& _x, const int& _y, const bool& _left_button)
{
	return true;
}

TextBox::~TextBox()
{
	font->Release(); font = 0;
}


