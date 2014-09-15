#include "Core\Macro.h"
#include "Graphics\Window_Game.h"

#include <boost\foreach.hpp>

#include "Game\Game.h"
#include "Game\Object.h"
#include "Game\Character.h"
#include "Graphics\Tile.h"
#include "Graphics\Layer.h"
#include "Graphics\Resources.h"

extern Game* game;
extern Resources* resources;

extern LPDIRECT3DDEVICE9 device;

void Window_Game::DrawFields()
{
	LPD3DXSPRITE sprite;
	D3DXCreateSprite(device, &sprite);
	sprite->Begin(0);

	boost::unique_lock<boost::shared_mutex> characters_lock(game->characters.data.mutex);
	int camera_x = (int)game->hero->location.x / 512;
	int camera_y = (int)game->hero->location.y / 512;

	double offset_x = game->hero->location.x - camera_x * 512;
	double offset_y = game->hero->location.y - camera_y * 512;

	for(int row = -1; row < 2; ++row)
		for(int column = -1; column < 2; ++column)
		{
			int tileid = resources->GetTileID(camera_x + column + game->battle_offset_x, camera_y + row + game->battle_offset_y);
			const Tile* const tile = resources->GetTile(tileid);
			for(int current = 0; current < 4 * 4; ++current)
				sprite->Draw(tile->GetTexture(), NULL, NULL, &D3DXVECTOR3(column * 512 - offset_x + 1024 / 2 + (current % 4) * 128, row * 512 - offset_y + 768 / 2 + (current / 4) * 128, 0), 0xFFFFFFFF);
		
			switch(game->battle_direction)
			{
				case 1:
					if ((camera_x + column < 0 ||  2 * 9 - 1 < camera_x + column) || (camera_y + row < 0 ||  1 * 9 < camera_y + row))
						for(int current = 0; current < 4 * 4; ++current)
							sprite->Draw(resources->battle_fog, NULL, NULL, &D3DXVECTOR3(column * 512 - offset_x + 1024 / 2 + (current % 4) * 128, row * 512 - offset_y + 768 / 2 + (current / 4) * 128, 0), 0xFFFFFFFF);
					break;

				case 2:
					if ((camera_x + column < 0 ||  1 * 9 - 1 < camera_x + column) || (camera_y + row < 0 ||  2 * 9 < camera_y + row))
						for(int current = 0; current < 4 * 4; ++current)
							sprite->Draw(resources->battle_fog, NULL, NULL, &D3DXVECTOR3(column * 512 - offset_x + 1024 / 2 + (current % 4) * 128, row * 512 - offset_y + 768 / 2 + (current / 4) * 128, 0), 0xFFFFFFFF);
					break;
			}
		}

	sprite->End();
	sprite->Release();
}

void Window_Game::DrawObjects()
{
	LPD3DXSPRITE sprite;
	D3DXCreateSprite(device, &sprite);
	sprite->Begin(0);

	BOOST_FOREACH(Object* object, game->objects.data.list)
	{
		sprite->Draw(object->texture, NULL, NULL,
			&D3DXVECTOR3(object->x - game->hero->location.x + 512 - 32, object->y - game->hero->location.y + 384 - 32, 0), 0xFFFFFFFF);
	}

	sprite->End();
	sprite->Release();
}

void Window_Game::DrawUnits()
{
	LPD3DXSPRITE sprite;
	D3DXCreateSprite(device, &sprite);
	sprite->Begin(0);

	sprite->Draw(resources->character_icons[game->hero->icon_id], NULL, NULL, &D3DXVECTOR3(512 - 32, 384 - 32, 0), 0xFFFFFFFF);

	BOOST_FOREACH(Character* character, game->characters.data.list)
	{
		sprite->Draw(resources->character_icons[character->icon_id], NULL, NULL,
			&D3DXVECTOR3(character->location.x - game->hero->location.x + 512 - 32, character->location.y - game->hero->location.y + 384 - 32, 0), 0xFFFFFFFF);
	}

	sprite->End();
	sprite->Release();
}

void Window_Game::DrawInterface()
{
	LPD3DXSPRITE sprite;
	D3DXCreateSprite(device, &sprite);
	sprite->Begin(0);

	BOOST_FOREACH(LayerNode* current, objects)
	{
		if (current->layer->Visible())
			current->layer->Render();
	}

	//if(game->invited)
	//{
	//	sprite->Draw(resources->group_invite_frame, NULL, NULL, &D3DXVECTOR3(512 - 256, 128, 0), 0xFFFFFFFF);
	//	sprite->Draw(resources->character_icons[game->inviter_icon_id], NULL, NULL, &D3DXVECTOR3(512 - 256 + 115, 128 + 88, 0), 0xFFFFFFFF);
	//}

	sprite->End();
	sprite->Release();
}

void Window_Game::DrawLoading()
{

}
