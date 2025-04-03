#include "defines.h"
#include "CollisionComponent.h"
#include "PhysicsManager.h"

CollisionComponent::CollisionComponent(COMPONENT_TYPE type) : Component(type)
{

}

CollisionComponent::~CollisionComponent()
{

}

void CollisionComponent::Register()
{
	// register collision with the physics system alongside the component manager
	Component::Register();

	PhysicsManager::RegisterCollision(this);
}

void CollisionComponent::Unregister()
{
	// register collision with the physics system alongside the component manager
	Component::Unregister();

	PhysicsManager::UnregisterCollision(this);
}

StaticCollisionComponent::StaticCollisionComponent(MyRectangle* rectangle) : CollisionComponent(COMPONENT_TYPE::STATIC_COLLISION)
{
	collider = rectangle;
}

DynamicCollisionComponent::DynamicCollisionComponent(MyRectangle* rectangle) : CollisionComponent(COMPONENT_TYPE::DYNAMIC_COLLISION)
{
	collider = rectangle;
}
