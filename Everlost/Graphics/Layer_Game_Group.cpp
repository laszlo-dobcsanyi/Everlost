#include "Core\Macro.h"
#include "Graphics\Layer_Game_Group.h"

#include <boost\bind.hpp>

#include "Game\Game.h"
#include "Game\Character.h"

#include "Graphics\Button.h"
#include "Graphics\Resources.h"

extern Game* game;
extern Resources* resources;
extern LPDIRECT3DDEVICE9 device;

GroupLayer::GroupLayer(const int& _x, const int& _y, const int& _width, const int& _height)
	: Layer(_x, _y, _width, _height)
{
	D3DXCreateFont(device, 40, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, "Arial Black", &font);

	D3DXCreateTextureFromFileEx(device, "data\\interface\\group_frame.png", D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
                            0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
							0, NULL, NULL, &group_frame);
}

bool GroupLayer::Visible()
{
	return (0 < game->group.number) && visible;
}

bool GroupLayer::Collide(const int& _x, const int& _y)
{
	if ((surface.left <= _x && _x <= surface.right) && (surface.top <= _y && _y <= surface.top + 32 + game->group.number * 78)) return true;
	return false;
};

void GroupLayer::Render()
{
	LPD3DXSPRITE sprite;
	D3DXCreateSprite(device, &sprite);
	sprite->Begin(0);

	RECT source; 

	source.left = 0; source.right = 256; source.top = 0; source.bottom = 16;
	sprite->Draw(group_frame, &source, NULL, &D3DXVECTOR3(surface.left, surface.top, 0), 0x88FFFFFF);

	source.left = 0; source.right = 256; source.top = 64; source.bottom = 64 + 78;

	int current = 0;
	BOOST_FOREACH(GroupCharacter* group_character, game->group.list)
	{
		sprite->Draw(group_frame, &source, NULL, &D3DXVECTOR3(surface.left, surface.top + 15 + current * 77, 0), 0x88FFFFFF);
		sprite->Draw(resources->character_icons[group_character->character->icon_id], NULL, NULL, &D3DXVECTOR3(surface.left + 15, surface.top + 16 + 6 + current * 77, 0), 0xFFFFFFFF);
		
		RECT font_rectangle;
		font_rectangle.left = surface.left + 96; font_rectangle.right = surface.right; font_rectangle.top = surface.top + 15 + 24 + current * 77; font_rectangle.bottom = surface.bottom;
		font->DrawText(sprite, group_character->character->name.c_str(), -1, &font_rectangle, DT_LEFT|DT_NOCLIP, 0xFF000000);

		++current;
	}

	source.left = 0; source.right = 256; source.top = 256 - 16; source.bottom = 256;
	sprite->Draw(group_frame, &source, NULL, &D3DXVECTOR3(surface.left, surface.top + 16 + game->group.number * 77 , 0), 0x88FFFFFF);
	
	sprite->End();
	sprite->Release();
}

void GroupLayer::Handle_KeyUp(const UINT_PTR& _key)
{

}

bool GroupLayer::Handle_MouseUp(const int& _x, const int& _y, const bool& _left_button)
{
	return false;
}

GroupLayer::~GroupLayer()
{
	group_frame->Release();
}

