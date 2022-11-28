#include "defines.h"
#include "Room.h"
#include "Tile.h"
#include "Manager.h"
#include "OverworldManager.h"
#include "FileReader.h"
#include "Character.h"

#include <iostream>
#include <fstream>
#include <string>

Room::Room(const char* filepath)
{
#pragma region Read Data

	RoomReader roomReader;
	roomReader.Open(filepath);

	int*** layers; 
	int** doorData; 
	std::vector<std::vector<std::string>> textList; 
	dimensions = roomReader.GetLayers(&layers);
	int doorCount = roomReader.GetDoorData(&doorData);
	interactableCount = roomReader.GetTextList(textList);
	interactableTiles = new Interactable*[interactableCount];
	std::map<std::string, Point<float>> NPCPaths;
	npcCount = roomReader.GetNPCList(NPCPaths);

#pragma endregion

#pragma region Translate Read Data into Tiles

	// set up the tiles
	int doorAt = 0, interactableAt = 0;

	for (int l = 0; l < 3; l++)
	{
		tiles[l] = new Tile * *[dimensions.x];
		int x = 0, y = 0;
		for (int i = 0; i < dimensions.x; i++)
		{
			tiles[l][i] = new Tile * [dimensions.y];
			for (int j = 0; j < dimensions.y; j++)
			{
				if (layers[l][i][j] != -1 && layers[l][i][j] != -2)
				{
					const ProtoTile* prototype = OverworldManager::GetProtoTile(layers[l][i][j]);
					if (prototype->door)
					{
						if (doorAt < doorCount)
						{
							tiles[l][i][j] = new Door(prototype, (float)x, (float)y, .25f * (float)l, (int)doorData[doorAt][0], (float)doorData[doorAt][1], (float)doorData[doorAt][2]);
							doorAt++;
						}
						else
						{
							OutputDebugString(L"ERROR READING DOOR DATA");
						}
					}
					else if (prototype->interactable)
					{
						if (interactableAt < interactableCount)
						{
							Interactable* interactable = new Interactable(prototype, (float)x, (float)y, .25f * (float)l, textList[interactableAt]);
							interactableTiles[interactableAt] = interactable;
							tiles[l][i][j] = interactable;
							interactableAt++;
						}
						else
						{
							OutputDebugString(L"ERROR READING INTERACTABLE TILE DATA");
						}
					}
					else
					{
						tiles[l][i][j] = new Tile(prototype, (float)x, (float)y, .25f * (float)l);
					}
				}
				else if (layers[l][i][j] == -2)
					OutputDebugString(L"ERROR READING FILE");
				else
					tiles[l][i][j] = nullptr;
				y += (int)TILE_HEIGHT;
			}
			y = 0; x += (int)TILE_WIDTH;
		}
	}

#pragma endregion

#pragma region Read NPC Data
	
	NPCs = new NonPlayer * [npcCount];
	int NPCAt = 0;
	for (auto it = NPCPaths.begin(); it != NPCPaths.end(); ++it, ++NPCAt)
	{
		NPCs[NPCAt] = new NonPlayer(it->second.x, it->second.y, 32, 32, Manager::GetTexture((int)TEXTURES_TEST::TEX_T_PLAYER), .6f, it->first.c_str());
	}

#pragma endregion

#pragma region Clean

	for (int l = 0; l < 3; l++)
	{
		for (int i = 0; i < dimensions.x; i++)
			delete[] layers[l][i];
		delete layers[l];
	}
	delete[] layers;

	for (int i = 0; i < doorCount; i++)
	{
		delete[] doorData[i];
	}
	delete[] doorData;

#pragma endregion
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
			if (tiles[l][i])
				delete tiles[l][i];
		}
		if (tiles[l])
			delete tiles[l];
	}

	if (interactableTiles)
		delete[] interactableTiles;
	
	for (int i = 0; i < npcCount; i++)
	{
		delete NPCs[i];
	}

	if (NPCs)
		delete[] NPCs;
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
	Object::Unfreeze();
}

const Tile* Room::GetTile(int layer, int x, int y) const 
{
	if (x >= dimensions.x || x < 0 || y >= dimensions.y || y < 0)
		return nullptr;
	return tiles[layer][x][y];
}

Point<int> Room::GetDimensions() const
{
	return dimensions;	
}

Interactable** const Room::GetInteractables() const
{
	return interactableTiles;
}

int Room::GetInteractableCount() const 
{
	return interactableCount;
}