#pragma once
#include "Component.h"

class CollisionComponent : public Component
{
public:

	MyRectangle* collider = nullptr;

	CollisionComponent(Object* _owner, COMPONENT_TYPE type);
	virtual ~CollisionComponent();

	virtual void Collide(CollisionComponent* collider) = 0;
	virtual void Register() override;
	virtual void Unregister() override;
};

class StaticCollisionComponent : public CollisionComponent
{
public:

	StaticCollisionComponent(Object* _owner, MyRectangle* rectangle);
	virtual void Collide(CollisionComponent* collider) override {};
};

class DynamicCollisionComponent : public CollisionComponent
{
public:

	DynamicCollisionComponent(Object* _owner, MyRectangle* rectangle);
	virtual void Collide(CollisionComponent* collider) override {};
};