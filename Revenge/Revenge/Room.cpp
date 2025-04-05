#include "defines.h"
#include "Room.h"
#include "Tile.h"
#include "Manager.h"
#include "OverworldManager.h"
#include "FileReader.h"
#include "Character.h"
#include "InteractionComponent.h"
#include "TriggerComponent.h"
#include "Rectangle.h"
#include "PhysicsManager.h"

#include <iostream>
#include <fstream>
#include <string>

Room::Room(std::string _debugName) : Object(_debugName)
{

}

Room::~Room()
{
	if (active) Deactivate();

	for (int l = 0; l < 3; l++)
	{
		for (int i = 0; i < dimensions.x; i++)
		{
			for (int j = 0; j < dimensions.y; j++)
			{
				if (tiles[l][i][j])
					delete tiles[l][i][j];
			}
		}
	}

	for (int i = 0; i < npcCount; i++)
	{
		delete NPCs[i];
	}
}

void Room::Activate()
{
	for (int l = 0; l < 3; l++)
	{
		for (int j = 0; j < dimensions.y; j++)
		{
			for (int i = 0; i < dimensions.x; i++)
			{
				if (tiles[l][i][j])
					tiles[l][i][j]->Activate();
			}
		}
	}

	for (int i = 0; i < npcCount; i++)
	{
		NPCs[i]->Activate();
	}

	PhysicsManager::CreateSectors();

	Object::Activate();
}

void Room::Deactivate()
{
	for (int l = 0; l < 3; l++)
	{
		for (int j = 0; j < dimensions.y; j++)
		{
			for (int i = 0; i < dimensions.x; i++)
			{
				if (tiles[l][i][j])
					tiles[l][i][j]->Deactivate();
			}
		}
	}

	for (int i = 0; i < npcCount; i++)
	{
		NPCs[i]->Deactivate();
	}

	Object::Deactivate();
}

void Room::Freeze()
{
	for (int l = 0; l < 3; l++)
	{
		for (int j = 0; j < dimensions.y; j++)
		{
			for (int i = 0; i < dimensions.x; i++)
			{
				if (tiles[l][i][j])
					tiles[l][i][j]->Freeze();
			}
		}
	}

	for (int i = 0; i < npcCount; i++)
	{
		NPCs[i]->Freeze();
	}

	Object::Freeze();
}

void Room::Unfreeze()
{
	for (int l = 0; l < 3; l++)
	{
		for (int j = 0; j < dimensions.y; j++)
		{
			for (int i = 0; i < dimensions.x; i++)
			{
				if (tiles[l][i][j])
					tiles[l][i][j]->Unfreeze();
			}
		}
	}

	for (int i = 0; i < npcCount; i++)
	{
		NPCs[i]->Unfreeze();
	}
	Object::Unfreeze();
}

Tile* Room::GetTile(int layer, int x, int y) const
{
	if (x >= dimensions.x || x < 0 || y >= dimensions.y || y < 0)
		return nullptr;
	return tiles[layer][y][x];
}

void Room::InterpretRoomData(RoomData roomData)
{

#pragma region Translate Data into Tiles

	// set up the tiles
	int doorAt = 0, dialogueAt = 0;

	for (int l = 0; l < 3; l++)
	{
		tiles[l] = std::vector<std::vector<Tile*>>();
		int x = 0, y = 0;
		for (int i = 0; i < dimensions.y; i++)
		{
			tiles[l].push_back(std::vector<Tile*>());
			for (int j = 0; j < dimensions.x; j++)
			{
				if (roomData.layers[l][i][j] != -1 && roomData.layers[l][i][j] != -2)
				{
					const ProtoTile* prototype = OverworldManager::GetProtoTile(roomData.layers[l][i][j]);
					Tile* newTile = new Tile(std::string("Tile") + std::to_string(l) + "," + std::to_string(i) + "," + std::to_string(j), prototype, (float)x, (float)y, .25f * (float)l);

					if (prototype->door)
					{
						if (doorAt < roomData.doorCount)
						{
							newTile->AddComponent(new DoorComponent(newTile, newTile->GetRectangle(), roomData.doorData[doorAt][0], roomData.doorData[doorAt][1], roomData.doorData[doorAt][2]));
							doorAt++;
						}
					}
					if (prototype->dialogueinteractable)
					{
						if (dialogueAt < dialogueCount)
						{
							newTile->AddComponent(new DialogueComponent(newTile, newTile->GetRectangle(), roomData.textList[dialogueAt]));
							dialogueAt++;
						}
					}

					tiles[l][i].push_back(newTile);
				}
				else if (roomData.layers[l][i][j] == -2)
				{
					OutputDebugString(L"ERROR READING FILE");
				}
				else
				{
					tiles[l][i].push_back(nullptr);
				}
				x += (int)TILE_HEIGHT;
			}
			x = 0; y += (int)TILE_WIDTH;
		}
	}


#pragma endregion

}