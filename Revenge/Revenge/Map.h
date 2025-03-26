#pragma once
#include "Object.h"

class Room;
class Map : public Object
{
protected:
	Room** rooms;
	Room* currentRoom;
	unsigned int roomCount;

public:

	Map(std::string _debugName);
	~Map();

	virtual void Activate();
	virtual void Deactivate();
	virtual void Freeze();
	virtual void Unfreeze();
	virtual void Update(float delta_time) {}
	virtual void Draw(SpriteBatch* spriteBatch) {}


	const Room* CurrentRoom() const { return currentRoom; }
	const Room* GetRoom(unsigned int index) const;
	void SetRoom(unsigned int index);

protected:

	virtual void CreateRooms() = 0;
};

