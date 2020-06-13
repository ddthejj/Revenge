#pragma once
#include <vector>
#include <string>

class FileReader
{
public: 

	std::vector<std::string> lines = std::vector<std::string>();

	virtual bool Open(const char* filepath);
	virtual void Close();
};

class RoomReader : public FileReader
{
public:

	// return the dimensions of the room in tiles
	Point<int> GetDimensions();
	// return the tile layers (in ints) of the room
	Point<int> GetLayers(int**** layers);
	// return the number of doors in the room
	int GetDoorCount();
	// return the data of each door in the room
	int GetDoorData(int*** doorData);
	// return the number of interactable tiles in the room
	int GetTextCount();
	// return 
	int GetTextList(std::vector<std::string>** textList);
};

class MenuReader : public FileReader
{
public:

	struct OptionData
	{
		std::string text;
		int returnValue;
		Point<int> position, matrixLocation;
	};

	// return the dimensions of the options within the menu
	Point<int> GetDimensions();
	// returns the options within the menu
	int GetOptions(OptionData** optionsList);
};