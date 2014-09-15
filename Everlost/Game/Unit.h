#ifndef GAME_UNIT_H
#define GAME_UNIT_H

#include <string>
#include <vector>

#include <d3d9.h> 
#include <d3dx9.h> 

class Unit
{
public:
	D3DXVECTOR3 location;
	D3DXVECTOR3 move_vector;
	float speed;

	int id;
	int icon_id;
	std::string name;

	virtual ~Unit();

	void Update(const float& _elapsed_time);
	void SetPosition(const std::vector<std::string>& _args);

protected:
	Unit();

private:
	Unit(const Unit& _other);
	Unit& operator=(const Unit& _other);
};

#endif