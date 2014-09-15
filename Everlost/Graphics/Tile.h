#ifndef GRAPHICS_TILE_H
#define GRAPHICS_TILE_H

#define COLLISION_CLASSES 8

#include <vector>

#include <d3d9.h> 
#include <d3dx9.h> 

struct ObjectTexture
{
	int id;
	LPDIRECT3DTEXTURE9 texture;

	ObjectTexture(const int& _id, const std::string& _texture);
	~ObjectTexture();
};

class Tile
{
public:
	const int id;

	Tile(const int& _id);
	~Tile();

	inline const LPDIRECT3DTEXTURE9 GetTexture() const { return tile_texture; };
	const LPDIRECT3DTEXTURE9& GetObjectTexture(const int& _id) const;

private:
	LPDIRECT3DTEXTURE9 tile_texture;
	std::vector<ObjectTexture*> object_textures[COLLISION_CLASSES];

	Tile(const Tile& _other);
	Tile& operator=(const Tile& _other);
};

#endif