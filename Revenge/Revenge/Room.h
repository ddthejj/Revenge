#pragma once
#include "Object.h"



class Tile;
class Sprite;
class Interactable;
class NonPlayer;

#define SECTOR_SIZE 4

class Room : public Object
{
protected:

	std::vector<std::vector<Tile*>> tiles[3];
	Point<int> dimensions;
	std::vector<Interactable*> interactableTiles;
	int interactableCount = 0;
	std::vector<NonPlayer*> NPCs;
	int npcCount = 0;

	std::vector<std::vector<Tile*>> sectors;

	struct RoomData
	{
		RoomData(std::vector<std::vector<std::vector<int>>> _layers, int _doorCount, std::vector<std::vector<int>> _doorData, std::vector<std::vector<std::string>> _textList)
		{
			layers = _layers;
			doorCount = _doorCount;
			doorData = _doorData;
			textList = _textList;
		}

		std::vector<std::vector<std::vector<int>>> layers;
		int doorCount;
		std::vector<std::vector<int>> doorData;
		std::vector<std::vector<std::string>> textList;
	};

public:

	Room(std::string _debugName);
	~Room();

	void Activate();
	void Deactivate();
	void Freeze();
	void Unfreeze();
	void Draw(SpriteBatch* spriteBatch) {}
	void Update(float delta_time) {}


	const Tile* GetTile(int layer, int x, int y) const;
	Point<int> GetDimensions() const { return dimensions; }
	std::vector<Interactable*> const GetInteractables() const { return interactableTiles; }
	int GetInteractableCount() const { return interactableCount; }
	std::vector<NonPlayer*> const GetNPCs() const { return NPCs; }
	int GetNPCCount() const { return npcCount; }

	std::vector<Sprite*> const TestCollision(Sprite* collider, MyRectangle collisionRectangle) const;

protected:

	virtual RoomData CreateRoomData() = 0;
	void InterpretRoomData(RoomData roomData);
	
private:

	void CreateSectors();
	std::vector<int> const GetSectors(MyRectangle collisionRectangle) const;
};

