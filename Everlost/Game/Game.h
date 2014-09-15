#ifndef GAME_GAME_H
#define GAME_GAME_H

#define GROUP_CHARACTERS_NUMBER 5

#include "Core\ListProcessable.hpp"

class Object;
class Character;

struct GroupCharacter
{
	bool leader;
	Character* character;

	GroupCharacter(Character* _character);
	~GroupCharacter();
};

class Game
{
public:
	Character* hero;

	bool invited;
	std::string inviter;
	int inviter_icon_id;

	int battle_direction;
	int battle_offset_x, battle_offset_y;

	ListLockable< GroupCharacter* > group;
	
	ListProcessable< Character* > characters;
	ListProcessable< Object* > objects;

	Game();
	~Game();

	void Relocate(const int& _direction, const int& _offset_x, const int& _offset_y);

	void Update(const float& _elapsed_time);

	Character* GetCharacter(const int& _id);

	void Dispose();

private:
	bool disposed;

	Game(const Game& _other);
	Game& operator=(const Game& _other);
};

#endif