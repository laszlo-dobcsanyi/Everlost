#include "Core\Macro.h"
#include "Graphics\Resources.h"

#include <fstream>
#include <sstream>

#include <boost\lexical_cast.hpp>

#include "Graphics\Tile.h"

extern LPDIRECT3DDEVICE9 device;

Resources::Resources()
	: disposed(false)
{
	#ifdef LOGGING
	Logger::Write(4, "\t> Loading resources..");
	#endif

	interface_keys[GROUP_INVITE]	= VK_F1;
	interface_keys[GROUP_KICK]		= VK_F2;
	interface_keys[BATTLE_JOIN]		= VK_F3;

	for (int faction = 0; faction < FACTIONS_NUMBER; faction++)
		for (int current = 0; current < CHARACTER_ICONS_NUMBER; current++)
		{
			std::stringstream name; name << "data\\icons\\hero"  << faction << "_" << current << ".png";
			D3DXCreateTextureFromFileEx(device, name.str().c_str(), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
                                        0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
                                        0, NULL, NULL, &character_icons[faction * CHARACTER_ICONS_NUMBER + current]);
		}

	D3DXCreateTextureFromFileEx(device, "data\\interface\\battle_fog.png", D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &battle_fog);
}

void Resources::LoadWorld(const int& _id)
{
	#ifdef LOGGING
	Logger::Write(4, "> Loading World " + boost::lexical_cast<std::string>(_id) + "..");
	#endif

	std::ifstream world_file("data\\worlds\\" + boost::lexical_cast<std::string>(_id) + ".world");
	if (world_file)
	{
		world_file >> world_name;
		world_file >> world_width >> world_height;

		int tmp;
		int* tile_data = new int[world_width * world_height];
		for(int current = 0; current < world_width * world_height; ++current)
			world_file >> tmp >> tmp >> tile_data[current] >> tmp;
		world_data = tile_data;
	}
	else
	{
		#ifdef LOGGING
		Logger::Write(1, "> Error while opening world file!");
		#endif
	}

	world_file.close();
}

const Tile* const Resources::GetTile(const int& _id)
{
	for(unsigned int current = 0; current < tiles.size(); ++current)
		if (tiles[current]->id == _id) return tiles[current];

	//Really?!
	Tile* tile = new Tile(_id);
	tiles.push_back(tile);
	return tile;
}


void Resources::UpdateTileset(D3DXVECTOR3 _location)
{
	//TODO
}

void Resources::Dispose()
{
	if (disposed) return; disposed = true;

	#ifdef LOGGING
	Logger::Write(5, "> Disposing Resources..");
	#endif

	for(unsigned int current = 0; current < tiles.size(); ++current)
		delete tiles[current];

	delete[] world_data;
}

Resources::~Resources()
{
	#ifdef LOGGING
	Logger::Write(4, "> Resources destructor..");
	#endif

	for (int faction = 0; faction < FACTIONS_NUMBER; faction++)
		for (int current = 0; current < CHARACTER_ICONS_NUMBER; current++)
			character_icons[faction * CHARACTER_ICONS_NUMBER + faction]->Release();

	battle_fog->Release();
}


