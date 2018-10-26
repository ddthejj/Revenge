#pragma once
#include "Sprite.h"
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
	~Tile();
	void Update();
	bool Collidable() { return collidable; }
};

class Door : public Tile
{
	int destination;
	float destX = 0, destY = 0;

public:
	Door(ProtoTile* prototype, float _x, float _y, float _layer, int _destination, float _destX, float _destY);
	~Door();

	int Destination() { return destination; }
	float DestinationX() { return destX; }
	float DestinationY() { return destY; }

	void Update();
};

class Interactable : public Tile
{

public:
	Interactable(ProtoTile* prototype, float _x, float _y, float _layer);
	~Interactable();

	void Update();
};