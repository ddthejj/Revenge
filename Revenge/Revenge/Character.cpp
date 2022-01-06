#include "defines.h"
#include "Character.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "Manager.h"
#include "OverworldManager.h"
#include "Room.h"
#include "Tile.h"
#include "SpriteBatch.h"
#include "InputManager.h"
#include "MenuManager.h"

#pragma region Character
Character::Character(float x, float y, float height, float width, Texture* _texture, float _layer) : Sprite(x, y, height, width, 0, 0, 32, 32, _texture, _layer)
{
	for (int i = 0; i < (int)DIRECTION::MAX; i++)
	{
		moving[i] = false;
	}
}

Character::~Character()
{
	if (active) Deactivate();
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
	if (active) Deactivate();
}

void Player::Update(float delta_time)
{
	Move();
}

void Player::UpPressedCallback()
{
	if (!moving[(int)DIRECTION::DOWN])
		moving[(int)DIRECTION::UP] = true;
	else
		moving[(int)DIRECTION::DOWN] = false;
}

void Player::DownPressedCallback()
{
	if (!moving[(int)DIRECTION::UP])
		moving[(int)DIRECTION::DOWN] = true;
	else
		moving[(int)DIRECTION::UP] = false;
}

void Player::LeftPressedCallback()
{
	if (!moving[(int)DIRECTION::RIGHT])
		moving[(int)DIRECTION::LEFT] = true;
	else
		moving[(int)DIRECTION::RIGHT] = false;
}

void Player::RightPressedCallback()
{
	if (!moving[(int)DIRECTION::LEFT])
		moving[(int)DIRECTION::RIGHT] = true;
	else
		moving[(int)DIRECTION::LEFT] = false;
}

void Player::UpReleasedCallback()
{
	if (!moving[(int)DIRECTION::UP])
		moving[(int)DIRECTION::DOWN] = true;
	else
		moving[(int)DIRECTION::UP] = false;
}

void Player::DownReleasedCallback()
{
	if (!moving[(int)DIRECTION::DOWN])
		moving[(int)DIRECTION::UP] = true;
	else
		moving[(int)DIRECTION::DOWN] = false;
}

void Player::LeftReleasedCallback()
{
	if (!moving[(int)DIRECTION::LEFT])
		moving[(int)DIRECTION::RIGHT] = true;
	else
		moving[(int)DIRECTION::LEFT] = false;
}

void Player::RightReleasedCallback()
{
	if (!moving[(int)DIRECTION::RIGHT])
		moving[(int)DIRECTION::LEFT] = true;
	else
		moving[(int)DIRECTION::RIGHT] = false;
}

void Player::OnInteractCallback()
{
	if (frozen || !active)
		return;


	Point<float> interactPoint;

	switch (wayFacing)
	{
	case DIRECTION::UP:
		interactPoint = Point<float>(rectangle->CenterX(), rectangle->Top() - (rectangle->Height() / 3.f));
		break;
	case DIRECTION::DOWN:
		interactPoint = Point<float>(rectangle->CenterX(), rectangle->Bottom() + (rectangle->Height() / 3.f));
		break;
	case DIRECTION::LEFT:
		interactPoint = Point<float>(rectangle->Left() - (rectangle->Width() / 3.f), rectangle->CenterY());
		break;
	case DIRECTION::RIGHT:
		interactPoint = Point<float>(rectangle->Right() + (rectangle->Width() / 3.f), rectangle->CenterY());
		break;
	}

	OverworldManager::OnInteract(interactPoint);
}

void Player::OnMenuCallback()
{
	MenuManager::OpenMenu((int)OVERWORLD_MENUS::MENU_BASE);
	Manager::FreezeScene();
}

void Player::BindCallbacks()
{
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_UP, std::bind(&Player::UpPressedCallback, this), this);
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_DOWN, std::bind(&Player::DownPressedCallback, this), this);
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_LEFT, std::bind(&Player::LeftPressedCallback, this), this);
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_RIGHT, std::bind(&Player::RightPressedCallback, this), this);

	InputManager::KeyReleasedCallbacks_Attach(KEYS::KEY_UP, std::bind(&Player::UpReleasedCallback, this), this);
	InputManager::KeyReleasedCallbacks_Attach(KEYS::KEY_DOWN, std::bind(&Player::DownReleasedCallback, this), this);
	InputManager::KeyReleasedCallbacks_Attach(KEYS::KEY_LEFT, std::bind(&Player::LeftReleasedCallback, this), this);
	InputManager::KeyReleasedCallbacks_Attach(KEYS::KEY_RIGHT, std::bind(&Player::RightReleasedCallback, this), this);

	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_INTERACT, std::bind(&Player::OnInteractCallback, this), this);
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_MENU, std::bind(&Player::OnMenuCallback, this), this);
	ResetInputs();
}

void Player::UnbindCallbacks()
{
	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_UP, std::bind(&Player::UpPressedCallback, this), this);
	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_DOWN, std::bind(&Player::DownPressedCallback, this), this);
	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_LEFT, std::bind(&Player::LeftPressedCallback, this), this);
	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_RIGHT, std::bind(&Player::RightPressedCallback, this), this);

	InputManager::KeyReleasedCallbacks_Remove(KEYS::KEY_UP, std::bind(&Player::UpReleasedCallback, this), this);
	InputManager::KeyReleasedCallbacks_Remove(KEYS::KEY_DOWN, std::bind(&Player::DownReleasedCallback, this), this);
	InputManager::KeyReleasedCallbacks_Remove(KEYS::KEY_LEFT, std::bind(&Player::LeftReleasedCallback, this), this);
	InputManager::KeyReleasedCallbacks_Remove(KEYS::KEY_RIGHT, std::bind(&Player::RightReleasedCallback, this), this);

	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_INTERACT, std::bind(&Player::OnInteractCallback, this), this);
	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_MENU, std::bind(&Player::OnMenuCallback, this), this);

	ResetInputs();
}

void Player::ResetInputs()
{
	if (frozen || !active)
	{
		for (int i = 0; i < (int)DIRECTION::MAX; i++)
		{
			moving[i] = false;
		}
	}
	else
	{
		moving[(int)DIRECTION::UP] = InputManager::IsKeyDown(KEYS::KEY_UP);
		moving[(int)DIRECTION::DOWN] = InputManager::IsKeyDown(KEYS::KEY_DOWN);
		moving[(int)DIRECTION::LEFT] = InputManager::IsKeyDown(KEYS::KEY_LEFT);
		moving[(int)DIRECTION::RIGHT] = InputManager::IsKeyDown(KEYS::KEY_RIGHT);
	}
}


void Player::Move()
{
	Room* currentRoom = OverworldManager::GetCurrentRoom();

	if (!currentRoom)
		return;

	DIRECTION preciousWayFacing = wayFacing;

	if (!moving[(int)DIRECTION::UP] && !moving[(int)DIRECTION::DOWN] && !moving[(int)DIRECTION::LEFT] && !moving[(int)DIRECTION::RIGHT])
	{
		int i = 0;
	}

	bool up = moving[(int)DIRECTION::UP], down = moving[(int)DIRECTION::DOWN], left = moving[(int)DIRECTION::LEFT], right = moving[(int)DIRECTION::RIGHT];
	TestCollision(&up, &down, &left, &right, currentRoom);

	bool ismoving = false;
	if (up || down || left || right)
	{
		animTimer += .05f;
		ismoving = true;
	}
	else
		animTimer = 0.f;

	if (/*wayFacing != preciousWayFacing || (*/ismoving && animTimer < 1.f)/*)*/
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
			wayFacing = DIRECTION::UP;
		}
		else
		{
			if (wayFacing != DIRECTION::UP)
				wayFacing = DIRECTION::UP;
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
			wayFacing = DIRECTION::DOWN;
		}
		else
		{
			if (wayFacing != DIRECTION::DOWN)
				wayFacing = DIRECTION::DOWN;
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
			wayFacing = DIRECTION::LEFT;
		}
		else
		{
			if (wayFacing != DIRECTION::LEFT)
				wayFacing = DIRECTION::LEFT;
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
			wayFacing = DIRECTION::RIGHT;
		}
		else
		{
			if (wayFacing != DIRECTION::RIGHT)
				wayFacing = DIRECTION::RIGHT;
			*right = false;
		}
	}
#pragma endregion
}

Point<float> Player::GetInteractPoint()
{
	return Point<float>();
}

#pragma endregion

#pragma region NonPlayer Character
NonPlayer::NonPlayer(float x, float y, float height, float width, Texture* _texture, float _layer) : Character(x, y, height, width, _texture, _layer)
{

}

NonPlayer::~NonPlayer()
{
	if (active) Deactivate();
}

void NonPlayer::Update(float delta_time)
{

}
#pragma endregion