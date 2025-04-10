#include "defines.h"
#include "Weapon.h"
#include "Character.h"
#include "Ability.h"

WeaponData::WeaponData(std::string _name, int _level, int _at, int _mg, MAGIC_TYPE _magicType, std::vector<Ability*> _abilities) :
	name(_name), level(_level), at(_at), mg(_mg), magicType(_magicType), abilities(_abilities)
{
}
