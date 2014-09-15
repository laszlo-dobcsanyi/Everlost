#include "Core\Macro.h"
#include "Graphics\Tile.h"

#include <fstream>

#include <boost\lexical_cast.hpp>

extern LPDIRECT3DDEVICE9 device;

ObjectTexture::ObjectTexture(const int& _id, const std::string& _texture)
	 : id(_id)
{
	#ifdef LOGGING
	Logger::Write(6, "\t\t> Loading StoredObject " + boost::lexical_cast<std::string>(_id) + "(" + _texture + ")..");
	#endif

	D3DXCreateTextureFromFile(device, ("data\\objects\\" + _texture + ".png").c_str(), &texture);
}

ObjectTexture::~ObjectTexture()
{	
	#ifdef LOGGING
	Logger::Write(6, "\t\t> StoredObject destructor " + boost::lexical_cast<std::string>(id) + "..");
	#endif

	texture->Release();
}

///

Tile::Tile(const int& _id)
	: id(_id)
{
	#ifdef LOGGING
	Logger::Write(5, "\t> Loading Tile " + boost::lexical_cast<std::string>(_id) + "..");
	#endif

	//Open Tile file and load object textures
	std::ifstream tile_file("data\\tiles\\" + boost::lexical_cast<std::string>(id) + ".tile");
	if (tile_file)
	{
		//Load Tile Texture
		std::string texture;	tile_file >> texture;
		D3DXCreateTextureFromFile(device, ("data\\tiles\\" + texture + ".png").c_str(), &tile_texture);

		//Create ObjectTextures in each collision class
		int number;
		for(int current = 0; current < COLLISION_CLASSES; ++current)
		{
			tile_file >> number;
			for(int index = 0; index < number; ++index)
			{
				int object_id; tile_file >> object_id;
				std::string object_texture; tile_file >> object_texture;

				object_textures[current].push_back(new ObjectTexture(object_id, object_texture));
			}
		}
	}
	tile_file.close();
}

const LPDIRECT3DTEXTURE9& Tile::GetObjectTexture(const int& _id) const
{
	for(int current = 0; current < COLLISION_CLASSES; ++current)
		for (unsigned int index = 0; index < object_textures[current].size(); ++index)
			if (object_textures[current][index]->id == _id)
				return object_textures[current][index]->texture;
}

Tile::~Tile()
{
	#ifdef LOGGING
	Logger::Write(6, "\t>Tile destructor " + boost::lexical_cast<std::string>(id) + "..");
	#endif

	tile_texture->Release();

	for(int current = 0; current < COLLISION_CLASSES; ++current)
		for(unsigned int index = 0; index < object_textures[current].size(); ++index)
			{
				object_textures[current][index]->texture->Release();
				delete object_textures[current][index];
			}
}

