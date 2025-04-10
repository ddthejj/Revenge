#pragma once
#include "Component.h"

class MovementComponent;

class AnimationComponent : public Component
{
public:

	// Which way the actor is facing
	enum class DIRECTION
	{
		UP, DOWN, LEFT, RIGHT, MAX
	};

protected:

	DIRECTION wayFacing = DIRECTION::DOWN;	// way the actor is looking for animation
	float animTimer = 0.f;					// timer for overworld animation
	float animTimerMax = 1.f;				// max timer of the movement animation

	MyRectangle* sourceRectangle = nullptr;

public:

	AnimationComponent(Object* _owner, MyRectangle* _sourceRectangle);

	const DIRECTION GetWayFacing() const { return wayFacing; }

	void AnimateMovement(float delta_time, MovementComponent* movementComp);

};

