#ifndef CORE_CONNECTION_H
#define CORE_CONNECTION_H

#define PING_TIME 1
#define MAX_PACKET_SIZE 512
#define GATEWAY_ADDRESS "127.0.0.2"

#include <boost\asio.hpp>
#include <boost\thread.hpp>

class Connection
{
public:
	enum ServerCommand
	{
		LOGIN_OK = 0,
		REGISTRATE_OK = 1,

		HERO_DATA = 10,
		HERO_WORLD = 11,

		CHARACTER_ENTER = 20,
		CHARACTER_MOVE = 21,
		CHARACTER_LEAVE = 29,

		OBJECT_ADD = 30,
		OBJECT_REMOVE = 39,

		GROUP_INVITATION = 40,
		GROUP_ADD = 41,
		GROUP_LEADER = 42,
		GROUP_LEAVE = 43,
		
		BATTLE_RELOCATE = 50,

		ERROR_LOGIN = 1000,
		ERROR_REGISTRATE = 1001
	};

	enum ClientCommand
	{
		LOGIN = 0,
		LOGIN_RESPONSE = 1,

		REGISTRATE = 2,
		PING = 3,

		HERO_MOVE = 10,
		HERO_STOP = 11,

		GROUP_INVITE = 40,
		GROUP_ACCEPT = 41,
		GROUP_DECLINE = 42,

		BATTLE_JOIN = 50
	};

public:
	Connection();
	~Connection();

	void Send(const int& _cmd, const std::string& _data);


private:
	char* data;
	boost::asio::ip::udp::endpoint endpoint;
	boost::asio::ip::udp::endpoint server_endpoint;
	boost::asio::ip::udp::endpoint connected_endpoint;

	boost::asio::io_service io_service;
    boost::asio::ip::udp::socket socket;

	boost::thread thread;

	boost::asio::deadline_timer* ping_timer;

	void Run();
	void Receive();
	void HandleReceive(const boost::system::error_code& _error, const size_t& _received);
	void HandleMessage(size_t _received);

	void HandleSend(const boost::system::error_code& _error, const size_t& _sent);

	void PingCallback(boost::asio::deadline_timer* _timer,const boost::system::error_code& _error);

	Connection(const Connection& _other);
	Connection& operator=(const Connection* _other);
};

#endif