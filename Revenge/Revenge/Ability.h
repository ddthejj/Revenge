#pragma once

#include <functional>

class Character;

class Ability
{
	char* name;
	std::function<bool(Character** users, Character** targets)> effect;
	//int cost;
};

