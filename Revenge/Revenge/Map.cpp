#include "defines.h"
#include "Map.h"
#include "Room.h"
#include "PhysicsManager.h"

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
	{
		currentRoom = rooms[index];
	}
}
