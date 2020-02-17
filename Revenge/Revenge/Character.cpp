#include "defines.h"
#include "Character.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "Manager.h"
#include "OverworldManager.h"
#include "Room.h"
#include "Tile.h"
#include "SpriteBatch.h"

#pragma region Character
Character::Character(float x, float y, float height, float width, Texture* _texture, float _layer) : Sprite(x, y, height, width, 0, 0, 32, 32, _texture, _layer)
{

}

Character::~Character()
{

}

void Character::Draw(SpriteBatch* spriteBatch)
{
	spriteBatch->Draw(texture, rectangle, sourceRectangle, 1.0f, layer);
}
#pragma endregion

#pragma region Player Character
Player::Player(float x, float y, float height, float width, Texture* _texture, float _layer) : Character(x, y, height, width, _texture, _layer)
{

}

Player::~Player()
{

}

void Player::Update()
{
	Move();
}

void Player::Move()
{
	Room* currentRoom = OverworldManager::GetCurrentRoom();

	if (!currentRoom)
		return;

	WayFacing preciousWayFacing = wayFacing;

	bool up = false, down = false, left = false, right = false;


	if (Manager::IsKeyDown(KEY_UP))
	{
		up = true;
	}
	if (Manager::IsKeyDown(KEY_DOWN))
	{
		if (!up)
			down = true;
		else
			up = false;
	}
	if (Manager::IsKeyDown(KEY_LEFT))
	{
		left = true;
	}
	if (Manager::IsKeyDown(KEY_RIGHT))
	{
		if (!left)
			right = true;
		else
			left = false;
	}

	TestCollision(&up, &down, &left, &right, currentRoom);

	bool moving = false;
	if (up || down || left || right)
	{
		animTimer += .05f;
		moving = true;
	}
	else
		animTimer = 0.f;

	if (wayFacing != preciousWayFacing || (moving && animTimer < 1.f))
	{
		animTimer = 1.f;
	}

	if (animTimer >= 4.f)
		animTimer = 1.f;


	sourceRectangle->SetX((int)animTimer * TILE_WIDTH);
	sourceRectangle->SetY((int)wayFacing * TILE_HEIGHT);
}

void Player::TestCollision(bool* up, bool* down, bool* left, bool* right, Room* currentRoom)
{
	int tilesRight = (int)(rectangle->Width() / TILE_HEIGHT) + 1;
	int tilesDown = (int)(rectangle->Height() / TILE_HEIGHT) + 1;

#pragma region TEST TOP
	bool collided = false;;

	if (*up)
	{
		MyRectangle futureRectangle(*rectangle);
		futureRectangle.MoveY(-mvmntSpeed);

		int atX = (int)(futureRectangle.X() / TILE_WIDTH);
		int atY = (int)(futureRectangle.Y() / TILE_HEIGHT);

		for (int j = atY; j < atY + tilesDown; j++)
		{
			for (int i = atX; i < atX + tilesRight; i++)
			{
				Tile* testing = currentRoom->GetTile(1, i, j);
				if (!testing)
					continue;

				if (testing->Collidable())
					if (testing->GetRectangle()->Intersects(futureRectangle))
					{
						collided = true;
						rectangle->SetY(testing->GetRectangle()->Bottom() + 1);
						goto COLLIDEDUP;
					}
			}
		}
	COLLIDEDUP:

		if (!collided)
		{
			rectangle->MoveY(-mvmntSpeed);
			//if (!*right && !*left)
			wayFacing = UP;
		}
		else
		{
			*up = false;
		}

	}
#pragma endregion

#pragma region TEST DOWN
	collided = false;

	if (*down)
	{
		MyRectangle futureRectangle(*rectangle);
		futureRectangle.MoveY(mvmntSpeed);

		int atX = (int)(futureRectangle.X() / TILE_WIDTH);
		int atY = (int)(futureRectangle.Y() / TILE_HEIGHT);

		for (int j = atY; j < atY + tilesDown; j++)
		{
			for (int i = atX; i < atX + tilesRight; i++)
			{
				Tile* testing = currentRoom->GetTile(1, i, j);
				if (!testing)
					continue;

				if (testing->Collidable())
					if (testing->GetRectangle()->Intersects(futureRectangle))
					{
						collided = true;
						rectangle->SetY(testing->GetRectangle()->Top() - 1 - TILE_HEIGHT);
						goto COLLIDEDDOWN;
					}
			}
		}
	COLLIDEDDOWN:

		if (!collided)
		{
			rectangle->MoveY(mvmntSpeed);
			//if (!*right && !*left)
			wayFacing = DOWN;
		}
		else
		{
			*down = false;
		}
	}
#pragma endregion

#pragma region TEST LEFT
	collided = false;

	if (*left)
	{
		MyRectangle futureRectangle(*rectangle);
		futureRectangle.MoveX(-mvmntSpeed);

		int atX = (int)(futureRectangle.X() / TILE_WIDTH);
		int atY = (int)(futureRectangle.Y() / TILE_HEIGHT);

		for (int j = atY; j < atY + tilesDown; j++)
		{
			for (int i = atX; i < atX + tilesRight; i++)
			{
				Tile* testing = currentRoom->GetTile(1, i, j);
				if (!testing)
					continue;

				if (testing->Collidable())
					if (testing->GetRectangle()->Intersects(futureRectangle))
					{
						collided = true;
						rectangle->SetX(testing->GetRectangle()->Right() + 1);
						goto COLLIDELEFT;
					}
			}
		}
	COLLIDELEFT:

		if (!collided)
		{
			rectangle->MoveX(-mvmntSpeed);
			//if (!*up && !*down)
			wayFacing = LEFT;
		}
		else
		{
			*left = false;
		}
	}
#pragma endregion

#pragma region TEST RIGHT
	collided = false;

	if (*right)
	{
		MyRectangle futureRectangle(*rectangle);
		futureRectangle.MoveX(mvmntSpeed);

		int atX = (int)(futureRectangle.X() / TILE_WIDTH);
		int atY = (int)(futureRectangle.Y() / TILE_HEIGHT);

		for (int j = atY; j < atY + tilesDown; j++)
		{
			for (int i = atX; i < atX + tilesRight; i++)
			{
				Tile* testing = currentRoom->GetTile(1, i, j);
				if (!testing)
					continue;

				if (testing->Collidable())
					if (testing->GetRectangle()->Intersects(futureRectangle))
					{
						collided = true;
						rectangle->SetX(testing->GetRectangle()->Left() - 1 - TILE_WIDTH);
						goto COLLIDERIGHT;
					}
			}
		}
	COLLIDERIGHT:

		if (!collided)
		{
			rectangle->MoveX(mvmntSpeed);
			//if (!*up && !*down)
			wayFacing = RIGHT;
		}
		else
		{
			*right = false;
		}
	}
#pragma endregion
}
#pragma endregion

#pragma region NonPlayer Character
NonPlayer::NonPlayer(float x, float y, float height, float width, Texture* _texture, float _layer) : Character(x, y, height, width, _texture, _layer)
{

}

NonPlayer::~NonPlayer()
{

}

void NonPlayer::Update()
{

}
#pragma endregion