#pragma once
#include "Sprite.h"

class Room;
class Ability;
class WeaponData;

enum class MAGIC_TYPE : unsigned int
{
	NONE = 0,

	ELEMENTAL = 1 << 0,
	HUMANITY = 1 << 1,

	FIRE = ELEMENTAL | (1 << 2),
	WATER = ELEMENTAL | (1 << 3),
	LIFE = ELEMENTAL | (1 << 4),
	EARTH = ELEMENTAL | (1 << 5),
	VENOM = ELEMENTAL | (1 << 6),

	MAGNET = HUMANITY | (1 << 7),
	WARD = HUMANITY | (1 << 8),
	LIGHT = HUMANITY | (1 << 9),
	MIND = HUMANITY | (1 << 10),
	DEATH = HUMANITY | (1 << 11),

	ALL = FIRE | WATER | LIFE | EARTH | VENOM | MAGNET | WARD | LIGHT | MIND | DEATH
};

class CharacterData
{
private:

	std::string firstName = "DEFAULT";	// the name of the character
	std::string lastName = "DEFAULT";	// the name of the character
	std::string nickname = "";			// possible character nickname
	int ratio = 0;						// ratio of the character
	int level = 0, hp = 0, attack = 0, defense = 0, mind = 0, spirit = 0, energy = 0;		// the character's base stats
	MAGIC_TYPE magicType = MAGIC_TYPE::NONE;						// the character's magic type
	WeaponData* weapon;					// the character's weapon's data
	std::vector<Ability*> abilities;	// the character's abilities

	bool reverseName = false;			// used for east asian style names (family name before given name)

public:

	CharacterData(std::string _firstname, std::string _lastname, int _ratio, int _level, int _hp, int _attack, int _defense, int _mind, int _spirit, int _energy, MAGIC_TYPE _magicType, WeaponData* _weapon, std::vector<Ability*> _abilities, std::string _nickname = "", bool _reverseName = false) :
		firstName(_firstname), lastName(_lastname),
		level(_level), hp(_hp), attack(_attack), defense(_defense), mind(_mind), spirit(_spirit), energy(_energy),
		magicType(_magicType), weapon(_weapon), abilities(_abilities),
		nickname(_nickname), reverseName(_reverseName)
	{ }

	// returns the first name of the character
	std::string GivenName(bool useNickname = true) const;
	// returns the last name of the character
	std::string FamilyName() const;
	// returns the first name and last name of the character
	std::string FullName(bool useNickname) const;
	// returns the chaos ratio of the character
	int RatioChaos() const { return ratio; }
	// returns the harmony ration of the charater
	int RatioHarmony() const { return 100 - ratio; }
	// returns the attack of the character
	int AT() const { return attack; }
	// returns the defense of the character
	int DF() const { return defense; }
	// returns the mind of the character
	int MN() const { return mind; }
	// returns the spirit of the character
	int SR() const { return spirit; }
	// returns the energy of the character
	int EN() const { return energy; }

	// returns whether or not the charactre is chaotic
	bool IsChaotic() const { return ratio > 50; }
	// returns whether or not the character is harmonic
	bool IsHarmonic() const { return ratio < 50; }

	// returns the magic type of the character 
	const MAGIC_TYPE Magic() const { return magicType; }
	// returns the abilities of the character
	const std::vector<Ability*> Abilities() const { return abilities; }
	// returns the ability of the character at the selected index
	const Ability* GetAbility(int index) const { return abilities.size() > index ? abilities[index] : nullptr; }
	// returns the weapon the character weilds
	const WeaponData* GetWeapon() { return weapon; }
};

// The base class for any character
class Character : public Sprite
{
protected:

#pragma region Properties

	CharacterData* characterData = nullptr;

#pragma endregion

public:

#pragma region Methods

	Character(std::string _debugName, float x, float y, float height, float width, const Texture* _texture, float _layer, CharacterData* _characterData);
	~Character();
	virtual void Draw(SpriteBatch* spriteBatch);
	const CharacterData* GetCharacterData() { return characterData; }
														
#pragma endregion

};

// the base class for playable characters
class Player : public Character
{
public:

#pragma region Methods

	Player(std::string _debugName, float x, float y, float height, float width, const Texture* _texture, float _layer, CharacterData* _characterData);
	~Player();

	// update the player
	void Update(float delta_time);
	// freeze the object
	virtual void Freeze();
	// unfreeze the object
	virtual void Unfreeze();

#pragma endregion

protected:

#pragma region Callback Methods

	// callbacks from input manager
	bool OnInteractCallback();
	bool OnMenuCallback();

	virtual void BindCallbacks();
	virtual void UnbindCallbacks();

	void ResetInputs();

#pragma endregion

#pragma region Methods

	// returns the interact point of the player 
	Point<float> GetInteractPoint() const;

#pragma endregion
};

// the base class for non player characters
class NonPlayer : public Character
{
public:

#pragma region Methods

	NonPlayer(std::string _debugName, float x, float y, float height, float width, const Texture* _texture, float _layer, CharacterData* _characterData, float _radius = 0.f);
	~NonPlayer();

	// update the NPC
	void Update(float delta_time);
#pragma endregion
};