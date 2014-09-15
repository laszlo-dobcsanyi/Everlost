#include "Core\Macro.h"
#include "Game\Character.h"

#include <boost\lexical_cast.hpp>

Character::Character(const std::vector< std::string >& _args)
	: Unit()
{
	id = boost::lexical_cast<int>(_args[0]);
	name = _args[1];
	icon_id = boost::lexical_cast<int>(_args[2]);

	location.x = boost::lexical_cast<float>(_args[3]);
	location.y = boost::lexical_cast<float>(_args[4]);

	move_vector.x = cos(boost::lexical_cast<float>(_args[5]));
	move_vector.y = sin(boost::lexical_cast<float>(_args[5]));

	speed = boost::lexical_cast<float>(_args[6].data());


	#ifdef LOGGING
	Logger::Write(4, "> Created Character @ " + boost::lexical_cast< std::string >(location.x) + "x" + boost::lexical_cast< std::string >(location.y) + "..");
	#endif
}

Character::~Character()
{
	#ifdef LOGGING
	Logger::Write(4, "> Character Destructor..");
	#endif
}

