#include "Core\Macro.h"
#include "Core\Connection.h"

#include <boost\lexical_cast.hpp>

#include "Core\Packet.h"

Connection::Connection()
	: data(new char[MAX_PACKET_SIZE]),
	  ping_timer(new boost::asio::deadline_timer(io_service, boost::posix_time::seconds(PING_TIME))),

	  endpoint(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 0)),
	  //endpoint(boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4(), 0)),
	  server_endpoint(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(GATEWAY_ADDRESS), 1425)),

	  io_service(),
	  socket(io_service, endpoint),
	  
	  thread(boost::bind(&Connection::Run, this))
{
	#ifdef LOGGING
	Logger::Write(0, "\t< Connection created @ [" + boost::lexical_cast<std::string>(endpoint.address()) + ":" + boost::lexical_cast<std::string>(endpoint.port()) +"]!");
	#endif

	DWORD size = 64 * 1024;
	setsockopt(socket.native(), SOL_SOCKET, SO_SNDBUF, (char *)&size, sizeof(WORD));
	setsockopt(socket.native(), SOL_SOCKET, SO_RCVBUF, (char *)&size, sizeof(WORD));
}

void Connection::Run()
{
	Receive();

	io_service.run();
}

void Connection::Receive()
{
	socket.async_receive_from(boost::asio::buffer(data, MAX_PACKET_SIZE), connected_endpoint,
		boost::bind(&Connection::HandleReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::HandleReceive(const boost::system::error_code& _error, const size_t& _received)
{
	if ((!_error) && (0 < _received))
	{
		if (connected_endpoint == server_endpoint) HandleMessage(_received);
		else
		{
			#ifdef LOGGING
			Logger::Write(2, "# Corrupted Message received from [" + boost::lexical_cast<std::string>(connected_endpoint.address()) + ":" + boost::lexical_cast<std::string>(connected_endpoint.port()) + "]!");
			#endif
		}
	}
	else
	{
		#ifdef LOGGING
		Logger::Write(3, "# Error @ Connection::HandleReceive " + _error.message() + " (" + boost::lexical_cast<std::string>(_received) + ") from [" + boost::lexical_cast<std::string>(connected_endpoint.address()) + ":" + boost::lexical_cast<std::string>(connected_endpoint.port()) + "]!");
		#endif
	}

	Receive();
}

void Connection::Send(const int& _cmd, const std::string& _data)
{
	Packet packet(_cmd, _data);

	#ifdef LOGGING
	//Logger::Write(9, "\t> Connection sending packet..");
	#endif

	ping_timer->expires_from_now(boost::posix_time::seconds(PING_TIME));

	socket.async_send_to(boost::asio::buffer(packet.data, packet.size), server_endpoint,
		boost::bind(&Connection::HandleSend, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::HandleSend(const boost::system::error_code& _error, const size_t& _sent)
{
	if (_error)
	{
		#ifdef LOGGING
		Logger::Write(3, "# Error @ Connection:HandleSend " + _error.message());
		#endif
	}
}

Connection::~Connection()
{
	#ifdef LOGGING
	Logger::Write(4, "<Connection destructor..");
	#endif
}
