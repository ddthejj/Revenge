#pragma once
#include "Sprite.h"
class Room;
class Ability;

// The base class for any character
class Character : public Sprite
{
protected:

#pragma region Enums

	// Which way the character is facing
	enum class DIRECTION 
	{ UP, DOWN, LEFT, RIGHT, MAX };

	enum class MAGIC_TYPE
	{
		NONE = 0,

		ELEMENTAL = 1 << 5,
		HUMANITY = 1 << 6,

		FIRE = ELEMENTAL | (1 << 0),
		WATER = ELEMENTAL | (1 << 1),
		LIFE = ELEMENTAL | (1 << 2),
		EARTH = ELEMENTAL | (1 << 3),
		VENOM = ELEMENTAL | (1 << 4),

		MAGNET = HUMANITY | (1 << 0),
		WARD = HUMANITY | (1 << 1),
		LIGHT = HUMANITY | (1 << 2),
		MIND = HUMANITY | (1 << 3),
		NECRO = HUMANITY | (1 << 4),
	};

#pragma endregion

#pragma region Properties

	char firstname[32] = "DEFAULT";	// the name of the character
	char lastname[32] = "DEFAULT";	// the name of the character
	int ratio = 0;			// ratio of the character
	float mvmntSpeed = 3.f;		// how quickly the character moves across the map
	int attack = 0, defense = 0, mind = 0, spirit = 0, energy = 0;	// the characters stats

	bool moving[(int)DIRECTION::MAX];
	DIRECTION wayFacing = DIRECTION::DOWN;
	float animTimer = 0;
	MAGIC_TYPE magicType[2] = { MAGIC_TYPE::NONE, MAGIC_TYPE::NONE };
	
	std::vector<Ability*> abilities;

#pragma endregion

public:

#pragma region Methods

	Character(float x, float y, float height, float width, const Texture* _texture, float _layer);
	~Character();
	virtual void Draw(SpriteBatch* spriteBatch);

	// returns the first name of the character
	const char* FirstName() const  { return firstname; }	
	// returns the last name of the character
	const char* LastName() const  { return lastname; }		
		// returns the first name and last name of the character
	const char* FullName() const							
	{ return std::string(firstname).append(" ").append(lastname).c_str(); }			
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
	const MAGIC_TYPE* Magic() const { return magicType; }		
	// returns the first magic type of the character 
	MAGIC_TYPE MagicPrimary() const { return magicType[0]; }	
	// returns the second magic type of the character 
	MAGIC_TYPE MagicSecondary() const { return magicType[1]; }	

	// returns the abilities of the character
	const std::vector<Ability*> Abilities() const { return abilities; }	
	// returns the ability of the character at the selected index
	const Ability* Ability(int index) const { return abilities.size() > index ? abilities[index] : nullptr; }
														
#pragma endregion

};

// the base class for playable characters
class Player : public Character
{
public:

#pragma region Methods

	Player(float x, float y, float height, float width, const Texture* _texture, float _layer);
	~Player();

	// update the player
	void Update(float delta_time);

#pragma endregion

protected:

#pragma region Callback Methods

	// callbacks from input manager
	void UpPressedCallback();
	void UpReleasedCallback();
	void DownPressedCallback();
	void DownReleasedCallback();
	void LeftPressedCallback();
	void LeftReleasedCallback();
	void RightPressedCallback();
	void RightReleasedCallback();
	void OnInteractCallback();
	void OnMenuCallback();

	virtual void BindCallbacks();
	virtual void UnbindCallbacks();

	void ResetInputs();

#pragma endregion

#pragma region Methods

	// move the player based on input
	void Move();
	// check if you're going to collide into walls
	void TestCollision(bool* up, bool* down, bool* left, bool* right, const Room* currentRoom);
	// returns the interact point of the player 
	Point<float> GetInteractPoint() const;

#pragma endregion
};

// the base class for non player characters
class NonPlayer : public Character
{
public:

#pragma region Methods

	NonPlayer(float x, float y, float height, float width, const Texture* _texture, float _layer);
	~NonPlayer();

	// update the NPC
	void Update(float delta_time);

#pragma endregion
};