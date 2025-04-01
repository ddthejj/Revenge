#include "defines.h"
#include "TriggerComponent.h"
#include "OverworldManager.h"


TriggerComponent::TriggerComponent(ComponentType _type, bool _isUnique) : Component(_type, _isUnique)
{
}

DoorComponent::DoorComponent(int _destination, int _x, int _y) : TriggerComponent(ComponentType::DoorTrigger, true)
{
	destination = _destination;
	x = _x;
	y = _y;
}

void DoorComponent::Trigger()
{
	OverworldManager::HitDoor(destination, x, y);
}
