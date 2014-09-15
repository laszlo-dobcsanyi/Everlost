#include "Core\Macro.h"
#include "Graphics\Window_Game.h"

#include <boost\lexical_cast.hpp>

#include "Core\Connection.h"

#include "Game\Game.h"
#include "Game\Character.h"
#include "Graphics\Layer.h"
#include "Graphics\Resources.h"

extern Connection* connection;

extern HWND form;
extern Game* game;
extern Resources* resources;

void Window_Game::Handle_KeyUp(const UINT_PTR& _key)
{
	if (_key == resources->interface_keys[GROUP_INVITE])
	{
		POINT mouse; GetCursorPos(&mouse); ScreenToClient(form, &mouse); 
		boost::unique_lock<boost::shared_mutex> lock(game->characters.data.mutex);

		BOOST_FOREACH(Character* character, game->characters.data.list)
		{
			if ((512 + character->location.x - game->hero->location.x - 32 <= mouse.x) && (mouse.x <= 512 + character->location.x - game->hero->location.x + 32))
				if ((384 + character->location.y - game->hero->location.y - 32 <= mouse.y) && (mouse.y <= 384 + character->location.y - game->hero->location.y + 32))
				{
					#ifdef LOGGING
					Logger::Write(6, ">Inviting " + character->name + "(" + boost::lexical_cast<std::string>(character->id) + ")");
					#endif

					connection->Send(Connection::ClientCommand::GROUP_INVITE, character->name);
					return;
				}

		}


		#ifdef LOGGING
		Logger::Write(6, ">BLIND Inviting..");
		#endif

		connection->Send(Connection::ClientCommand::GROUP_INVITE, "lackoa");
	}

	if (_key == resources->interface_keys[BATTLE_JOIN])
	{
		connection->Send(Connection::ClientCommand::BATTLE_JOIN, "");
		return;
	}


	if (_key == VK_F4)
	{
		#ifdef LOGGING
		Logger::Write(6, ">BLIND Inviting..");
		#endif

		connection->Send(Connection::ClientCommand::GROUP_INVITE, "lackos");
	}
}

Character* Window_Game::GetCharacter(const int& _x, const int& _y)
{
	return 0;
}

void Window_Game::Handle_MouseUp(const int& _x, const int& _y, const bool& _left_button)
{
	BOOST_FOREACH(LayerNode* current, objects)
	{
		if (current->layer->Visible())
			if (current->layer->Collide(_x, _y))
				if (current->layer->Handle_MouseUp(_x, _y, _left_button)) return;
	}

	//Character stop
	if ((512 - 32 <= _x) && (_x <= 512 + 32))
		if ((384 - 32 <= _y) && (_y <= 384 + 32))
			{ connection->Send(Connection::HERO_STOP, ""); return; }

	//Character move
	float direction = std::atan2(_y - 384, _x - 512);
	connection->Send(Connection::HERO_MOVE, boost::lexical_cast<std::string>(direction));
}