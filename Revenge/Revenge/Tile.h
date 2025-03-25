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
	bool collidable, door, interactable;

	ProtoTile(const Texture* _texture, float _height, float _width, bool _collidable = false, bool _door = false, bool _interactable = false)
	{
		texture = _texture; height = _height; width = _width; collidable = _collidable; door = _door; interactable = _interactable;
	}
	ProtoTile(ProtoTile* that);
};

class Tile : public Sprite
{
	bool collidable;

public:
	Tile(std::string _debugName, const ProtoTile* prototype, float _x, float _y, float _layer);
	virtual ~Tile();

	bool Collidable() const { return collidable; }

	virtual void Interact() {}
	virtual void Update(float delta_time);
};

class Door : public Tile
{
	int destination;
	float destX = 0, destY = 0;

public:
	Door(std::string _debugName, const ProtoTile* prototype, float _x, float _y, float _layer, int _destination, float _destX, float _destY);
	virtual ~Door();

	int Destination() const { return destination; }
	float DestinationX() const { return destX; }
	float DestinationY() const { return destY; }

	virtual void Update(float delta_time);
};

class Interactable : public Tile
{
	std::vector<std::string> lines;

public:
	Interactable(std::string _debugName, const ProtoTile* prototype, float _x, float _y, float _layer, std::vector<std::string> _lines);
	virtual ~Interactable();

	virtual void Interact() const;
	virtual void Update(float delta_time);
};