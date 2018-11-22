#pragma once
#include "Sprite.h"
class Room;

class Character : public Sprite
{
protected:
	enum WayFacing { UP, DOWN, LEFT, RIGHT };

	char name[32];
	float mvmntSpeed = 3.f;
	//MyRectangle* sourceRectangle = new MyRectangle(0, 0, 32, 32);

	WayFacing wayFacing = DOWN;
	float animTimer = 0;

public:
	Character(float x, float y, float height, float width, Texture* _texture, float _layer);
	~Character();
	virtual void Draw(SpriteBatch* spriteBatch);
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