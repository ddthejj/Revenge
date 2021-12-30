#pragma once
#include "Object.h"

class Tile;
class Interactable;

class Room : public Object
{
	Tile*** tiles[3] = { nullptr, nullptr, nullptr };
	Point<int> dimensions;
	Interactable** interactableTiles = nullptr;
	int interactableCount = 0;

public:
	Room(const char* filepath);
	~Room();

	void Activate();
	void Deactivate();
	void Freeze();
	void Unfreeze();
	void Draw(SpriteBatch* spriteBatch) {}
	void Update(float delta_time) {}

	Tile* GetTile(int layer, int x, int y);
	Point<int> GetDimensions();
	Interactable** GetInteractables();
	int GetInteractableCount();
};

