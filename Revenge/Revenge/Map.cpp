#include "defines.h"
#include "Map.h"
#include "Room.h"
#include <string>

Map::Map(std::string _debugName, const char * filepath, unsigned int _roomCount) : Object(_debugName)
{
	roomCount = _roomCount;
	rooms = new Room*[roomCount];

	for (unsigned int i = 0; i < roomCount; i++)
	{
		std::string finalPath = filepath;
		char number[32]; _itoa_s(i, number, 32);
		finalPath += number;
		finalPath += ".txt";

		rooms[i] = new Room(std::string("Room") + std::to_string(i), finalPath.c_str());
	}

	currentRoom = rooms[0];
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