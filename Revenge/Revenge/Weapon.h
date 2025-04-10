#pragma once

#include <string>

enum class MAGIC_TYPE : unsigned int;
class Ability;

class WeaponData
{
	std::string name = "DEFAULT";		// the name of the weapon
	int level = 0, at = 0, mg = 0;		// the weapon's base stats
	MAGIC_TYPE magicType;				// the weapon's magic type
	std::vector<Ability*> abilities;

public:

	WeaponData(std::string _name, int _level, int _at, int _mg, MAGIC_TYPE _magicType, std::vector<Ability*> _abilities);

	// returns the name of the weapon
	std::string tName() const { return name; }
	// returns the level of the weapon
	int Level() const { return level; }
	// returns the attack of the weapon
	int AT() const { return at; }
	// returns the magic power of the weapon
	int MG() const { return mg; }
	// returns the magic type of the weapon 
	const MAGIC_TYPE Magic() const { return magicType; }
	// returns the abilities of the weapon
	const std::vector<Ability*> Abilities() const { return abilities; }
	// returns the ability of the weapon at the selected index
	const Ability* GetAbility(int index) const { return abilities.size() > index ? abilities[index] : nullptr; }
};

class Weapon
{
	WeaponData* weaponData;

public:

	Weapon(WeaponData* _weaponData) { weaponData = _weaponData; }
};