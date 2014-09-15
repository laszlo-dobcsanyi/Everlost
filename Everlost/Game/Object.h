#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <vector>
#include <string>

#include <d3d9.h> 
#include <d3dx9.h>

class Object
{
public:
	int x, y, id;
	LPDIRECT3DTEXTURE9 texture;

	Object(const std::vector< std::string >& _args);
	~Object();

private:
	Object(const Object& _other);
	Object& operator=(const Object& _other);
};

#endif