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

// example for CreateRoomData override
/*
Room::RoomData Room::CreateRoomData()
{
	std::vector<std::vector<std::vector<int>>> layers =
	{
		{
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1},
			{-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1},
			{-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1},
			{-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1},
			{-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1},
			{-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1},
			{-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1},
			{-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1},
			{-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1},
			{-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1},
			{-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1},
			{-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1},
			{-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1},
			{-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
		},

		{
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1},
			{1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1},
			{1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1},
			{1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1},
			{1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1},
			{1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,1},
			{1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,1},
			{1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,1},
			{1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,1},
			{1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1},
			{1,-1,-1,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1},
			{1,-1,-1,-1,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1},
			{1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1},
			{1,3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1},
			{1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1}
		},

		{
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
		}
	};

	std::vector<std::vector<int>> doorData = { {1, 240, 33}, {1, 240, 33} };
	std::vector<std::vector<std::string>> textList = 
	{ 
		{"Hello!"}, 
		{"Que es esta? Donde esta el queso de mi padre? Quien sus pantalones? This is a violation of basic human decency. That's super weird, so I wasn't thinking that much about it. Ooh look, a robin! That's really interesting. Robins that fly into windows are less interesting. Hmmmmmmmm? Continuing testing, since the font size has lowered. We're now testing on bigger boxes, trying to see if they can contain even more text. How much text could a text box box if a text box could box text? This sentence should send us over the edge, rolling us into a new box. Hopefully. Just kidding, we need a bit more. Wow, we can hold a lot of text. This seems like a lot of text. Maybe the font size should be bigger. "},
		{"What?", "Who?", "When?" "Why?"}
	};
	dimensions = Point<int>(16, 16);
	int doorCount = 2;
	interactableCount = 3;
	interactableTiles = new Interactable * [interactableCount];
	npcCount = 1;

#pragma region Create NPC Data

	NPCs = new NonPlayer * [npcCount];

	NPCs[0] = new TestNPC(std::string("NPC") + std::to_string(0), 200, 200, 32, 32, Manager::GetTexture((int)TEXTURES_TEST::TEX_T_PLAYER), .6f);

#pragma endregion

	return RoomData(std::vector<std::vector<std::vector<int>>>(), 0, std::vector<std::vector<int>>(), std::vector<std::vector<std::string>>());
}
*/

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

void Room::InterpretRoomData(RoomData roomData)
{

#pragma region Translate Data into Tiles

	// set up the tiles
	int doorAt = 0, interactableAt = 0;

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
					if (prototype->door)
					{
						if (doorAt < roomData.doorCount)
						{
							tiles[l][i].push_back(new Door(std::string("Door") + std::to_string(doorAt), prototype, (float)x, (float)y, .25f * (float)l, (int)roomData.doorData[doorAt][0], (float)roomData.doorData[doorAt][1], (float)roomData.doorData[doorAt][2]));
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
							Interactable* interactable = new Interactable(std::string("Interactable") + std::to_string(interactableAt), prototype, (float)x, (float)y, .25f * (float)l, roomData.textList[interactableAt]);
							interactableTiles.push_back(interactable);
							tiles[l][i].push_back(interactable);
							interactableAt++;
						}
						else
						{
							OutputDebugString(L"ERROR READING INTERACTABLE TILE DATA");
						}
					}
					else
					{
						tiles[l][i].push_back(new Tile(std::string("Tile") + std::to_string(l) + "," + std::to_string(i) + "," + std::to_string(j), prototype, (float)x, (float)y, .25f * (float)l));
					}
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

	CreateSectors();

#pragma endregion

}

void Room::CreateSectors()
{
	int sectorDimsX = (int)std::ceil(dimensions.x / SECTOR_SIZE);
	int sectorDimsY = (int)std::ceil(dimensions.y / SECTOR_SIZE);

	sectors = std::vector<std::vector<Tile*>>();

	for (int sectorAt = 0; sectorAt < sectorDimsX * sectorDimsY; sectorAt++)
	{
		sectors.push_back(std::vector<Tile*>());
	}

	// segment the map into sectors to help reduce collision checking time
	for (int y = 0; y < dimensions.y; y++)
	{
		for (int x = 0; x < dimensions.x; x++)
		{
			sectors[int(std::floor(x / SECTOR_SIZE) * sectorDimsX + std::floor(y / SECTOR_SIZE))].push_back(tiles[1][x][y]);
		}
	}
}
