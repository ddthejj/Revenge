#include "defines.h"
#include "MovementComponent.h"
#include "CollisionComponent.h"
#include "PhysicsManager.h"
#include "Character.h"
#include "Rectangle.h"
#include "InputManager.h"
#include "AnimationComponent.h"
#include <random>
#include <ctime>
#include <cmath>

MovementComponent::MovementComponent(Object* _owner, CollisionComponent* _collisionComp) : Component(_owner, COMPONENT_TYPE::MOVEMENT)
{
	collisionComp = _collisionComp;
}

void MovementComponent::Move(float delta_time)
{
	AnimationComponent* animComp = (AnimationComponent*)owner->GetComponentOfType(COMPONENT_TYPE::ANIMATION);

	if (!IsMoving())
	{
		if (animComp)
		{
			animComp->AnimateMovement(delta_time, this);
		}
		return;
	}

	if (IsMoving() && collisionComp)
	{
		collided = PhysicsManager::TryMove(collisionComp, velocity);
	}

	if (animComp)
	{
		animComp->AnimateMovement(delta_time, this);
	}
}

void MovementComponent::Freeze()
{
	if (!frozen)
	{
		Manager::RemoveUpdate(this);
		Object::Freeze();
	}
}

void MovementComponent::Unfreeze()
{
	if (frozen)
	{
		Manager::AddUpdate(this);
		Object::Unfreeze();
	}
}

PlayerControlMovementComponent::PlayerControlMovementComponent(Object* _owner, CollisionComponent* _collisionComp) : MovementComponent(_owner, _collisionComp)
{

}

void PlayerControlMovementComponent::Update(float delta_time)
{
  	Move(delta_time);
}

void PlayerControlMovementComponent::BindCallbacks()
{
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_UP, std::bind(&PlayerControlMovementComponent::UpPressedCallback, this), this);
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_DOWN, std::bind(&PlayerControlMovementComponent::DownPressedCallback, this), this);
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_LEFT, std::bind(&PlayerControlMovementComponent::LeftPressedCallback, this), this);
	InputManager::KeyPressedCallbacks_Attach(KEYS::KEY_RIGHT, std::bind(&PlayerControlMovementComponent::RightPressedCallback, this), this);

	InputManager::KeyReleasedCallbacks_Attach(KEYS::KEY_UP, std::bind(&PlayerControlMovementComponent::UpReleasedCallback, this), this);
	InputManager::KeyReleasedCallbacks_Attach(KEYS::KEY_DOWN, std::bind(&PlayerControlMovementComponent::DownReleasedCallback, this), this);
	InputManager::KeyReleasedCallbacks_Attach(KEYS::KEY_LEFT, std::bind(&PlayerControlMovementComponent::LeftReleasedCallback, this), this);
	InputManager::KeyReleasedCallbacks_Attach(KEYS::KEY_RIGHT, std::bind(&PlayerControlMovementComponent::RightReleasedCallback, this), this);
}

void PlayerControlMovementComponent::UnbindCallbacks()
{
	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_UP, std::bind(&PlayerControlMovementComponent::UpPressedCallback, this), this);
	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_DOWN, std::bind(&PlayerControlMovementComponent::DownPressedCallback, this), this);
	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_LEFT, std::bind(&PlayerControlMovementComponent::LeftPressedCallback, this), this);
	InputManager::KeyPressedCallbacks_Remove(KEYS::KEY_RIGHT, std::bind(&PlayerControlMovementComponent::RightPressedCallback, this), this);

	InputManager::KeyReleasedCallbacks_Remove(KEYS::KEY_UP, std::bind(&PlayerControlMovementComponent::UpReleasedCallback, this), this);
	InputManager::KeyReleasedCallbacks_Remove(KEYS::KEY_DOWN, std::bind(&PlayerControlMovementComponent::DownReleasedCallback, this), this);
	InputManager::KeyReleasedCallbacks_Remove(KEYS::KEY_LEFT, std::bind(&PlayerControlMovementComponent::LeftReleasedCallback, this), this);
	InputManager::KeyReleasedCallbacks_Remove(KEYS::KEY_RIGHT, std::bind(&PlayerControlMovementComponent::RightReleasedCallback, this), this);
}

bool PlayerControlMovementComponent::UpPressedCallback()
{
	velocity += Point<float>(0.f, -mvmntSpeed);
	return false;
}

bool PlayerControlMovementComponent::DownPressedCallback()
{
	velocity += Point<float>(0.f, mvmntSpeed);
	return false;
}

bool PlayerControlMovementComponent::LeftPressedCallback()
{
	velocity += Point<float>(-mvmntSpeed, 0.f);
	return false;
}

bool PlayerControlMovementComponent::RightPressedCallback()
{
	velocity += Point<float>(mvmntSpeed, 0.f);
	return false;
}

bool PlayerControlMovementComponent::UpReleasedCallback()
{
	velocity -= Point<float>(0.f, -mvmntSpeed);
	return false;
}

bool PlayerControlMovementComponent::DownReleasedCallback()
{
	velocity -= Point<float>(0.f, mvmntSpeed);
	return false;
}

bool PlayerControlMovementComponent::LeftReleasedCallback()
{
	velocity -= Point<float>(-mvmntSpeed, 0.f);
	return false;
}

bool PlayerControlMovementComponent::RightReleasedCallback()
{
	velocity -= Point<float>(mvmntSpeed, 0.f);
	return false;
}

RandomRadiusMovementComponent::RandomRadiusMovementComponent(Object* _owner, CollisionComponent* _collisionComp, float _moveRadius, float _moveDelay) : MovementComponent(_owner, _collisionComp),
moveRadius(_moveRadius), moveDelay(_moveDelay)
{ 
	startLocation = collisionComp->collider->Location();
	moveToLocation = collisionComp->collider->Location();
}

void RandomRadiusMovementComponent::Update(float delta_time)
{
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
		if (moveToLocation.Equals(collisionComp->collider->Location(), mvmntSpeed))
		{
			collisionComp->collider->SetLocation(moveToLocation);

			velocity = Point<float>(0.f, 0.f);
		}
	}


	if (moveToLocation != collisionComp->collider->Location())
	{
		velocity = moveToLocation - collisionComp->collider->Location();
		velocity.Normalize();
		velocity *= mvmntSpeed;
	}

	Move(delta_time);

	// if we collide
	if (collided)
	{
		moveToLocation = collisionComp->collider->Location();
	}
}

PatrolMovementComponent::PatrolMovementComponent(Object* _owner, CollisionComponent* _collisionComp) : MovementComponent(_owner, _collisionComp)
{
}

FollowMovementComponent::FollowMovementComponent(Object* _owner, CollisionComponent* _collisionComp) : MovementComponent(_owner, _collisionComp)
{
}

ChaseMovementComponent::ChaseMovementComponent(Object* _owner, CollisionComponent* _collisionComp) : MovementComponent(_owner, _collisionComp)
{
}
