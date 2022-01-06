#pragma once
#include "Sprite.h"
class Room;

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

	char name[32] = "DEFAULT";	// the name of the character
	float mvmntSpeed = 3.f;		// how quickly the character moves across the map
	float attack = 0, defense = 0, mind = 0, spirit = 0, speed = 0, dexterity = 0, accuracy = 0;	// the characters stats
	//MyRectangle* sourceRectangle = new MyRectangle(0, 0, 32, 32);

	bool moving[(int)DIRECTION::MAX];
	DIRECTION wayFacing = DIRECTION::DOWN;
	float animTimer = 0;

#pragma endregion

public:

#pragma region Methods

	Character(float x, float y, float height, float width, Texture* _texture, float _layer);
	~Character();
	virtual void Draw(SpriteBatch* spriteBatch);

	char* Name() { return name; }		// returns the name of the character
	float AT() { return attack; }		// returns the attack of the character
	float DF() { return defense; }		// returns the defense of the character
	float MN() { return mind; }			// returns the mind of the character
	float SR() { return spirit; }		// returns the spirit of the character
	float SP() { return speed; }		// returns the speed of the character
	float DX() { return dexterity; }	// returns the dexterity of the character
	float AC() { return accuracy; }		// returns the accuracy of the character

#pragma endregion

};

// the base class for playable characters
class Player : public Character
{
public:

#pragma region Methods

	Player(float x, float y, float height, float width, Texture* _texture, float _layer);
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
	void TestCollision(bool* up, bool* down, bool* left, bool* right, Room* currentRoom);
	// returns the interact point of the player 
	Point<float> GetInteractPoint();

#pragma endregion
};

// the base class for non player characters
class NonPlayer : public Character
{
public:

#pragma region Methods

	NonPlayer(float x, float y, float height, float width, Texture* _texture, float _layer);
	~NonPlayer();

	// update the NPC
	void Update(float delta_time);

#pragma endregion
};