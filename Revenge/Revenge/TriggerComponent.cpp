#include "defines.h"
#include "TriggerComponent.h"
#include "OverworldManager.h"
#include "CollisionComponent.h"


TriggerComponent::TriggerComponent(COMPONENT_TYPE _type, bool _isUnique, MyRectangle* _rectangle) : Component(_type, _isUnique)
{
	rectangle = _rectangle;
}

DoorComponent::DoorComponent(MyRectangle* _rectangle, int _destination, int _x, int _y) : TriggerComponent(COMPONENT_TYPE::DOOR_TRIGGER, true, _rectangle)
{
	destination = _destination;
	x = _x;
	y = _y;
}

void DoorComponent::Trigger(CollisionComponent* component)
{
	OverworldManager::HitDoor(destination, x, y);
}
