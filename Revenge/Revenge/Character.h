#pragma once
#include "Sprite.h"
class Room;

class Character : public Sprite
{
protected:
	enum WayFacing { UP, DOWN, LEFT, RIGHT };

	char name[32] = "DEFAULT";
	float mvmntSpeed = 3.f;
	float attack = 0, defense = 0, mind = 0, spirit = 0, speed = 0, dexterity = 0, accuracy = 0;
	//MyRectangle* sourceRectangle = new MyRectangle(0, 0, 32, 32);

	WayFacing wayFacing = DOWN;
	float animTimer = 0;

public:
	Character(float x, float y, float height, float width, Texture* _texture, float _layer);
	~Character();
	virtual void Draw(SpriteBatch* spriteBatch);

	char* Name() { return name; }
	float AT() { return attack; }
	float DF() { return defense; }
	float MN() { return mind; }
	float SR() { return spirit; }
	float SP() { return speed; }
	float DX() { return dexterity; }
	float AC() { return accuracy; }
};

class Player : public Character
{
public:
	Player(float x, float y, float height, float width, Texture* _texture, float _layer);
	~Player();

	void Update();

protected:
	void Move();
	void TestCollision(bool* up, bool* down, bool* left, bool* right, Room* currentRoom);
};

class NonPlayer : public Character
{
public:
	NonPlayer(float x, float y, float height, float width, Texture* _texture, float _layer);
	~NonPlayer();

	void Update();
};