#ifndef GRAPHICS_RESOURCES_H
#define GRAPHICS_RESOURCES_H

#define FACTIONS_NUMBER 3
#define CHARACTER_ICONS_NUMBER 24

#include <vector>

#include <d3d9.h> 
#include <d3dx9.h> 

class Tile;

enum InterfaceKeys
{
	GROUP_INVITE	= 0,
	GROUP_KICK		= 1,
	BATTLE_JOIN		= 2
};

class Resources
{
public:
	/// General

	LPDIRECT3DTEXTURE9 character_icons[FACTIONS_NUMBER * CHARACTER_ICONS_NUMBER];

	/// Interface

	UINT interface_keys[3];

	LPDIRECT3DTEXTURE9 battle_fog;

	Resources();
	~Resources();
	
	void LoadWorld(const int& _id);

	inline bool Ready() const { return world_data; };
	const Tile* const GetTile(const int& _id);
	inline const int& GetTileID(const int& _x, const int&_y) const
	{
		return world_data[_y * world_width + _x];
	};
	void UpdateTileset(D3DXVECTOR3 _location);

	void Dispose();

private:
	bool disposed;

	std::vector<Tile*> tiles;

	std::string world_name;
	int world_width, world_height;
	int* world_data;

	Resources(const Resources& _other);
	Resources& operator=(const Resources& _other);
};

#endif