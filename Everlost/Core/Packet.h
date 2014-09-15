#ifndef CORE_PACKET_H
#define CORE_PACKET_H

#include <string>

struct Packet
{
public:
	int size;
	char* data;

	Packet(const int& _command, const std::string& _message);
	~Packet();

private:
	Packet(const Packet& _other);
	Packet& operator=(const Packet* other);
};

#endif
