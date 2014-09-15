#ifndef GAME_CHARACTER_H
#define GAME_CHARACTER_H

#include <vector>

#include "Game\Unit.h"

class Character
	: public Unit
{
public:
	Character(const std::vector< std::string >& _args);
	~Character();

private:
	Character(const Character& _other);
	Character& operator=(const Character& _other);
};

#endif