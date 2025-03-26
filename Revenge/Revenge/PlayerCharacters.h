#pragma once
#include "Character.h"

class PC_Archer : public Player
{
	using Player::Player;

protected:

	virtual void CreateCharacterData() override;
};