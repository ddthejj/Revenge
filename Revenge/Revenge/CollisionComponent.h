#pragma once
#include "Component.h"

class CollisionComponent : public Component
{
public:

	MyRectangle* collider = nullptr;

	CollisionComponent();
	virtual ~CollisionComponent();

	virtual void Collide() = 0;
};
