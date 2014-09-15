#include "Core\Macro.h"
#include "Game\Unit.h"

#include <math.h>

#include <boost\lexical_cast.hpp>

Unit::Unit()
	: location(0, 0, 0),
	  move_vector(0, 0, 0),
	  speed(0)
{

}

void Unit::Update(const float& _elapsedtime)
{
	//if (speed != 0)
	{
		location.x += move_vector.x * speed * _elapsedtime;
		location.y += move_vector.y * speed * _elapsedtime;
	}

	//std::cout << "\t> " << location.x << ":" << location.y << " . " << _elapsedtime << " (" << move_vector.x << " x " << move_vector.y << " @ " << speed <<  std::endl;
}

void Unit::SetPosition(const std::vector<std::string>& _args)
{
	location.x = boost::lexical_cast<float>(_args[1]);
	location.y = boost::lexical_cast<float>(_args[2]);

	move_vector.x = std::cos(boost::lexical_cast<float>(_args[3]));
	move_vector.y = std::sin(boost::lexical_cast<float>(_args[3]));

	speed = boost::lexical_cast<float>(_args[4].data());
}

Unit::~Unit()
{

}

