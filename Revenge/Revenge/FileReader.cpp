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

std::vector<unsigned long long> SaveReader::GetFlags() const
{
	std::vector<unsigned long long> flags;

	for (int i = 0; i < 8; i++)
	{
		char* end;
		flags.push_back(strtoull(lines[i].c_str(), &end, 10));
	}

	return flags;
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