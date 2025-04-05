#include "defines.h"
#include "Character.h"
#include "SpriteBatch.h"
#include "Texture.h"
#include "Manager.h"
#include "OverworldManager.h"
#include "Room.h"
#include "Tile.h"
#include "InputManager.h"
#include "MenuManager.h"
#include "Ability.h"
#include "FileReader.h"
#include "Rectangle.h"
#include "PhysicsManager.h"
#include "CollisionComponent.h"

#include "InteractionComponent.h"

#include <random>
#include <ctime>
#include <cmath>

#pragma region Character

Character::Character(std::string _debugName, float x, float y, float height, float width, const Texture* _texture, float _layer) : Sprite(_debugName, x, y, height, width, 0, 0, 32, 32, _texture, _layer)
{
	AddComponent(new DynamicCollisionComponent(this, rectangle));
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
	spriteBatch->Draw(this);
}

void Character::Move(float delta_time)
{
	if (!IsMoving())
	{
		if (animTimer > 0.f)
		{
			// resets animation state
			AnimateMovement(delta_time);
		}
		return;
	}

	if (IsMoving())
	{
		collided = PhysicsManager::TryMove((CollisionComponent*)GetComponentOfType(COMPONENT_TYPE::COLLISION), velocity);
	}

	AnimateMovement(delta_time);
}

void Character::AnimateMovement(float delta_time)
{
	if (velocity.Magnitude() > 0.f)
	{
		DIRECTION prevWayFacing = wayFacing;

		if (std::abs(velocity.x) > std::abs(velocity.y))
		{
			if (velocity.x < 0)
			{
				wayFacing = DIRECTION::LEFT;
			}
			else
			{
				wayFacing = DIRECTION::RIGHT;
			}
		}
		else
		{
			if (velocity.y < 0)
			{
				wayFacing = DIRECTION::UP;
			}
			else
			{
				wayFacing = DIRECTION::DOWN;
			}
		}

		if (prevWayFacing != wayFacing)
		{
			// changed direction
			animTimer = 0.f;
		}

		sourceRectangle->SetLocation(Point<float>((int)(animTimer / (animTimerMax / 4.f)) * sourceRectangle->Width(), (int)wayFacing * sourceRectangle->Height()));

		animTimer += delta_time;

		if (animTimer >= animTimerMax)
		{
			animTimer = 0.f;
		}
	}
	else
	{
		// stopped
		animTimer = 0.f;
		sourceRectangle->SetLocation(Point<float>((int)(animTimer / (animTimerMax / 4.f)) * sourceRectangle->Width(), (int)wayFacing * sourceRectangle->Height()));
	}
}

#pragma endregion

#pragma region Player Character

Player::Player(std::string _debugName, float x, float y, float height, float width, const Texture* _texture, float _layer) : Character(_debugName, x, y, height, width, _texture, _layer)
{
}

Player::~Player()
{
	if (active) Deactivate();
}

void Player::Update(float delta_time)
{
	Move(delta_time);
}

void Player::Freeze() 
{
	Character::Freeze();
	// unbind menu callback to prevent us from opening the menu again when we press the same button to close the menu
	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_MENU, std::bind(&Player::OnMenuCallback, this), this);

}

void Player::Unfreeze()
{
	Character::Unfreeze();
	// rebing menu callback
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_MENU, std::bind(&Player::OnMenuCallback, this), this);
}

bool Player::UpPressedCallback()
{
	velocity += Point<float>(0.f, -mvmntSpeed);
	return false;
}

bool Player::DownPressedCallback()
{
	velocity += Point<float>(0.f, mvmntSpeed);
	return false;
}

bool Player::LeftPressedCallback()
{
	velocity += Point<float>(-mvmntSpeed, 0.f);
	return false;
}

bool Player::RightPressedCallback()
{
	velocity += Point<float>(mvmntSpeed, 0.f);
	return false;
}

bool Player::UpReleasedCallback()
{
	velocity -= Point<float>(0.f, -mvmntSpeed);
	return false;
}

bool Player::DownReleasedCallback()
{
	velocity -= Point<float>(0.f, mvmntSpeed);
	return false;
}

bool Player::LeftReleasedCallback()
{
	velocity -= Point<float>(-mvmntSpeed, 0.f);
	return false;
}

bool Player::RightReleasedCallback()
{
	velocity -= Point<float>(mvmntSpeed, 0.f);
	return false;
}

bool Player::OnInteractCallback()
{
	if (frozen || !active)
		return false;


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

	return OverworldManager::OnInteract(interactPoint);
}

bool Player::OnMenuCallback()
{
	MenuManager::OpenOverworldMenu();
	OverworldManager::FreezeScene();
	return true;
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

NonPlayer::NonPlayer(std::string _debugName, float x, float y, float height, float width, const Texture* _texture, float _layer) : Character(_debugName, x, y, height, width, _texture, _layer)
{
	startLocation = Point<float>(x, y);
	moveToLocation = startLocation;

	AddComponent(new DialogueComponent(this, rectangle, std::vector<std::string>{"hello!"}));
}

NonPlayer::~NonPlayer()
{
	if (active) Deactivate();
}

void NonPlayer::Update(float delta_time)
{
	Move(delta_time);
}

void NonPlayer::Move(float delta_time)
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
				float theta = ((float)std::rand() / RAND_MAX) * 2 * (float)std::_Pi_val;
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
	case (MOVE_MODE::FOLLOW):

		break;
	}


	if (moveToLocation != GetPos())
	{
		velocity = moveToLocation - GetPos();
		velocity.Normalize();
		velocity *= mvmntSpeed;
	}

	Point<float> prevPos = GetPos();

	Character::Move(delta_time);

	// if we collide
	if (collided)
	{
		moveToLocation = GetPos();
	}
}
#pragma endregion