#include "Core\Macro.h"
#include "Game\Game.h"

#include <boost\foreach.hpp>
#include <boost\lexical_cast.hpp>

#include "Game\Object.h"
#include "Game\Character.h"

///
GroupCharacter::GroupCharacter(Character* _character)
	: character(_character),
	  leader(false)
{

}

GroupCharacter::~GroupCharacter()
{
	delete character;
};

///

Game::Game()
	: disposed(false),
	  invited(false),


	  battle_direction(-1),
	  battle_offset_x(0),
	  battle_offset_y(0)
{

}

void Game::Relocate(const int& _direction, const int& _offset_x, const int& _offset_y)
{
	boost::unique_lock<boost::shared_mutex> characters_lock(characters.data.mutex);
	battle_direction = _direction;	battle_offset_x = _offset_x;	battle_offset_y = _offset_y;

	Logger::Write(5, ">Relocating for battle to " + boost::lexical_cast<std::string>(_offset_x) + "x" + boost::lexical_cast<std::string>(_offset_y) + "..");

	hero->location.x -= _offset_x * 512;
	hero->location.y -= _offset_y * 512;

	BOOST_FOREACH(Character* character, characters.data.list)
	{
		character->location.x -= _offset_x * 512;
		character->location.y -= _offset_y * 512;
	}

	boost::unique_lock<boost::shared_mutex> objects_lock(objects.data.mutex);
	BOOST_FOREACH(Object* object, objects.data.list)
	{
		object->x -= _offset_x * 512;
		object->y -= _offset_y * 512;
	}
}

void Game::Update(const float& _elapsed_time)
{
	objects.Process_Removing();
	characters.Process_Removing();

	if (hero) hero->Update(_elapsed_time);
	boost::unique_lock<boost::shared_mutex> characters_lock(characters.data.mutex);
	BOOST_FOREACH(Character* character, characters.data.list)
	{
		character->Update(_elapsed_time);
	}
	characters_lock.unlock();

	objects.Process_Adding();
	characters.Process_Adding();
}

Character* Game::GetCharacter(const int& _id)
{
	if (hero->id == _id) return hero;

	boost::shared_lock<boost::shared_mutex> characters_lock(characters.data.mutex);
	BOOST_FOREACH(Character* character, characters.data.list)
	{
		if (character->id == _id) return character;
	}

	return 0;
}

void Game::Dispose()
{
	if (disposed) return; disposed = true;

	#ifdef LOGGING
	Logger::Write(5, "\t> Disposing Game..");
	#endif

	if (hero) delete hero;

	BOOST_FOREACH(GroupCharacter* group_character, group.list)
	{
		delete group_character;
	}

	BOOST_FOREACH(Character* character, characters.data.list)
	{
		delete character;
	}
}

Game::~Game()
{
	#ifdef LOGGING
	Logger::Write(4, "> Game destructor..");
	#endif
}


