#include "Core\Macro.h"
#include "Core\Connection.h"

#include <vector>
#include <string>

#include <boost\lexical_cast.hpp>
#include <boost\algorithm\string.hpp>

#include "Game\Game.h"
#include "Game\Object.h"
#include "Game\Character.h"
#include "Graphics\Resources.h"
#include "Graphics\Window_Game.h"

extern Game* game;
extern Window* window;
extern WindowStates window_state;
extern Resources* resources;

std::string ToString(char* _data, unsigned int _size)
{
	std::string value = "";
	for(unsigned int current = 0; current < _size; ++current) value += _data[current];
	return value;
}

void Connection::HandleMessage(size_t _received)
{
	int command = 0x00000000; command |= data[0] << 24; command |= data[1] << 16; command |= data[2] << 8; command |= data[3];
	std::vector< std::string > arguments; boost::split(arguments, ToString(data + 4, _received - 4), boost::is_any_of(";"));

	switch (command)
	{
		case ServerCommand::LOGIN_OK:
			{
				window_state = WindowStates::CHANGE_GAME;

				#ifdef LOGGING
				Logger::Write(5, "\t< Authenticated successfully!");
				#endif

				server_endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(GATEWAY_ADDRESS), boost::lexical_cast<int>(data + 4));
				ping_timer->async_wait(boost::bind(&Connection::PingCallback, this, ping_timer, boost::asio::placeholders::error));

				Send(ClientCommand::LOGIN_RESPONSE, "");
			} break;

		case ServerCommand::REGISTRATE_OK:
			#ifdef LOGGING
			Logger::Write(5, "\t< Registrated succesfully!");
			#endif
			break;

		///

		case ServerCommand::HERO_DATA: 
			#ifdef LOGGING
			Logger::Write(9, "> Hero data..");
			#endif

			game->hero = new Character(arguments);
			break;

		case ServerCommand::HERO_WORLD: 
			#ifdef LOGGING
			Logger::Write(9, "> World data..");
			#endif

			resources->LoadWorld(boost::lexical_cast<int>(arguments[0].data()));
			break;

		///

		case ServerCommand::CHARACTER_ENTER: {
			#ifdef LOGGING
			Logger::Write(9, "> Character entering..");
			#endif
			
			Character* character = game->GetCharacter(boost::lexical_cast<int>(arguments[0].data()));
			if (character)
			{
				#ifdef LOGGING
				Logger::Write(3, "# Character already added with ID(" + arguments[0] + ")..");
				#endif
			}
			else
			{
				game->characters.Add(new Character(arguments));
			}

			}break;

		case ServerCommand::CHARACTER_MOVE:{
			#ifdef LOGGING
			Logger::Write(9, "> Moving character..");
			#endif

			Character* character = game->GetCharacter(boost::lexical_cast<int>(arguments[0].data()));
			if (character)
			{
				character->SetPosition(arguments);
			}
			else
			{
				#ifdef LOGGING
				Logger::Write(3, "# Character not found with ID(" + arguments[0] + ")!");
				#endif
			}
			}break;

		case ServerCommand::CHARACTER_LEAVE:{
			#ifdef LOGGING
			Logger::Write(9, "> Character leaving..");
			#endif

			Character* character = game->GetCharacter(boost::lexical_cast<int>(arguments[0].data()));
			if (character)
			{
				game->characters.Remove(character);
			}
			else
			{
				#ifdef LOGGING
				Logger::Write(3, "# Character already left with ID(" + arguments[0] + ")..");
				#endif
			}
			}break;

		///

		case ServerCommand::OBJECT_ADD:
			#ifdef LOGGING
			Logger::Write(8, "> Adding object..");
			#endif

			game->objects.Add(new Object(arguments));
			break;

		case ServerCommand::OBJECT_REMOVE:
			#ifdef LOGGING
			Logger::Write(8, "> Removing object..");
			#endif
			break;

		///

		case ServerCommand::GROUP_INVITATION:
			game->inviter = arguments[0];
			game->inviter_icon_id = 0;
			game->invited = true;
			break;

		case ServerCommand::GROUP_ADD:
			{
				#ifdef LOGGING
				Logger::Write(8, "> GroupAdd...");
				#endif

				GroupCharacter* group_character = new GroupCharacter(new Character(arguments));
				game->group.Add(group_character);
			}
			break;

		case ServerCommand::GROUP_LEADER:
			{
				#ifdef LOGGING
				Logger::Write(8, "> GroupLeader...");
				#endif

				boost::unique_lock<boost::shared_mutex> lock(game->group.mutex);

				int ID = boost::lexical_cast< int >(arguments[0].data());

				BOOST_FOREACH(GroupCharacter* group_character, game->group.list)
				{
					if (group_character->character->id == ID)
					{
						group_character->leader = true;
					}
					else
					{
						group_character->leader = false;
					}
				}
			}
			break;

		case ServerCommand::GROUP_LEAVE:
			{				
				#ifdef LOGGING
				Logger::Write(8, "> GroupLeave...");
				#endif

				int ID = boost::lexical_cast< int >(arguments[0].data());

				if (ID == game->hero->id)
				{
					game->group.Clear();
				}
				else
				{
					//boost::unique_lock<boost::shared_mutex> lock(game->group.mutex);
	
					GroupCharacter* leaver = 0;

					BOOST_FOREACH(GroupCharacter* group_character, game->group.list)
					{
						if (group_character->character->id == ID)
						{
							leaver = group_character;
							break;
						}

					}

					if (leaver)
					{
						game->group.Remove(leaver);
					}
					else
					{
						#ifdef LOGGING
						Logger::Write(3, "# Error while removing character from group: Character not found!");
						#endif
					}
				}
			}
			break;

		///

		case ServerCommand::BATTLE_RELOCATE:
			{
			#ifdef LOGGING
			Logger::Write(5, "> Relocating for battle!");
			#endif

			int direction, offset_x, offset_y; direction = boost::lexical_cast< int >(arguments[0]); offset_x = boost::lexical_cast< int > (arguments[1]); offset_y = boost::lexical_cast< int >(arguments[2].data());
			game->Relocate(direction, offset_x, offset_y);
			}
			break;

		///

		case ServerCommand::ERROR_LOGIN:
			#ifdef LOGGING
			Logger::Write(5, "# Error while logging in! (Maybe wrong username/password?)");
			#endif
			break;

		case ServerCommand::ERROR_REGISTRATE:
			#ifdef LOGGING
			Logger::Write(5, "# Error while registrating! (Username already in use?)");
			#endif
			break;

		///

		default:
			#ifdef LOGGING
			Logger::Write(5, "# Error @ Connection::HandleMessage: Unknown command!");
			#endif
			break;
	}
}

void Connection::PingCallback(boost::asio::deadline_timer* _timer,const boost::system::error_code& _error)
{
	if (!_error) Send(ClientCommand::PING, "");

	#ifdef LOGGING
	Logger::Write(6, ">Ping..");
	#endif

	_timer->expires_from_now(boost::posix_time::seconds(PING_TIME));
	_timer->async_wait(boost::bind(&Connection::PingCallback, this, _timer, boost::asio::placeholders::error));
}



