#pragma once
#include <functional>
#include <string>

class Character;

class Ability
{
	std::string name;
	std::function<bool(Character** users, Character** targets)> effect;
	int power;
	int mpower;
	int cost;

public: 

	Ability(std::string _name, int _power, int _mpower, int _cost)
	{
		name = _name; power = _power; mpower = _mpower; cost = _cost;
	}
};

