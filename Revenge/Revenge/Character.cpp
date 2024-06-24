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
#include "Ability.h"
#include "FileReader.h"

#include <random>
#include <ctime>
#include <cmath>

#pragma region Character
Character::Character(std::string _debugName, float x, float y, float height, float width, const Texture* _texture, float _layer) : Sprite(_debugName, x, y, height, width, 0, 0, 32, 32, _texture, _layer)
{
}
Character::Character(std::string _debugName, float x, float y, float height, float width, const Texture* _texture, float _layer, const char* filepath) : Sprite(_debugName, x, y, height, width, 0, 0, 32, 32, _texture, _layer)
{
	ReadData(filepath);
}

Character::~Character()
{
	if (active) Deactivate();

	for (int i = 0; i < abilities.size(); i++)
	{
		SafeDelete(abilities[i]);
	}
}

void Character::Draw(SpriteBatch* spriteBatch)
{
	spriteBatch->Draw(texture, rectangle, sourceRectangle, 1.0f, layer);
}

void Character::ReadData(const char* filepath)
{
	CharacterReader characterReader;
	characterReader.Open(filepath);

	firstname = characterReader.GetFirstName();
	lastname = characterReader.GetLastName();
	ratio = characterReader.GetRatio();
	startingLevel = characterReader.GetStartingLevel();
	std::vector<int> stats = characterReader.GetStats();
	attack = stats[0];
	defense = stats[1];
	mind = stats[2];
	spirit = stats[3];
	energy = stats[4];
	magicType[0] = characterReader.GetPrimaryMagic();
	magicType[1] = characterReader.GetSecondaryMagic();
	abilities = characterReader.GetAbilities();
}

void Character::Move()
{
	if (!IsMoving())
		return;

	TestCollision();

	AnimateMovement();
}

void Character::TestCollision()
{
	const Room* currentRoom = OverworldManager::GetCurrentRoom();

	if (!currentRoom)
		return;

	// horizontal movement first
	MyRectangle futureRectangle(*rectangle);
	futureRectangle.SetLocation(Point<float>(futureRectangle.Location().x + velocity.x, futureRectangle.Location().y));

	std::vector<Tile*> collidedTiles = currentRoom->TestCollision(futureRectangle);

	for (int i = 0; i < collidedTiles.size(); i++)
	{
		Tile* collidedTile = collidedTiles[i];

		if (velocity.x > 0) // moving right
		{
			if (futureRectangle.Right() - collidedTile->GetRectangle()->Left() <= velocity.x)
			{
				futureRectangle.SetX(collidedTile->GetRectangle()->Left() - 1 - futureRectangle.Width());
			}
		}
		else if (velocity.x < 0) // moving left
		{
			if (futureRectangle.Left() - collidedTile->GetRectangle()->Right() >= velocity.x)
			{
				futureRectangle.SetX(collidedTile->GetRectangle()->Right() + 1);
			}
		}
	}

	rectangle->SetLocation(futureRectangle.Location());

	// vertical movement second
	futureRectangle.SetLocation(Point<float>(futureRectangle.Location().x, futureRectangle.Location().y + velocity.y));
	collidedTiles = currentRoom->TestCollision(futureRectangle);

	for (int i = 0; i < collidedTiles.size(); i++)
	{
		Tile* collidedTile = collidedTiles[i];

		if (velocity.y < 0) // moving up
		{
			if (futureRectangle.Top() - collidedTile->GetRectangle()->Bottom() >= velocity.y)
			{
				futureRectangle.SetY(collidedTile->GetRectangle()->Bottom() + 1);
			}
		}
		else if (velocity.y > 0) // moving down
		{
			if (futureRectangle.Bottom() - collidedTile->GetRectangle()->Top() <= velocity.y)
			{
				futureRectangle.SetY(collidedTile->GetRectangle()->Top() - 1 - futureRectangle.Height());
			}
		}
	}

	rectangle->SetLocation(futureRectangle.Location());
}

void Character::AnimateMovement()
{

}

#pragma endregion

#pragma region Player Character

Player::Player(std::string _debugName, float x, float y, float height, float width, const Texture* _texture, float _layer, const char* filepath) : Character(_debugName, x, y, height, width, _texture, _layer, filepath)
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
	velocity += Point<float>(0.f, -mvmntSpeed);
}

void Player::DownPressedCallback()
{
	velocity += Point<float>(0.f, mvmntSpeed);
}

void Player::LeftPressedCallback()
{
	velocity += Point<float>(-mvmntSpeed, 0.f);
}

void Player::RightPressedCallback()
{
	velocity += Point<float>(mvmntSpeed, 0.f);
}

void Player::UpReleasedCallback()
{
	velocity -= Point<float>(0.f, -mvmntSpeed);
}

void Player::DownReleasedCallback()
{
	velocity -= Point<float>(0.f, mvmntSpeed);
}

void Player::LeftReleasedCallback()
{
	velocity -= Point<float>(-mvmntSpeed, 0.f);
}

void Player::RightReleasedCallback()
{
	velocity -= Point<float>(mvmntSpeed, 0.f);
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
	/*
	if (frozen || !active)
	{
		velocity = Point<float>(0.f, 0.f);
	}
	else
	{
		moving[(int)DIRECTION::UP] = InputManager::IsKeyDown(KEYS::KEY_UP);
		moving[(int)DIRECTION::DOWN] = InputManager::IsKeyDown(KEYS::KEY_DOWN);
		moving[(int)DIRECTION::LEFT] = InputManager::IsKeyDown(KEYS::KEY_LEFT);
		moving[(int)DIRECTION::RIGHT] = InputManager::IsKeyDown(KEYS::KEY_RIGHT);
	}
	*/
}

Point<float> Player::GetInteractPoint() const
{
	return Point<float>();
}

#pragma endregion

#pragma region NonPlayer Character

NonPlayer::NonPlayer(std::string _debugName, float x, float y, float height, float width, const Texture* _texture, float _layer, const char* filepath) : Character(_debugName, x, y, height, width, _texture, _layer)
{
	ReadData(filepath);

	startLocation = Point<float>(x, y);
	moveToLocation = startLocation;
}

NonPlayer::~NonPlayer()
{
	if (active) Deactivate();
}

void NonPlayer::ReadData(const char* filepath)
{
	NPCReader npcReader;
	npcReader.Open(filepath);

	firstname = npcReader.GetFirstName();
	lastname = npcReader.GetLastName();
	ratio = npcReader.GetRatio();
	startingLevel = npcReader.GetStartingLevel();
	std::vector<int> stats = npcReader.GetStats();
	attack = stats[0];
	defense = stats[1];
	mind = stats[2];
	spirit = stats[3];
	energy = stats[4];
	magicType[0] = npcReader.GetPrimaryMagic();
	magicType[1] = npcReader.GetSecondaryMagic();
	abilities = npcReader.GetAbilities();
	movementMode = npcReader.GetMovementMode();
	moveRadius = npcReader.GetMovementRadius();
}

void NonPlayer::Update(float delta_time)
{
	switch (movementMode)
	{
	case (MOVE_MODE::NONE):

		break;
	case (MOVE_MODE::RANDOM_RADIUS):

		moveTimer += delta_time;
		if (!IsMoving())
		{
			if (moveTimer >= moveDelay)
			{
				moveTimer = 0;

				//random point within radius

				float r = moveRadius * std::sqrt((float)std::rand() / RAND_MAX);
				float theta = (float)(std::rand() / RAND_MAX) * 2 * (float)std::_Pi;
				float x = startLocation.x + r * std::cos(theta);
				float y = startLocation.y + r * std::sin(theta);

				moveToLocation = Point<float>(x, y);
			}
		}

		if (IsMoving())
		{
			if (moveToLocation.Equals(GetPos(), mvmntSpeed))
			{
				SetPos(moveToLocation);

				velocity = Point<float>(0.f, 0.f);
			}
		}

		break;
	case (MOVE_MODE::ATTACK):

		break;
	}


	if (moveToLocation != GetPos())
	{
		if (moveToLocation.x > GetPos().x)
		{
			velocity.x = mvmntSpeed;
		}
		else if (moveToLocation.x < GetPos().x)
		{
			velocity.x = -mvmntSpeed;
		}
		else
		{
			velocity.x = 0;
		}

		if (moveToLocation.y > GetPos().y)
		{
			velocity.y = mvmntSpeed;
		}
		else if (moveToLocation.y < GetPos().y)
		{
			velocity.y = -mvmntSpeed;
		}
		else
		{
			velocity.y = 0;
		}
	}

	Move();
}

#pragma endregion