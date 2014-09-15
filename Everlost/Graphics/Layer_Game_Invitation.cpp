#include "Core\Macro.h"
#include "Graphics\Layer_Game_Invitation.h"

#include <boost\bind.hpp>

#include "Core\Connection.h"

#include "Game\Game.h"
#include "Game\Character.h"

#include "Graphics\Button.h"

extern Connection* connection;

extern Game* game;
extern LPDIRECT3DDEVICE9 device;

InvitationLayer::InvitationLayer(const int& _x, const int& _y, const int& _width, const int& _height)
	: Layer(_x, _y, _width, _height)
{
	font_rectangle.left = surface.left + 128 + 64; font_rectangle.right = surface.right; font_rectangle.top = surface.top + 32; font_rectangle.bottom = surface.bottom;
	D3DXCreateFont(device, 40, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, "Arial Black", &font);
		
	D3DXCreateTextureFromFileEx(device, "data\\interface\\group_invite_frame.png", D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
                                0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
								0, NULL, NULL, &group_invite_frame);
	
	AcceptButton = new Button(surface.left + 16, surface.bottom - 44 - 16, 128, 44, "data\\interface\\button_accept.png", boost::bind(&InvitationLayer::Process_AcceptButton, this));
	DeclineButton = new Button(surface.right - 128 - 16, surface.bottom - 44 - 16, 128, 44, "data\\interface\\button_decline.png", boost::bind(&InvitationLayer::Process_DeclineButton, this));
}

bool InvitationLayer::Visible()
{
	return game->invited && visible;
}

void InvitationLayer::Render()
{
	LPD3DXSPRITE sprite;
	D3DXCreateSprite(device, &sprite);
	sprite->Begin(0);

	sprite->Draw(group_invite_frame, NULL, NULL, &D3DXVECTOR3(surface.left, surface.top, 0), 0xFFFFFFFF);

	font->DrawText(sprite, game->inviter.c_str(), -1, &font_rectangle, DT_LEFT|DT_NOCLIP, 0xCC000000);

	AcceptButton->Render();
	DeclineButton->Render();
	
	sprite->End();
	sprite->Release();
}

void InvitationLayer::Handle_KeyUp(const UINT_PTR& _key)
{

}

bool InvitationLayer::Handle_MouseUp(const int& _x, const int& _y, const bool& _left_button)
{
	if (AcceptButton->Collide(_x, _y)) return AcceptButton->Handle_MouseUp(_x, _y, _left_button);
	if (DeclineButton->Collide(_x, _y)) return DeclineButton->Handle_MouseUp(_x, _y, _left_button);
	return false;
}

void InvitationLayer::Process_AcceptButton()
{
	game->invited = false;
	connection->Send(Connection::ClientCommand::GROUP_ACCEPT, "");
}

void InvitationLayer::Process_DeclineButton()
{
	game->invited = false;
	connection->Send(Connection::ClientCommand::GROUP_DECLINE, "");
}

InvitationLayer::~InvitationLayer()
{
	group_invite_frame->Release();
	font->Release();

	delete AcceptButton;
	delete DeclineButton;
}

