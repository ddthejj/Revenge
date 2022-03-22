#pragma once
#include <vector>
#include <string>

enum class ANCHOR_POINT : unsigned char;
class NonPlayer;

class FileReader
{
public: 

	std::vector<std::string> lines = std::vector<std::string>();

	virtual bool Open(const char* filepath);
	virtual void Close();

	std::vector<std::string> ParseLine(std::string line, char delim) const;
};

class RoomReader : public FileReader
{
public:

	// return the dimensions of the room in tiles
	Point<int> GetDimensions() const;
	// return the tile layers (in ints) of the room
	Point<int> GetLayers(int**** layers) const;
	// return the number of doors in the room
	int GetDoorCount() const;
	// return the data of each door in the room
	int GetDoorData(int*** doorData) const;
	// return the number of interactable tiles in the room
	int GetTextCount() const;
	// return the list of text for interactable tiles in the room
	int GetTextList(std::vector<std::vector<std::string>>& textList) const;
	// return the number of NPCs in a room
	int GetNPCCount() const;
	// return the NPC data of the room
	int GetNPCList(std::vector<NonPlayer*>& NPCList) const;

private:

	int GetDoorCountLine() const;
	int GetTextCountLine() const;
	int GetNPCCountLine() const;
};

class MenuReader : public FileReader
{
public:

	struct OptionData
	{
		std::string text = "";
		int returnValue = -1;
		Point<int> position = Point<int>(0,0), matrixLocation = Point<int>(0, 0);
	};

	// return the dimensions of the options within the menu
	Point<int> GetDimensions() const;
	// returns the options within the menu
	int GetOptions(MenuReader::OptionData** optionsList) const;
	// returns the anchor point of the menu
	ANCHOR_POINT GetAnchor() const;
};

class SoundReader : public FileReader
{

};