#include "defines.h"
#include "Map.h"
#include "Room.h"
#include <string>

Map::Map(std::string _debugName) : Object(_debugName)
{
}

Map::~Map()
{
	if (active) Deactivate();

	for (unsigned int i = 0; i < roomCount; i++)
	{
		delete rooms[i];
	}
	delete rooms;
}

void Map::Activate()
{
	Object::Activate();
	currentRoom->Activate();
}

void Map::Deactivate()
{
	Object::Deactivate();
	currentRoom->Deactivate();
}

void Map::Freeze()
{
	Object::Freeze();
	currentRoom->Freeze();
}

void Map::Unfreeze()
{
	Object::Unfreeze();
	currentRoom->Unfreeze();
}

const Room* Map::GetRoom(unsigned int index) const
{
	if (index < roomCount && index >= 0)
		return rooms[index];
	else
		return nullptr;
}

void Map::SetRoom(unsigned int index)
{
	if (index < roomCount)
		currentRoom = rooms[index];
}

// example for CreateRooms override
/*
void Map::CreateRooms()
{
	roomCount = 2;

	rooms = new Room * [roomCount];
	rooms[0] = new TestRoom0(std::string("TestRoom0"));
	rooms[1] = new TestRoom1(std::string("TestRoom1"));

	currentRoom = rooms[0];
}
*/
