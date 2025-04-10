#pragma once
#include "Component.h"

class CollisionComponent;

class MovementComponent : public Component
{
protected:

	CollisionComponent* collisionComp = nullptr;

	Point<float> velocity = { 0.f, 0.f };	// actor overworld movement velocity
	float mvmntSpeed = 2.f;					// how quickly the cdaasharacter moves across the map
	bool collided = false;					// whether the character has collided with something this frame

public:

	MovementComponent(Object* _owner, CollisionComponent* _collisionComp);

	virtual void Freeze() override;
	virtual void Unfreeze() override;

	// move the actor based on velocity
	virtual void Move(float delta_time);
	// check if actor is moving
	bool IsMoving() { return velocity.Magnitude() != 0.f; }
	const bool HasCollided() const { return collided; }
	const Point<float> GetVelocity() const { return velocity; }

	void AddMovement(Point<float> speed) { velocity += speed; }
};

class PlayerControlMovementComponent : public MovementComponent
{
	CollisionComponent* collisionComp = nullptr;

public:

	PlayerControlMovementComponent(Object* _owner, CollisionComponent* _collisionComp);

	virtual void Update(float delta_time) override;
	virtual void BindCallbacks() override;
	virtual void UnbindCallbacks() override;

	bool UpPressedCallback();
	bool UpReleasedCallback();
	bool DownPressedCallback();
	bool DownReleasedCallback();
	bool LeftPressedCallback();
	bool LeftReleasedCallback();
	bool RightPressedCallback();
	bool RightReleasedCallback();
};

class RandomRadiusMovementComponent : public MovementComponent
{
	float moveRadius = 0.f;
	float moveTimer = 0.f, moveDelay = 5.f;
	Point<float> startLocation;
	Point<float> moveToLocation;

public:

	RandomRadiusMovementComponent(Object* _owner, CollisionComponent* _collisionComp, float _moveRadius, float _moveDelay);

	virtual void Update(float delta_time) override;
};

class PatrolMovementComponent : public MovementComponent
{
public:

	PatrolMovementComponent(Object* _owner, CollisionComponent* _collisionComp);
};

class FollowMovementComponent : public MovementComponent
{
public:

	FollowMovementComponent(Object* _owner, CollisionComponent* _collisionComp);
};

class ChaseMovementComponent : public MovementComponent
{
public:

	ChaseMovementComponent(Object* _owner, CollisionComponent* _collisionComp);
};