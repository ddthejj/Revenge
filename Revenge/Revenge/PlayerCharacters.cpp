#include "defines.h"
#include "PlayerCharacters.h"

void PC_Archer::CreateCharacterData()
{
	firstname = "Archer";
	lastname = "Alwestow";
	ratio = 67;
	startingLevel = 1;
	std::vector<int> stats = { 1,1,1,1,1 };
	attack = stats[0];
	defense = stats[1];
	mind = stats[2];
	spirit = stats[3];
	energy = stats[4];
	magicType[0] = MAGIC_TYPE::FIRE;
	magicType[1] = MAGIC_TYPE::MAGNET;
	abilities = {};
}
