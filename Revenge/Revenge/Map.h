#pragma once
#include "Object.h"

class Room;
class Map : public Object
{
	Room** rooms;
	Room* currentRoom;
	unsigned int roomCount;

	//static bool fadingIn = false, fadingOut = false;

public:
	Map(const char* filepath, unsigned int _roomCount);
	~Map();

	virtual void Activate();
	virtual void Deactivate();
	virtual void Freeze();
	virtual void Unfreeze();
	virtual void Update(float delta_time) {}
	virtual void Draw(SpriteBatch* spriteBatch) {}

	Room* CurrentRoom() { return currentRoom; }
	Room* GetRoom(unsigned int index);
	void SetRoom(unsigned int index);
};

