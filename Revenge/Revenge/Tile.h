#pragma once
#include "Sprite.h"
#include <string>
class Texture;
class MyRectangle;
class Room;

struct ProtoTile
{
	Texture* texture;
	float height, width;
	bool collidable, door, interactable;

	ProtoTile(Texture* _texture, float _height, float _width, bool _collidable = false, bool _door = false, bool _interactable = false)
	{
		texture = _texture; height = _height; width = _width; collidable = _collidable; door = _door; interactable = _interactable;
	}
};

class Tile : public Sprite
{
	bool collidable;

public:
	Tile(ProtoTile* prototype, float _x, float _y, float _layer);
	virtual ~Tile();

	bool Collidable() { return collidable; }

	virtual void Interact() {}
	virtual void Update();
};

class Door : public Tile
{
	int destination;
	float destX = 0, destY = 0;

public:
	Door(ProtoTile* prototype, float _x, float _y, float _layer, int _destination, float _destX, float _destY);
	virtual ~Door();

	int Destination() { return destination; }
	float DestinationX() { return destX; }
	float DestinationY() { return destY; }

	virtual void Update();
};

class Interactable : public Tile
{
	std::string* lines;

public:
	Interactable(ProtoTile* prototype, float _x, float _y, float _layer, std::string* _lines);
	virtual ~Interactable();

	virtual void Interact();
	virtual void Update();
};