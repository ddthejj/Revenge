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

Room::Room(std::string _debugName, const char* filepath) : Object(_debugName)
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
	interactableTiles = new Interactable * [interactableCount];
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
							tiles[l][i][j] = new Door(std::string("Door") + std::to_string(doorAt), prototype, (float)x, (float)y, .25f * (float)l, (int)doorData[doorAt][0], (float)doorData[doorAt][1], (float)doorData[doorAt][2]);
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
							Interactable* interactable = new Interactable(std::string("Interactable") + std::to_string(interactableAt), prototype, (float)x, (float)y, .25f * (float)l, textList[interactableAt]);
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
						tiles[l][i][j] = new Tile(std::string("Tile") + std::to_string(l) + "," + std::to_string(i) + "," + std::to_string(j), prototype, (float)x, (float)y, .25f * (float)l);
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
		NPCs[NPCAt] = new NonPlayer(std::string("NPC") + std::to_string(NPCAt), it->second.x, it->second.y, 32, 32, Manager::GetTexture((int)TEXTURES_TEST::TEX_T_PLAYER), .6f, it->first.c_str());
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

	CreateSectors();
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

	if (sectors)
	{
		int sectorCount = (int)std::ceil(dimensions.x / SECTOR_SIZE) * (int)std::ceil(dimensions.y / SECTOR_SIZE);
		for (int i = 0; i < sectorCount; i++)
		{
			delete[] sectors[i];
		}
		delete[] sectors;
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

std::vector<Sprite*> const Room::TestCollision(Sprite* collider, MyRectangle collisionRectangle) const
{
	std::vector<int> collidedSectors = GetSectors(collisionRectangle);
	int sectorTileNum = SECTOR_SIZE * SECTOR_SIZE;
	std::vector<Sprite*> collidedSprites;

	for (int i = 0; i < collidedSectors.size(); i++)
	{
		for (int j = 0; j < sectorTileNum; j++)
		{
			if (sectors[collidedSectors[i]][j] && sectors[collidedSectors[i]][j]->Collidable() && sectors[collidedSectors[i]][j]->GetRectangle()->Intersects(collisionRectangle))
			{
				collidedSprites.push_back(sectors[collidedSectors[i]][j]);
			}
		}
	}

	for (int i = 0; i < npcCount; i++)
	{
		if (NPCs[i]->GetRectangle()->Intersects(collisionRectangle))
		{
			if (collider != NPCs[i])
			{
				collidedSprites.push_back(NPCs[i]);
			}
		}
	}


	const Player* currentPlayer = OverworldManager::GetCurrentPlayer();
	if (currentPlayer->GetRectangle()->Intersects(collisionRectangle))
	{
		if (collider != currentPlayer)
		{
			collidedSprites.push_back((Sprite*)currentPlayer);
		}
	}

	return collidedSprites;
}

std::vector<int> const Room::GetSectors(MyRectangle collisionRectangle) const
{
	int sectorDimsX = (int)std::ceil(dimensions.x / SECTOR_SIZE);
	int sectorDimsY = (int)std::ceil(dimensions.y / SECTOR_SIZE);
	int sectorNum = sectorDimsX * sectorDimsY;

	std::vector<int> outSectors;

	for (int i = 0; i < sectorNum; i++)
	{
		MyRectangle sectorRectangle(i % sectorDimsX * SECTOR_SIZE * TILE_WIDTH, (float)std::floor(i / sectorDimsY) * SECTOR_SIZE * TILE_HEIGHT, SECTOR_SIZE * TILE_WIDTH, SECTOR_SIZE * TILE_HEIGHT);

		if (outSectors.size() == 0 && sectorRectangle.Contains(collisionRectangle))
		{
			outSectors.push_back(i);
			return outSectors;
		}
		if (sectorRectangle.Intersects(collisionRectangle))
		{
			outSectors.push_back(i);
		}
	}

	return outSectors;
}

void Room::CreateSectors()
{
	int sectorDimsX = (int)std::ceil(dimensions.x / SECTOR_SIZE);
	int sectorDimsY = (int)std::ceil(dimensions.y / SECTOR_SIZE);

	sectors = new Tile * *[sectorDimsX * sectorDimsY];

	for (int sectorAt = 0; sectorAt < sectorDimsX * sectorDimsY; sectorAt++)
	{
		sectors[sectorAt] = new Tile * [SECTOR_SIZE * SECTOR_SIZE];
	}

	// segment the map into sectors to help reduce collision checking time
	for (int y = 0; y < dimensions.y; y++)
	{
		for (int x = 0; x < dimensions.x; x++)
		{
			sectors[int(std::floor(y / SECTOR_SIZE) * sectorDimsX + std::floor(x / SECTOR_SIZE))][int(std::floor(y % SECTOR_SIZE) * SECTOR_SIZE + std::floor(x % SECTOR_SIZE))] = tiles[1][x][y];
		}
	}
}
