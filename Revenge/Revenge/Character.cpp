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
#include "MovementComponent.h"
#include "AnimationComponent.h"

#pragma region Character Data

std::string CharacterData::GivenName(bool useNickname) const
{
	if (useNickname && nickname.size() > 0)
	{
		return nickname;
	}
	else
	{
		if (reverseName)
		{
			return lastName;
		}
		else
		{
			return firstName;
		}
	}
}

std::string CharacterData::FamilyName() const
{
	if (!reverseName)
	{
		return lastName;
	}
	else
	{
		return firstName;
	}
}

std::string CharacterData::FullName(bool useNickname) const
{
	if (!reverseName)
	{
		return std::string(GivenName(useNickname).append(" ").append(FamilyName()).c_str());
	}
	else
	{
		return std::string(FamilyName().append(" ").append(GivenName(useNickname)).c_str());
	}
}

#pragma endregion 

#pragma region Character

Character::Character(std::string _debugName, float x, float y, float height, float width, const Texture* _texture, float _layer, CharacterData* _characterData) : Sprite(_debugName, x, y, height, width,0,0, height, width, _texture, _layer)
{
	characterData = _characterData;

	AddComponent(new DynamicCollisionComponent(this, rectangle));
	AddComponent(new AnimationComponent(this, sourceRectangle));
}

Character::~Character()
{
	if (active) Deactivate();

	for (int i = 0; i < characterData->Abilities().size(); i++)
	{
		//SafeDelete(characterData->Abilities()[i]);
	}
}

void Character::Draw(SpriteBatch* spriteBatch)
{
	spriteBatch->Draw(this);
}

#pragma endregion

#pragma region Player Character

Player::Player(std::string _debugName, float x, float y, float height, float width, const Texture* _texture, float _layer, CharacterData* _characterData) : Character(_debugName, x, y, height, width, _texture, _layer, _characterData)
{
	AddComponent(new PlayerControlMovementComponent(this, (CollisionComponent*)GetComponentOfType(COMPONENT_TYPE::COLLISION)));
}

Player::~Player()
{
	if (active) Deactivate();
}

void Player::Update(float delta_time)
{
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

bool Player::OnInteractCallback()
{
	if (frozen || !active)
		return false;


	Point<float> interactPoint;

	AnimationComponent* animComponent = (AnimationComponent*)GetComponentOfType(COMPONENT_TYPE::ANIMATION);

	switch (animComponent->GetWayFacing())
	{
	case AnimationComponent::DIRECTION::UP:
		interactPoint = Point<float>(rectangle->CenterX(), rectangle->Top() - (rectangle->Height() / 3.f));
		break;
	case  AnimationComponent::DIRECTION::DOWN:
		interactPoint = Point<float>(rectangle->CenterX(), rectangle->Bottom() + (rectangle->Height() / 3.f));
		break;
	case  AnimationComponent::DIRECTION::LEFT:
		interactPoint = Point<float>(rectangle->Left() - (rectangle->Width() / 3.f), rectangle->CenterY());
		break;
	case  AnimationComponent::DIRECTION::RIGHT:
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
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_INTERACT, std::bind(&Player::OnInteractCallback, this), this);
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_MENU, std::bind(&Player::OnMenuCallback, this), this);
	ResetInputs();
}

void Player::UnbindCallbacks()
{
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

NonPlayer::NonPlayer(std::string _debugName, float x, float y, float height, float width, const Texture* _texture, float _layer, CharacterData* _characterData, float _radius) : Character(_debugName, x, y, height, width, _texture, _layer, _characterData)
{
	/*
	movementMode = _movementMode;
	moveRadius = _radius;

	startLocation = Point<float>(x, y);
	moveToLocation = startLocation;
	*/

	AddComponent(new DialogueComponent(this, rectangle, std::vector<std::string>{"hello!"}));
	AddComponent(new RandomRadiusMovementComponent(this, (CollisionComponent*)GetComponentOfType(COMPONENT_TYPE::COLLISION), _radius, 5.f));
}

NonPlayer::~NonPlayer()
{
	if (active) Deactivate();
}

void NonPlayer::Update(float delta_time)
{

}

#pragma endregion

