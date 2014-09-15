#include "Core\Macro.h"
#include "Graphics\Button.h"

extern LPDIRECT3DDEVICE9 device;

Button::Button(const int& _x, const int& _y, const int& _width, const int& _height, const std::string& _texture, const boost::function<void (void)>& _callback)
	: Layer(_x, _y, _width, _height),
	  callback(_callback)
{
	D3DXCreateTextureFromFileEx(device, _texture.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &texture);
}

void Button::Render()
{
	if (visible)
	{
		LPD3DXSPRITE sprite;
		D3DXCreateSprite(device, &sprite);
		sprite->Begin(0);

		sprite->Draw(texture, NULL, NULL, &D3DXVECTOR3(surface.left, surface.top, 0), 0xFFFFFFFF);

		sprite->End();
		sprite->Release();
	}
}

void Button::Handle_KeyUp(const UINT_PTR& _key)
{
	if (_key == VK_RETURN) callback();
}

bool Button::Handle_MouseUp(const int& _x, const int& _y, const bool& _left_button)
{
	callback();
	return true;
}

Button::~Button()
{
	texture->Release(); texture = 0;
}

