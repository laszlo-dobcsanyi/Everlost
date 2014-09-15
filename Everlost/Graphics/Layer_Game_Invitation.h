#ifndef GRAPHICS_LAYER_GAME_INVITATION_H
#define GRAPHICS_LAYER_GAME_INVITATION_H

#include "Graphics\Layer.h"

class Button;

class InvitationLayer
	: public Layer
{
public:
	InvitationLayer(const int& _x, const int& _y, const int& _width, const int& _height);
	~InvitationLayer();

	bool Visible();

	void Render();

	void Handle_KeyUp(const UINT_PTR& _key);
	bool Handle_MouseUp(const int& _x, const int& _y, const bool& _left_button);

private:
	LPDIRECT3DTEXTURE9 group_invite_frame;

	RECT font_rectangle;
	ID3DXFont* font;

	Button* AcceptButton;
	Button* DeclineButton;

	void Process_AcceptButton();
	void Process_DeclineButton();

	InvitationLayer(const InvitationLayer& _other);
	InvitationLayer& operator=(const InvitationLayer& _other);
};

#endif