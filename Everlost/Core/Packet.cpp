#include "Core\Macro.h"
#include "Core\Packet.h"

Packet::Packet(const int& _command, const std::string& _message)
	: data(new char[4 + _message.size() + 1]),
	  size(4 + _message.size() + 1)
{
	data[0] = (_command >> 24) & 0xFF;
	data[1] = (_command >> 16) & 0xFF;
	data[2] = (_command >> 8) & 0xFF;
	data[3] = _command & 0xFF;
	memcpy(data + 4, _message.c_str(), _message.size() + 1);
}

Packet::~Packet()
{
	delete[] data;
}
