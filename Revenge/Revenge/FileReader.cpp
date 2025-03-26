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