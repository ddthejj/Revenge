#include "defines.h"
#include "CollisionComponent.h"
#include "PhysicsManager.h"

CollisionComponent::CollisionComponent(Object* _owner, COMPONENT_TYPE type) : Component(_owner, type)
{

}

CollisionComponent::~CollisionComponent()
{

}

void CollisionComponent::Activate()
{
	// register collision with the physics system alongside the component manager
	Component::Activate();

	PhysicsManager::RegisterCollision(this);
}

void CollisionComponent::Deactivate()
{
	// register collision with the physics system alongside the component manager
	Component::Deactivate();

	PhysicsManager::UnregisterCollision(this);
}

StaticCollisionComponent::StaticCollisionComponent(Object* _owner, MyRectangle* rectangle) : CollisionComponent(_owner, COMPONENT_TYPE::STATIC_COLLISION)
{
	collider = rectangle;
}

DynamicCollisionComponent::DynamicCollisionComponent(Object* _owner, MyRectangle* rectangle) : CollisionComponent(_owner, COMPONENT_TYPE::DYNAMIC_COLLISION)
{
	collider = rectangle;
}
