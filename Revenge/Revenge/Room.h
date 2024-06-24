#pragma once
#include "Object.h"

#include <vector>

class Tile;
class Interactable;
class NonPlayer;

#define SECTOR_SIZE 4

class Room : public Object
{
	Tile*** tiles[3] = { nullptr, nullptr, nullptr };
	Point<int> dimensions;
	Interactable** interactableTiles = nullptr;
	int interactableCount = 0;
	NonPlayer** NPCs;
	int npcCount = 0;

	Tile*** sectors;

public:
	Room(std::string _debugName, const char* filepath);
	~Room();

	void Activate();
	void Deactivate();
	void Freeze();
	void Unfreeze();
	void Draw(SpriteBatch* spriteBatch) {}
	void Update(float delta_time) {}

	const Tile* GetTile(int layer, int x, int y) const;
	Point<int> GetDimensions() const;
	Interactable** const GetInteractables() const;
	int GetInteractableCount() const;
	NonPlayer** const GetNPCs() const { return NPCs; }
	int GetNPCCount() const { return npcCount; }

	std::vector<Tile*> const TestCollision(MyRectangle collisionRectangle) const;

private:

	void CreateSectors();
	std::vector<int> const GetSectors(MyRectangle collisionRectangle) const;
};

