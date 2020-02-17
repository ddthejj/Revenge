#pragma once
#include "Object.h"

class Tile;

class Room : public Object
{
	Tile*** tiles[3] = { nullptr, nullptr, nullptr };
	int dimX = 0, dimY = 0;

public:
	Room(const char* filepath);
	~Room();

	void Activate();
	void Deactivate();
	void Freeze();
	void Unfreeze();

	Tile* GetTile(int layer, int x, int y);
};

