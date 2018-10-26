#include "Room.h"
#include "Tile.h"
#include "Manager.h"

#include <iostream>
#include <fstream>
#include <string>

Room::Room(const char* filepath)
{
	std::ifstream room(filepath, std::ios::in);

	if (room.is_open())
	{
		// set up temp variables
		char word[255];
		int whichChar = 0;
		int** layers[3];
		// get the x dimension of the room
		do
		{
			room.get(word[whichChar]);
		} while (word[whichChar] != ',' && word[whichChar++] != '\n');
		dimX = atoi(word);
		// get the y dimension of the room
		whichChar = 0;
		do
		{
			room.get(word[whichChar]);
		} while (word[whichChar] != ',' && word[whichChar++] != '\n');
		dimY = atoi(word);
		// move past empty line
		char ret = room.get();
		// set up int layers
		for (int l = 0; l < 3; l++)
		{
			layers[l] = new int*[dimX];

			for (int i = 0; i < dimX; i++)
			{
				layers[l][i] = new int[dimY];
				for (int j = 0; j < dimY; j++)
					layers[l][i][j] = -2;
			}
		}
		// read int layers

		// for each of the 3 layers
		for (int l = 0; l < 3; l++)
		{
			// going down the y
			for (int j = 0; j < dimY; j++)
			{
				// going accross the x
				for (int i = 0; i < dimX; i++)
				{
					// read the tile index
					whichChar = 0;
					do
					{
						room.get(word[whichChar]);
					} while (word[whichChar] != ',' && word[whichChar++] != '\n' && !room.eof());

					bool eol = false;
					if (word[whichChar-1] == '\n')
						eol = true;

					word[whichChar] = '\0';
					layers[l][i][j] = atoi(word);

					if (eol)
					{
						break;
					}
				}
			}
			// move past the empty lines
			room.get();
		}
		
		// get number of doors in the room
		whichChar = 0;
		do
		{
			room.get(word[whichChar]);
		} while (word[whichChar] != ',' && word[whichChar++] != '\n' && !room.eof());

		word[whichChar] = '\0';
		int doorCount = atoi(word);
		// read into door list
		float** doorLocations = new float*[doorCount];
		for (int i = 0; i < doorCount; i++)
		{
			// set up door list
			doorLocations[i] = new float[3];
			// read into each element of the door list (room number, x, y)
			for (int j = 0; j < 3; j++)
			{
				whichChar = 0;
				do
				{
					room.get(word[whichChar]);
				} while (word[whichChar] != ',' && word[whichChar++] != '\n' && !room.eof());

				bool eol = false;
				if (word[whichChar - 1] == '\n')
					eol = true;

				word[whichChar] = '\0';
				doorLocations[i][j] = (float)atoi(word);

				if (eol)
				{
					break;
				}
			}
		}

		// reading done
		room.close();

		// set up the tiles
		int doorAt = 0;

		for (int l = 0; l < 3; l++)
		{
			tiles[l] = new Tile**[dimX];
			int x = 0, y = 0;
			for (int i = 0; i < dimX; i++)
			{
				tiles[l][i] = new Tile*[dimY];
				for (int j = 0; j < dimY; j++)
				{
					if (layers[l][i][j] != -1 && layers[l][i][j] != -2)
					{
						ProtoTile* prototype = Manager::GetProtoTile(layers[l][i][j]);
						if (prototype->door)
						{
							tiles[l][i][j] = new Door(prototype, (float)x, (float)y, .25f * (float)l, (int)doorLocations[doorAt][0], doorLocations[doorAt][1], doorLocations[doorAt][2]);
							doorAt++;
						}
						else if (prototype->interactable)
						{
							tiles[l][i][j] = new Interactable(prototype, (float)x, (float)y, .25f * (float)l);
						}
						else
							tiles[l][i][j] = new Tile(prototype, (float)x, (float)y, .25f * (float)l);
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

		for (int l = 0; l < 3; l++)
		{
			for (int i = 0; i < dimX; i++)
				delete[] layers[l][i];
			delete layers[l];
		}
		for (int i = 0; i < doorCount; i++)
		{
			delete[] doorLocations[i];
		}
		delete doorLocations;
	}

}

Room::~Room()
{
	for (int l = 0; l < 3; l++)
	{
		for (int i = 0; i < dimX; i++)
		{
			for (int j = 0; j < dimY; j++)
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
}

void Room::Activate()
{
	for (int l = 0; l < 3; l++)
	{
		for (int j = 0; j < dimY; j++)
		{
			for (int i = 0; i < dimX; i++)
			{
				if (tiles[l][i][j])
					tiles[l][i][j]->Activate();
			}
		}
	}
	Object::Activate();
}

void Room::Deactivate()
{
	for (int l = 0; l < 3; l++)
	{
		for (int j = 0; j < dimY; j++)
		{
			for (int i = 0; i < dimX; i++)
			{
				if (tiles[l][i][j])
					tiles[l][i][j]->Deactivate();
			}
		}
	}

	Object::Deactivate();
}

void Room::Freeze()
{
	for (int l = 0; l < 3; l++)
	{
		for (int j = 0; j < dimY; j++)
		{
			for (int i = 0; i < dimX; i++)
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
		for (int j = 0; j < dimY; j++)
		{
			for (int i = 0; i < dimX; i++)
			{
				if (tiles[l][i][j])
					tiles[l][i][j]->Unfreeze();
			}
		}
	}
	Object::Unfreeze();
}

Tile* Room::GetTile(int layer, int x, int y)
{
	if (x >= dimX || x < 0 || y >= dimY || y < 0)
		return nullptr;
	return tiles[layer][x][y];
}