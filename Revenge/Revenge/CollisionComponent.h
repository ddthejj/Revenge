#pragma once
#include "Component.h"

class CollisionComponent : public Component
{
public:

	MyRectangle* collider = nullptr;

	CollisionComponent(COMPONENT_TYPE type);
	virtual ~CollisionComponent();

	virtual void Collide(CollisionComponent* collider) = 0;
	virtual void Register() override;
	virtual void Unregister() override;
};

class StaticCollisionComponent : public CollisionComponent
{
public:

	StaticCollisionComponent(MyRectangle* rectangle);
	virtual void Collide(CollisionComponent* collider) override {};
};

class DynamicCollisionComponent : public CollisionComponent
{
public:

	DynamicCollisionComponent(MyRectangle* rectangle);
	virtual void Collide(CollisionComponent* collider) override {};
};