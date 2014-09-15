#ifndef GRAPHICS_LAYER_GAME_GROUP_H
#define GRAPHICS_LAYER_GAME_GROUP_H

#include "Graphics\Layer.h"

class GroupLayer
	: public Layer
{
public:
	GroupLayer(const int& _x, const int& _y, const int& _width, const int& _height);
	~GroupLayer();

	bool Visible();
	bool Collide(const int& _x, const int& _y);

	void Render();

	void Handle_KeyUp(const UINT_PTR& _key);
	bool Handle_MouseUp(const int& _x, const int& _y, const bool& _left_button);

private:
	ID3DXFont* font;
	LPDIRECT3DTEXTURE9 group_frame;

	GroupLayer(const GroupLayer& _other);
	GroupLayer& operator=(const GroupLayer& _other);
};

#endif