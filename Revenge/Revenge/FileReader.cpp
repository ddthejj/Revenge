#include "defines.h"
#include "FileReader.h"
#include <Windows.h>

#include <iostream>
#include <fstream>

bool FileReader::Open(const char* filepath)
{
	std::ifstream file(filepath, std::ios::in);

	if (!file.is_open())
		return false;

	// read every line from the file into a big vector to be parsed through later
	char buffer[2048];
	while (file.getline(buffer, 2048))
	{
		lines.push_back(std::string(buffer));
	}

	file.close();
	return true;
}

void FileReader::Close()
{
	// ifstream is already closed, all there is to do is to clear the vector
	lines.clear();
}

Point<int> RoomReader::GetDimensions()
{
	// the file either didn't correctly open or contained nothing
	if (lines.size() == 0)
		return Point<int>(0, 0);

	Point<int> dimensions;

	// read the x dimension
	char xBuffer[8];
	int charAt = 0;
	for (; lines[0][charAt] != ','; charAt++)
	{
		xBuffer[charAt] = lines[0][charAt];
	}
	xBuffer[charAt] = '\0';
	dimensions.x = atoi(xBuffer);

	// read the y dimension
	char yBuffer[8];
	charAt++;
	for (int i = 0; charAt < lines[0].size(); charAt++)
	{
		yBuffer[i] = lines[0][charAt];
		i++;
	}
	yBuffer[charAt] = '\0';
	dimensions.y = atoi(yBuffer);

	return dimensions;
}

Point<int> RoomReader::GetLayers(int**** layers)
{
	// the file either didn't correctly open or contained nothing
	if (lines.size() == 0)
		return Point<int>(0, 0);

	// all rooms will be 3 layers
	(*layers) = new int** [3];
	Point<int> dims = GetDimensions();

	// initialize each of the layers to the size of [x][y]
	for (int l = 0; l < 3; l++)
	{
		(*layers)[l] = new int* [dims.x];
		for (int x = 0; x < dims.x; x++)
		{
			(*layers)[l][x] = new int[dims.y];
			for (int y = 0; y < dims.y; y++)
			{
				(*layers)[l][x][y] = -2;
			}
		}
	}

	// room reading starts at 
	int roomline = 2;

	// read each of the 3 blocks of ints
	for (unsigned int l = 0; l < 3; l++)
	{
		// read down the file
		for (unsigned int y = 0; y < (unsigned int)dims.y; y++)
		{
			// set reading variables
			int whichchar = 0;
			int bufferchar = 0;
			char buffer[16];
			// read across the file
			for (int x = 0; x < dims.x; x++)
			{
				// read until you hit the end of the line or the delimiter
				while (whichchar < lines[roomline].size() && lines[roomline][whichchar] != ',')
				{
					buffer[bufferchar] = lines[roomline][whichchar];
					bufferchar++;
					whichchar++;
				}
				// indicate the end of the string
				buffer[bufferchar] = '\0';
				// convert the string to an integer
				(*layers)[l][x][y] = atoi(buffer);

				// reset reading variables
				bufferchar = 0;
				buffer[0] = '\0';
				whichchar++;
			}
			// increment room line after hitting the end of the line
			roomline++;
		}
		// increment room line an extra time after hitting the end of a layer (there's an extra line between layers)
		roomline++;
	}

	return dims;
}

int RoomReader::GetDoorCount()
{
	// the file either didn't correctly open or contained nothing
	if (lines.size() == 0)
		return 0;

	// there is a lot going into this calculation, but the variables are: 
	// + 2 for the dimension line and extra space
	// y * 3 for the number of lines in the room data
	// + 3 for the number of lines in between rooms
	return atoi(lines[(GetDimensions().y * 3) + 5].c_str());
}

int RoomReader::GetDoorData(int*** doorData)
{
	// the file either didn't correctly open or contained nothing
	if (lines.size() == 0)
		return 0;

	// there is a lot going into this calculation, but the variables are: 
	// + 2 for the dimension line and extra space
	// y * 3 for the number of lines in the room data
	// + 3 for the number of lines in between rooms
	int doorCountLine = (GetDimensions().y * 3) + 5;
	int doorCount = atoi(lines[doorCountLine].c_str());
	// initialize passed in variable
	(*doorData) = new int* [doorCount];

	// for each door
	for (int i = 0; i < doorCount; i++)
	{
		// initialize list (all doors will have 3 variables: room number, and then x and y position in that room
		(*doorData)[i] = new int[3];
		// set reading variables 
		int whichchar = 0;
		int bufferchar = 0;
		char buffer[16];
		// for the 3 variables in each door
		for (int j = 0; j < 3; j++)
		{
			// read until you hit the end of the line or the delimeter
			while (whichchar < lines[doorCountLine + i + 1].size() && lines[doorCountLine + i + 1][whichchar] != ',')
			{
				buffer[bufferchar] = lines[doorCountLine + i + 1][whichchar];
				bufferchar++;
				whichchar++;
			}
			// end the string
			buffer[bufferchar] = '\0';
			// put the read data in to the return variable
			(*doorData)[i][j] = atoi(buffer);

			// clear reading variables
			bufferchar = 0;
			buffer[0] = '\0';
			whichchar++;
		}
	}

	return doorCount;
}

int RoomReader::GetTextCount()
{
	// the file either didn't correctly open or contained nothing
	if (lines.size() == 0)
		return 0;

	// there is a lot going into this calculation, but the variables are: 
	// + 2 for the dimension line and extra space
	// y * 3 for the number of lines in the room data
	// + 3 for the number of lines in between rooms
	// this gets us to the start of the door list. We have to get the door count and pass those lines as well
	// then + 2 for the extra line after the door list and for the door list line itself
	return atoi(lines[(GetDimensions().y * 3) + 5 + GetDoorCount() + 2].c_str());
}

int RoomReader::GetTextList(std::vector<std::string>** textList)
{
	// the file either didn't correctly open or contained nothing
	if (lines.size() == 0)
		return 0;

	// there is a lot going into this calculation, but the variables are: 
	// + 2 for the dimension line and extra space
	// y * 3 for the number of lines in the room data
	// + 3 for the number of lines in between rooms
	// this gets us to the start of the door list. We have to get the door count and pass those lines as well
	// then + 2 for the extra line after the door list and for the door list line itself
	int TextCountLine = (GetDimensions().y * 3) + 5 + GetDoorCount() + 2;
	// read that number into an int
	int TextCount = atoi(lines[TextCountLine].c_str());

	// initialize passed in variable
	(*textList) = new std::vector<std::string>[TextCount];
	// which line we are currently reading
	int lineAt = TextCountLine + 1;

	// for each interactable object in the room
	for (int i = 0; i < TextCount; i++)
	{
		// repeat this until we're told to manually end the lines for this tile
		// this can't be "until end of line" because sometimes they span multiple lines, with the end signaled by a \" character
		while (true)
		{
			// set reading variables
			bool tileDone = false;
			int whichchar = 0;
			int bufferchar = 0;
			char buffer[2048];

			// read until end of line
			while (whichchar < lines[lineAt].size())
			{
				// if this is the delimeter but it's the first character in the tile
				if (whichchar == 0 && lines[lineAt][whichchar] == '\"')
				{
					// just skip this char
					whichchar++;
					continue;
				}
				// if we've hit the delimeter
				else if (lines[lineAt][whichchar] == '\"')
				{
					// we've read all that we want to put in this tile
					tileDone = true;
					break;
				}
				// add the letter to the buffer
				buffer[bufferchar] = lines[lineAt][whichchar];
				bufferchar++;
				whichchar++;
			}
			// end the string
			buffer[bufferchar] = '\0';
			// add it to this tile's list of text
			(*textList)[i].push_back(std::string(buffer));

			// reset reading variables
			bufferchar = 0;
			buffer[0] = '\0';
			whichchar++;
			lineAt++;

			// if we're done reading the tile, break out of the while loop
			if (tileDone)
				break;
		}
	}

	return TextCount;
}


Point<int> MenuReader::GetDimensions()
{
	// the file either didn't correctly open or contained nothing
	if (lines.size() == 0)
		return Point<int>(0, 0);

	Point<int> dimensions;

	// read the x dimension
	char xBuffer[8];
	int charAt = 0;
	for (; lines[0][charAt] != ','; charAt++)
	{
		xBuffer[charAt] = lines[0][charAt];
	}
	xBuffer[charAt] = '\0';
	dimensions.x = atoi(xBuffer);

	// read the y dimension
	char yBuffer[8];
	charAt++;
	for (int i = 0; charAt < lines[0].size(); charAt++)
	{
		yBuffer[i] = lines[0][charAt];
		i++;
	}
	yBuffer[charAt] = '\0';
	dimensions.y = atoi(yBuffer);

	return dimensions;
}

int MenuReader::GetOptions(MenuReader::OptionData** optionsList)
{
	// number of options = number of lines - 1 for the dimensions line
	int optionCount = (int)lines.size() - 1;
	*optionsList = new OptionData[optionCount];

	for (int i = 0; i < optionCount; i++)
	{
		int lineAt = i + 1;

		char buffer[256];
		int charAt = 0;

		// read the text of the option
		for (int j = 0; lines[lineAt][charAt] != ','; charAt++, j++)
		{
			buffer[j] = lines[lineAt][charAt];
		}
		buffer[charAt] = '\0';
		(*optionsList)[i].text = std::string(buffer);

		ZeroMemory(buffer, sizeof(buffer));
		charAt++;
		// read the return value of the option
		for (int j = 0; lines[lineAt][charAt] != ','; charAt++, j++)
		{
			buffer[j] = lines[lineAt][charAt];
		}
		buffer[charAt] = '\0';
		(*optionsList)[i].returnValue = atoi(buffer);

		ZeroMemory(buffer, sizeof(buffer));
		charAt++;
		// read the location of the option relative to the location of the menu
		// x
		for (int j = 0; lines[lineAt][charAt] != ','; charAt++, j++)
		{
			buffer[j] = lines[lineAt][charAt];
		}
		buffer[charAt] = '\0';
		(*optionsList)[i].position.x = atoi(buffer);

		ZeroMemory(buffer, sizeof(buffer));
		charAt++;
		// y
		for (int j = 0; lines[lineAt][charAt] != ','; charAt++, j++)
		{
			buffer[j] = lines[lineAt][charAt];
		}
		buffer[charAt] = '\0';
		(*optionsList)[i].position.y = atoi(buffer);

		ZeroMemory(buffer, sizeof(buffer));
		charAt++;
		// read the location of the option within the menu's selection matrix
		// x
		for (int j = 0; lines[lineAt][charAt] != ','; charAt++, j++)
		{
			buffer[j] = lines[lineAt][charAt];
		}
		buffer[charAt] = '\0';
		(*optionsList)[i].matrixLocation.x = atoi(buffer);

		ZeroMemory(buffer, sizeof(buffer));
		charAt++;
		// y
		for (int j = 0; charAt < lines[lineAt].size() && lines[lineAt][charAt] != ','; charAt++, j++)
		{
			buffer[j] = lines[lineAt][charAt];
		}
		buffer[charAt] = '\0';
		(*optionsList)[i].matrixLocation.y = atoi(buffer);
	}

 	return optionCount;
}
