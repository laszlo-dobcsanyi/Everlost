#include "Core\Macro.h"
#include "Game\Object.h"

#include <boost\lexical_cast.hpp>

extern LPDIRECT3DDEVICE9 device;

Object::Object(const std::vector< std::string >& _args)
{
	#ifdef LOGGING
	Logger::Write(6, "\t> Object constructor..");
	#endif

	x = boost::lexical_cast<float>(_args[0]);
	y = boost::lexical_cast<float>(_args[1]);
	id = boost::lexical_cast<int>(_args[2].data());

	D3DXCreateTextureFromFile(device, ("data\\objects\\" + boost::lexical_cast<std::string>(id) + ".png").c_str(), &texture);
}

Object::~Object()
{
	#ifdef LOGGING
	Logger::Write(6, "\t> Object destructor..");
	#endif

	texture->Release();
}


