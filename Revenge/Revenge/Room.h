#pragma once
#include "Object.h"



class Tile;
class Sprite;
class NonPlayer;
//class PhysicsManager;
//struct PhysicsManager::Sector;

class Room : public Object
{
protected:

	std::vector<std::vector<Tile*>> tiles[3];
	Point<int> dimensions;
	int dialogueCount = 0;
	std::vector<NonPlayer*> NPCs;
	int npcCount = 0;
	//std::vector<PhysicsManager::Sector> sector;

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


	Tile* GetTile(int layer, int x, int y) const;
	Point<int> GetDimensions() const { return dimensions; }
	int GetDialogueCount() const { return dialogueCount; }
	std::vector<NonPlayer*> const GetNPCs() const { return NPCs; }
	int GetNPCCount() const { return npcCount; }


protected:

	virtual RoomData CreateRoomData() = 0;
	void InterpretRoomData(RoomData roomData);
	
private:

};

