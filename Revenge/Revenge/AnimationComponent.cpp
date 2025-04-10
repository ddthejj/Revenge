#include "defines.h"
#include "AnimationComponent.h"
#include "MovementComponent.h"
#include "Rectangle.h"

AnimationComponent::AnimationComponent(Object* _owner, MyRectangle* _sourceRectangle) : Component(_owner, COMPONENT_TYPE::ANIMATION)
{
	sourceRectangle = _sourceRectangle;
}

void AnimationComponent::AnimateMovement(float delta_time, MovementComponent* movementComp)
{
	if (movementComp->GetVelocity().Magnitude() > 0.f)
	{
		DIRECTION prevWayFacing = wayFacing;

		if (std::abs(movementComp->GetVelocity().x) > std::abs(movementComp->GetVelocity().y))
		{
			if (movementComp->GetVelocity().x < 0)
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
			if (movementComp->GetVelocity().y < 0)
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
