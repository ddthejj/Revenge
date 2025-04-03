#pragma once
#include "Sprite.h"
#include <string>

class Texture;
class MyRectangle;
class Room;

struct ProtoTile
{
	const Texture* texture;
	float height, width;
	bool collidable, door, dialogueinteractable;

	ProtoTile(const Texture* _texture, float _height, float _width, bool _collidable = false, bool _door = false, bool _dialogueinteractable = false)
	{
		texture = _texture; height = _height; width = _width; collidable = _collidable; door = _door; dialogueinteractable = _dialogueinteractable;
	}
	ProtoTile(ProtoTile* that);
};

class Tile : public Sprite
{
public:
	Tile(std::string _debugName, const ProtoTile* prototype, float _x, float _y, float _layer);
	virtual ~Tile();

	virtual void Interact() {}
	virtual void Update(float delta_time);
};