#pragma once
#include "Sprite.h"
#include <vector>
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

#pragma endregion

#pragma region Properties

	std::string firstname = "DEFAULT";	// the name of the character
	std::string lastname = "DEFAULT";	// the name of the character
	int ratio = 0;						// ratio of the character
	float mvmntSpeed = 2.f;				// how quickly the character moves across the map
	int startingLevel = 1;				// level the character starts at
	int currentLevel = 1;				// level the character is at the moment
	int attack = 0, defense = 0, mind = 0, spirit = 0, energy = 0;		// the character's base stats
	MAGIC_TYPE magicType[2] = { MAGIC_TYPE::NONE, MAGIC_TYPE::NONE };	// the character's magic types
	std::vector<Ability*> abilities;									// the character's abilities

	Point<float> velocity = { 0.f, 0.f };	// character overworld movement velocity
	DIRECTION wayFacing = DIRECTION::DOWN;	// way the character is looking for animation
	float animTimer = 0;					// timer for overworld animation

#pragma endregion

public:

#pragma region Methods

protected:
	Character(float x, float y, float height, float width, const Texture* _texture, float _layer);
public:
	Character(float x, float y, float height, float width, const Texture* _texture, float _layer, const char* filepath);
	~Character();
	virtual void Draw(SpriteBatch* spriteBatch);

	// read character data from a file
	virtual void ReadData(const char* filepath);
	// returns the first name of the character
	std::string FirstName() const { return firstname; }
	// returns the last name of the character
	std::string LastName() const  { return lastname; }		
	// returns the first name and last name of the character
	std::string FullName() const							
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


	// move the character based on velocity
	void Move();
	// check if current velocity will take you inside collision
	void TestCollision();
	// animate the character based on movement
	void AnimateMovement();
	// check if character is moving
	bool IsMoving() { return velocity.Magnitude() != 0.f; }
														
#pragma endregion

};

// the base class for playable characters
class Player : public Character
{
public:

#pragma region Methods

	Player(float x, float y, float height, float width, const Texture* _texture, float _layer, const char* filepath);
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

	// returns the interact point of the player 
	Point<float> GetInteractPoint() const;

#pragma endregion
};

// the base class for non player characters
class NonPlayer : public Character
{
public:

	MOVE_MODE movementMode = MOVE_MODE::NONE;
	float moveRadius = 0.f;
	float moveTimer = 0.f, moveDelay = 5000.f;
	Point<float> startLocation;
	Point<float> moveToLocation;

#pragma region Methods

	NonPlayer(float x, float y, float height, float width, const Texture* _texture, float _layer, const char* filepath);
	~NonPlayer();

	// read character data from file
	virtual void ReadData(const char* filepath) override;
	// update the NPC
	void Update(float delta_time);

#pragma endregion
};