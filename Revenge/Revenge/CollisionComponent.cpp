#include "defines.h"
#include "CollisionComponent.h"

CollisionComponent::CollisionComponent() : Component(ComponentType::Collision)
{

}

CollisionComponent::~CollisionComponent()
{
	SafeDelete(collider);
}
