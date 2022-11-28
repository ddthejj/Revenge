#include "defines.h"
#include "FileReader.h"
#include "SaveManager.h"
#include "Ability.h"
#include "Character.h"

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

std::vector<std::string> FileReader::ParseLine(std::string line, char delim) const
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


Point<int> RoomReader::GetDimensions() const
{
	// the file either didn't correctly open or contained nothing
	if (lines.size() == 0)
		return Point<int>(0, 0);

	std::vector<std::string> dims = ParseLine(lines[0], ',');

	return Point<int>(atoi(dims[0].c_str()), atoi(dims[1].c_str()));
}

Point<int> RoomReader::GetLayers(int**** layers) const
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

int RoomReader::GetDoorCount() const
{
	// the file either didn't correctly open or contained nothing
	if (lines.size() == 0)
		return 0;

	return atoi(lines[GetDoorCountLine()].c_str());
}

int RoomReader::GetDoorData(int*** doorData) const
{
	// the file either didn't correctly open or contained nothing
	if (lines.size() == 0)
		return 0;

	int doorCountLine = GetDoorCountLine();
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

int RoomReader::GetTextCount() const
{
	// the file either didn't correctly open or contained nothing
	if (lines.size() == 0)
		return 0;

	return atoi(lines[GetTextCountLine()].c_str());
}

int RoomReader::GetTextList(std::vector<std::vector<std::string>>& textList) const
{
	// the file either didn't correctly open or contained nothing
	if (lines.size() == 0)
		return 0;

	// read that number into an int
	int TextCountLine = GetTextCountLine();
	int TextCount = atoi(lines[TextCountLine].c_str());

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

int RoomReader::GetNPCCount() const
{
	// the file either didn't correctly open or contained nothing
	if (lines.size() == 0)
		return 0;

	return atoi(lines[GetNPCCountLine()].c_str());
}

int RoomReader::GetNPCList(std::map<std::string, Point<float>>& NPCList) const
{
	// the file either didn't correctly open or contained nothing
	if (lines.size() == 0)
		return 0;

	int NPCCountLine = GetNPCCountLine();
	int NPCCount = atoi(lines[NPCCountLine].c_str());

	int lineAt = NPCCountLine + 1;

	for (int i = 0; i < NPCCount; i++)
	{
		std::vector<std::string> line = ParseLine(lines[lineAt], ',');

		NPCList.insert(std::pair<std::string, Point<float>>(line[0], Point<float>((float)atoi(line[1].c_str()), (float)atoi(line[2].c_str()))));
		lineAt++;
	}

	return NPCCount;
}

int RoomReader::GetDoorCountLine() const
{
	// there is a lot going into this calculation, but the variables are: 
	// + 2 for the dimension line and extra space
	// y * 3 for the number of lines in the room data
	// + 3 for the number of lines in between rooms
	return (GetDimensions().y * 3) + 5;
}

int RoomReader::GetTextCountLine() const
{
	// We have to get the door count and pass those lines as well
	// then + 2 for the extra line after the door list and for the door list line itself
	return GetDoorCountLine() + GetDoorCount() + 2;
}

int RoomReader::GetNPCCountLine() const
{
	// go to the text line
	int currentLine = GetTextCountLine() + 1;
	int textCount = GetTextCount();

	// loop through the text lines to get to the end of them
	for (int i = 0; i < textCount; i++)
	{
		int lineCount = atoi(lines[currentLine].c_str());

		for (int j = 0; j < lineCount; j++)
		{
			currentLine++;
		}
		currentLine++;
	}
	// extra line for blank space at the end
	return currentLine + 1;
}


Point<int> MenuReader::GetDimensions() const
{
	// the file either didn't correctly open or contained nothing
	if (lines.size() == 0)
		return Point<int>(0, 0);

	std::vector<std::string> dims = ParseLine(lines[0], ',');
	return Point<int>(atoi(dims[0].c_str()), atoi(dims[1].c_str()));
}

int MenuReader::GetOptions(MenuReader::OptionData** optionsList) const
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

ANCHOR_POINT MenuReader::GetAnchor() const
{

	return EnumParser::ParseAnchorPoint(lines[1]);
}

int SaveReader::GetFlags(unsigned long long** flags) const
{
	(*flags) = new unsigned long long[8];

	for (int i = 0; i < 8; i++)
	{
		char* end;
		(*flags)[i] = strtoull(lines[i].c_str(), &end, 10);
	}

	return 8;
}

bool SaveWriter::WriteSave(const char* filepath)
{
	std::ofstream file(filepath, std::ios::out);

	if (!file.is_open())
		return false;

	file.clear();

	char buffer[32];
	_ui64toa_s((unsigned long long)SaveManager::mainQuestProgress, buffer, 32, 10);
	file << buffer;
	file << '\n';

	_ui64toa_s((unsigned long long)SaveManager::mainQuestDecisions, buffer, 32, 10);
	file << buffer;
	file << '\n';

	_ui64toa_s((unsigned long long)SaveManager::sideQuestProgress, buffer, 32, 10);
	file << buffer;
	file << '\n';

	_ui64toa_s((unsigned long long)SaveManager::sideQuestDecisions, buffer, 32, 10);
	file << buffer;
	file << '\n';

	_ui64toa_s((unsigned long long)SaveManager::itemsCollected, buffer, 32, 10);
	file << buffer;
	file << '\n';

	_ui64toa_s((unsigned long long)SaveManager::conversationChoices, buffer, 32, 10);
	file << buffer;
	file << '\n';

	_ui64toa_s((unsigned long long)SaveManager::defeatedEnemies, buffer, 32, 10);
	file << buffer;
	file << '\n';

	_ui64toa_s((unsigned long long)SaveManager::misc, buffer, 32, 10);
	file << buffer;
	file << '\n';

	file.close();
	return true;
}


std::string CharacterReader::GetFirstName()
{
	return lines[0];
}

std::string CharacterReader::GetLastName()
{
	return lines[1];
}

int CharacterReader::GetStartingLevel()
{
	return atoi(lines[2].c_str());
}

int CharacterReader::GetRatio()
{
	return atoi(lines[3].c_str());
}

std::vector<int> CharacterReader::GetStats()
{
	std::vector<std::string> stats_s = ParseLine(lines[4], ',');
	std::vector<int> stats_i;

	for (int i = 0; i < stats_s.size(); i++)
	{
		stats_i.push_back(atoi(stats_s[i].c_str()));
	}

	return stats_i;
}

MAGIC_TYPE CharacterReader::GetPrimaryMagic()
{
	std::vector<std::string> magics = ParseLine(lines[5], ',');

	if (ParseLine(magics[0], '-').size() > 1) // if dash is present, we're parsing a party member with multiple possible magic types
	{
		return EnumParser::ParseMagicType(magics[0]);
	}
	else
	{
		return MAGIC_TYPE::UNDECIDED;
	}
}

MAGIC_TYPE CharacterReader::GetSecondaryMagic()
{
	std::vector<std::string> magics = ParseLine(lines[5], ',');

	if (magics.size() == 1)
	{
		return MAGIC_TYPE::NONE;
	}
	else
	{
		if (ParseLine(magics[1], '-').size() > 1)
		{
			return EnumParser::ParseMagicType(magics[1]);
		}
		else
		{
			return MAGIC_TYPE::UNDECIDED;
		}
	}
}

std::vector<Ability*> CharacterReader::GetAbilities()
{
	std::vector<Ability*> abilities_a;
	std::vector<std::string> abilities_s = ParseLine(lines[6], '|');
	for (int i = 0; i < abilities_s.size(); i++)
	{
		std::vector<std::string> ability = ParseLine(abilities_s[i], ',');

		abilities_a.push_back(new Ability(ability[0], atoi(ability[1].c_str()), atoi(ability[2].c_str()), atoi(ability[3].c_str())));
	}

	return abilities_a;
}


MOVE_MODE NPCReader::GetMovementMode()
{
	return (MOVE_MODE)(atoi(ParseLine(lines[7], ',')[0].c_str()));
}

float NPCReader::GetMovementRadius()
{
	return (float)atoi(ParseLine(lines[7], ',')[1].c_str());
}
