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

std::vector<std::string> FileReader::ParseLine(std::string line, char delim)
{
	std::vector<std::string> lineList;

	int charAt = 0;
	while (charAt < line.size())
	{
		char buffer[2048];
		int bufferAt = 0;

		while (charAt < line.size() && line[charAt] != delim)
		{
			buffer[bufferAt] = line[charAt];
			charAt++;
			bufferAt++;
		}

		buffer[bufferAt] = '\0';
		lineList.push_back(std::string(buffer));
		charAt++;
	}


	return lineList;
}


Point<int> RoomReader::GetDimensions()
{
	// the file either didn't correctly open or contained nothing
	if (lines.size() == 0)
		return Point<int>(0, 0);

	std::vector<std::string> dims = ParseLine(lines[0], ',');

	return Point<int>(atoi(dims[0].c_str()), atoi(dims[1].c_str()));
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
			std::vector<std::string> line = ParseLine(lines[roomline], ',');
			// read across the file
			for (int x = 0; x < dims.x; x++)
			{
				// convert the string to an integer
				(*layers)[l][x][y] = atoi(line[x].c_str());
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
		std::vector<std::string> line = ParseLine(lines[doorCountLine + i + 1], ',');
		// for the 3 variables in each door
		for (int j = 0; j < 3; j++)
		{
			(*doorData)[i][j] = atoi(line[j].c_str());
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

int RoomReader::GetTextList(std::vector<std::vector<std::string>>& textList)
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
	//(*textList) = new std::vector<std::string>[TextCount];
	// which line we are currently reading
	int lineAt = TextCountLine + 1;

	// for each interactable object in the room
	for (int i = 0; i < TextCount; i++)
	{
		int tileLines = atoi(lines[lineAt++].c_str());
		textList.push_back(std::vector<std::string>());
		for (int j = 0; j < tileLines; j++)
		{
			textList[i].push_back(lines[lineAt]);
			lineAt++;
		}
	}

	return TextCount;
}


Point<int> MenuReader::GetDimensions()
{
	// the file either didn't correctly open or contained nothing
	if (lines.size() == 0)
		return Point<int>(0, 0);

	std::vector<std::string> dims = ParseLine(lines[0], ',');
	return Point<int>(atoi(dims[0].c_str()), atoi(dims[1].c_str()));
}

int MenuReader::GetOptions(MenuReader::OptionData** optionsList)
{
	// get the number of options
	int optionCount = atoi(lines[2].c_str());
	*optionsList = new OptionData[optionCount];

	for (int i = 0; i < optionCount; i++)
	{
		int lineAt = i + 3;

		std::vector<std::string> optionData = ParseLine(lines[lineAt], ',');

		(*optionsList)[i].text = optionData[0];
		(*optionsList)[i].returnValue = atoi(optionData[1].c_str());
		(*optionsList)[i].position.x = atoi(optionData[2].c_str());
		(*optionsList)[i].position.y = atoi(optionData[3].c_str());
		(*optionsList)[i].matrixLocation.x = atoi(optionData[4].c_str());
		(*optionsList)[i].matrixLocation.y = atoi(optionData[5].c_str());
	}

	return optionCount;
}

ANCHOR_POINT MenuReader::GetAnchor()
{
	
	return EnumParser::ParseAnchorPoint(lines[1]);
}
